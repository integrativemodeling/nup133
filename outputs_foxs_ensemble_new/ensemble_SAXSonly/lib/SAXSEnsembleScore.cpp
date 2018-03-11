#include "SAXSEnsembleScore.h"
#include "stat_helpers.h"

#include <IMP/saxs/WeightedProfileFitter.h>

SAXSEnsembleScore::SAXSEnsembleScore(
                          const IMP::saxs::Profiles& profiles,
                          const IMP::saxs::Profile* exp_profile,
                          bool c1_c2_approximate, float min_weight_threshold) :
  profiles_(profiles), exp_profile_(exp_profile),
  c1_c2_approximate_(c1_c2_approximate),
  min_weight_threshold_(min_weight_threshold) {

  if(profiles_.size() < 1) {
    IMP_THROW("SAXSEnsembleScore - please provide at least one profile"
              << std::endl, IOException);
  }

  // resample all models profiles
  resample(exp_profile_, profiles_, resampled_profiles_);

  // init scoring object
  score_ = new IMP::saxs::WeightedProfileFitter(exp_profile_);

  // compute average c1/c2
  set_average_c1_c2(score_, resampled_profiles_, average_c1_, average_c2_);
  //std::cerr << "Average_C1_" << average_c1_ << " c2 " << average_c2_ << std::endl;
}

void SAXSEnsembleScore::resample(const IMP::saxs::Profile* exp_profile,
                                 const IMP::saxs::Profiles& profiles,
                                 IMP::saxs::Profiles& resampled_profiles) {

  resampled_profiles.reserve(profiles.size());
  for(unsigned int i=0; i<profiles.size(); i++) {
    IMP::saxs::Profile *resampled_profile =
      new IMP::saxs::Profile(exp_profile->get_min_q(), exp_profile->get_max_q(),
                             exp_profile->get_delta_q());
    profiles[i]->resample(exp_profile, resampled_profile, true);
    resampled_profiles.push_back(resampled_profile);
  }
}

void SAXSEnsembleScore::set_average_c1_c2(
                                 IMP::saxs::WeightedProfileFitter* score,
                                 const IMP::saxs::Profiles& profiles,
                                 float& c1, float& c2) {
  c1 = 0.0;
  c2 = 0.0;
  IMP::saxs::ProfilesTemp profiles_temp(1);
  for(unsigned int i=0; i<profiles.size(); i++) {
    profiles_temp[0] = profiles[i];
    IMP::saxs::WeightedFitParameters fp =
      score->fit_profile(profiles_temp, 0.95, 1.05, -2.0, 4.0);
    c1+= fp.get_c1();
    c2+= fp.get_c2();
  }
  c1 /= profiles.size();
  c2 /= profiles.size();
  for(unsigned int i=0; i<profiles.size(); i++) {
    profiles[i]->sum_partial_profiles(c1, c2);
  }
}


double SAXSEnsembleScore::get_score(const Ensemble& e) const {
  const std::vector<unsigned int>& members = e.get_members();
  IMP::saxs::ProfilesTemp profiles(members.size());
  for(unsigned int i=0; i<members.size(); i++) {
    profiles[i] = resampled_profiles_[members[i]];
    if(c1_c2_approximate_)
      profiles[i]->sum_partial_profiles(average_c1_, average_c2_);
  }

  std::vector<double> weights;
  double chi;
  if(c1_c2_approximate_) {
    chi = score_->compute_score(profiles, weights);
  } else { // fit c1/c2
    IMP::saxs::WeightedFitParameters fp =
      score_->fit_profile(profiles, 0.95, 1.05, -2.0, 4.0);
    chi = fp.get_chi();
  }

  return chi;
}

IMP::saxs::WeightedFitParameters
               SAXSEnsembleScore::get_fit_parameters(Ensemble& e) const {
  const std::vector<unsigned int>& members = e.get_members();
  IMP::saxs::ProfilesTemp profiles(members.size());
  for(unsigned int i=0; i<members.size(); i++)
    profiles[i] = resampled_profiles_[members[i]];

  IMP::saxs::WeightedFitParameters fp =
    score_->fit_profile(profiles, 0.95, 1.05, -2.0, 4.0);
  e.set_score(fp.get_chi());

  // check the weight contribution of each member
  for(unsigned int i=0; i<fp.get_weights().size(); i++) {
    if(fp.get_weights()[i]<min_weight_threshold_) {
      e.set_score(-1);
      break;
    }
  }
  return fp;
}

void SAXSEnsembleScore::write_fit_file(Ensemble& e,
                                     const IMP::saxs::WeightedFitParameters& fp,
                                     const std::string fit_file_name) const {
  const std::vector<unsigned int>& members = e.get_members();
  IMP::saxs::ProfilesTemp profiles(members.size());
  for(unsigned int i=0; i<members.size(); i++)
    profiles[i] = resampled_profiles_[members[i]];
  score_->write_fit_file(profiles, fp, fit_file_name);
}

