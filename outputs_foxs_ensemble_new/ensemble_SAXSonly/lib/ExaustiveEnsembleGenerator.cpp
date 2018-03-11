#include "ExaustiveEnsembleGenerator.h"

ExaustiveEnsembleGenerator::ExaustiveEnsembleGenerator(unsigned int population_size,
                                                       EnsembleScore *score) :
  N_(population_size), score_(score) {
}


void ExaustiveEnsembleGenerator::generate(unsigned int ensemble_size,
                                          std::string out_file_name) {

  std::vector<Ensemble> curr_ensembles, new_ensembles;

  // init ensembles of size 1
  curr_ensembles.reserve(N_);
  for(unsigned int i=0; i<N_; i++) {
    Ensemble e(1); e.add_member(i);
    curr_ensembles.push_back(e);
  }

  for(unsigned int i=1; i<ensemble_size; i++) {
    new_ensembles.reserve(curr_ensembles.size()*((N_-1)/2.0));
    add_one_member(curr_ensembles, new_ensembles);
    curr_ensembles = new_ensembles;
    new_ensembles.clear();
  }
  ensembles_ = curr_ensembles; // save last ensembles
  score(out_file_name);
}

void ExaustiveEnsembleGenerator::generate_intervals(unsigned int ensemble_size,
                                                    std::string out_file_name) {
   unsigned int interval_size = (std::floor)(N_/(float)ensemble_size);

   std::vector<Ensemble> curr_ensembles, new_ensembles;

  // init ensembles of size 1
  curr_ensembles.reserve(interval_size);
  for(unsigned int i=0; i<interval_size; i++) {
    Ensemble e(1); e.add_member(i);
    curr_ensembles.push_back(e);
  }

  for(unsigned int i=1; i<ensemble_size; i++) {
    new_ensembles.reserve(curr_ensembles.size()*((interval_size-1)/2.0));
    add_one_member_from_interval(curr_ensembles, new_ensembles,
                          std::make_pair(i*interval_size, (i+1)*interval_size));
    curr_ensembles = new_ensembles;
    new_ensembles.clear();
  }
  ensembles_ = curr_ensembles; // save last ensembles
  score(out_file_name);
}

void ExaustiveEnsembleGenerator::add_one_member_from_interval(
                    const std::vector<Ensemble>& init_ensembles,
                    std::vector<Ensemble>& new_ensembles,
                    std::pair<unsigned int, unsigned int> interval) {

  for(unsigned int i=0; i<init_ensembles.size(); i++) {
    for(unsigned int j=interval.first; j<interval.second; j++) {
      Ensemble new_ensemble(init_ensembles[i]);
      new_ensemble.add_member(j);
      new_ensembles.push_back(new_ensemble);
    }
  }
}

void ExaustiveEnsembleGenerator::add_one_member(
                    const std::vector<Ensemble>& init_ensembles,
                    std::vector<Ensemble>& new_ensembles) {

  for(unsigned int i=0; i<init_ensembles.size(); i++) {
    unsigned int first_to_search = init_ensembles[i].get_last_member()+1;
    for(unsigned int j=first_to_search; j<N_; j++) {
      Ensemble new_ensemble(init_ensembles[i]);
      new_ensemble.add_member(j);
      new_ensembles.push_back(new_ensemble);
    }
  }
}

void ExaustiveEnsembleGenerator::score(std::string out_file_name) {
  std::ofstream out_file(out_file_name.c_str());
  for(unsigned int i=0; i<ensembles_.size(); i++) {
    double score = score_->get_score(ensembles_[i]);
    ensembles_[i].set_score(score);
    std::cerr << ensembles_[i] << std::endl;
    if(score > 0.0) {
      IMP::saxs::WeightedFitParameters fp = score_->get_fit_parameters(ensembles_[i]);
      score_->output(out_file, ensembles_[i], fp);
    }
  }
  out_file.close();
}