void SAXSEnsembleScore::output(std::ostream& s, Ensemble& e,
                               IMP::saxs::WeightedFitParameters& fp) const {
  s.setf(std::ios::fixed, std::ios::floatfield);
  s << std::setw(6) << std::setprecision(3)
    << " Ensemble_score = " << e.get_score() << " z-score " << e.get_zscore()
    << " Chi = " << fp.get_chi() << " c1 = " << fp.get_c1()
    << " c2 = " << fp.get_c2() << std::endl;

  const std::vector<unsigned int>& members = e.get_members();
  for(unsigned int i=0; i<members.size(); i++) {
    s << std::setw(4) << members[i] << " " << profiles_[members[i]]->get_name();
    if(fp.get_weights().size() > i)
      s << std::setw(5) << std::setprecision(3) << "(" << fp.get_weights()[i] << ") ";
    s << std::endl;
  }
}

void SAXSEnsembleScore::output(std::ostream& s,
                               std::vector<Ensemble>& ensembles,
                               std::vector<IMP::saxs::WeightedFitParameters>& fps) const {

  unsigned int N = profiles_.size();
  std::vector<unsigned int> members_counters(N, 0);
  std::vector<std::vector<double> > members_weights(N);
  unsigned int total_member_num = 0;

  // count the number of occurences of each profile in ensembles
  // (members_counters) and store the weights (members_weights)
  for(unsigned int i=0; i<ensembles.size(); i++) {
    const std::vector<unsigned int>& members = ensembles[i].get_members();
    const std::vector<double>& weights = fps[i].get_weights();
    for(unsigned int k=0; k<members.size(); k++) {
      members_counters[members[k]]++;
      members_weights[members[k]].push_back(weights[k]);
      total_member_num++;
    }
  }

  // compute member probs and weight variance
  std::vector<double> member_prob(N, 0.0);
  std::vector<double> weight_average(N, 0.0);
  std::vector<double> weight_variance(N, 0.0);

  // compute the probability of each profile to appear in the ensembles
  // (member_prob), it's average weight across ensembles (weight_average)
  // and variance (weight_variance)
  for(unsigned int i=0; i<N; i++) {
    if(members_counters[i] > 0) {
      if(members_counters[i] == 1) {
         member_prob[i] = 1.0/total_member_num;
         weight_average[i] = members_weights[i][0];
         weight_variance[i] = 1.0;
      } else {
        member_prob[i] = members_counters[i]/(double)ensembles.size();
        std::pair<double, double> av_std = get_average_and_stdev(members_weights[i]);
        weight_average[i] = av_std.first;
        weight_variance[i] = av_std.second;
      }
      std::cerr << i << " # ensembles " << members_counters[i]
                << " probability " << member_prob[i]
                << " weight_average " << weight_average[i]
                << " weight_var " << weight_variance[i] << std::endl;
    }
  }

  // output ensembles with probabilities and weight average/variance
  s.setf(std::ios::fixed, std::ios::floatfield);
  for(unsigned int i=0; i<ensembles.size(); i++) {
    s << i << " Ensemble_score = " << std::setw(6) << std::setprecision(3)
      << ensembles[i].get_score() << " z-score " << ensembles[i].get_zscore()
      << " Chi = " << fps[i].get_chi() << " c1 = " << fps[i].get_c1()
      << " c2 = " << fps[i].get_c2() << std::endl;

    const std::vector<unsigned int>& members = ensembles[i].get_members();

    for(unsigned int k=0; k<members.size(); k++) {
      s << std::setw(4) << members[k] << " " << profiles_[members[k]]->get_name();
      if(fps[i].get_weights().size() > k) {
        s << std::setw(5) << std::setprecision(3)
          << "(" << fps[i].get_weights()[k] << ") ";
        s << "[ " << member_prob[members[k]] << ", "
          << weight_average[members[k]] << ", "
          << weight_variance[members[k]] << " ] ";
      }
      s << std::endl;
    }
  }
}

void SAXSEnsembleScore::output_matrix(std::ostream& s, float c1, float c2) const {
  for(unsigned int i=0; i<resampled_profiles_.size(); i++) {
    resampled_profiles_[i]->sum_partial_profiles(c1, c2);
  }

  for(unsigned int q=0; q<resampled_profiles_[0]->size(); q++) {
    for(unsigned int i=0; i<resampled_profiles_.size(); i++) {
      s << resampled_profiles_[i]->get_intensity(q)/exp_profile_->get_error(q) << ' ';
    }
    s << std::endl;
  }
}
