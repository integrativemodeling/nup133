#include "EnsembleGenerator.h"
#include "stat_helpers.h"

#include <IMP/base/exception.h>

#include <algorithm>
#include <boost/tuple/tuple.hpp>
#include <queue>
#include <fstream>
#include <map>

EnsembleGenerator::EnsembleGenerator(unsigned int population_size,
                                     unsigned int best_k,
                                     EnsembleScore *score) :
  N_(population_size), K_(best_k), score_(score) {
}

void EnsembleGenerator::init() {
  // reserve space
  ensembles_.insert(ensembles_.begin(), 100, std::vector<Ensemble>());
  ensembles_[0].reserve(N_);

  // generate & score N ensembles of size 1
  for(unsigned int i=0; i<N_; i++) {
    Ensemble e(1);
    e.add_member(i);
    e.set_score(score_->get_score(e));
    ensembles_[0].push_back(e);
  }

  std::sort(ensembles_[0].begin(), ensembles_[0].end(), CompareEnsembles());

  // erase low scoring ensembles
  if(ensembles_[0].size() > K_) {
    ensembles_[0].erase(ensembles_[0].begin()+K_, ensembles_[0].end());
    std::cerr << "Number of ensembles of size 1: " << ensembles_[0].size() << std::endl;
  }

  rescore_and_output(ensembles_[0]);
}

void EnsembleGenerator::generate(unsigned int ensemble_size) {
  init();
  if(ensembles_.size() < ensemble_size) {
    std::cerr << "Only ensembles up to " << ensemble_size
              << " members are supported" << std::endl;
    return;
  }
  for(unsigned int i=1; i<ensemble_size; i++) {
    if(ensembles_[i-1].size() > 0) add_one_member(ensembles_[i-1], ensembles_[i]);
    rescore_and_output(ensembles_[i]);
  }
}

void EnsembleGenerator::rescore_and_output(std::vector<Ensemble>& ensembles) const {

  unsigned int print_num = std::min((unsigned int)ensembles.size(), K_);

  std::vector<IMP::saxs::WeightedFitParameters> fps, sorted_fps;
  fps.reserve(print_num);
  std::multimap<float, unsigned int> scores;
  std::vector<double> v_scores; // for average and std

  // re-score
  for(unsigned int i=0; i<print_num; i++) {
    if(i>0 && i%100==0) {
      std::cerr << "Rescoring ensemble " << i << " out of " << print_num << std::endl;
    }
    IMP::saxs::WeightedFitParameters p=score_->get_fit_parameters(ensembles[i]);
    fps.push_back(p);
    // do not output ensembles with one of the weights below threshold
    if(ensembles[i].get_score() < 0.0) continue;
    scores.insert(std::make_pair(ensembles[i].get_score(), i));
    v_scores.push_back(ensembles[i].get_score());
  }


  // sort
  std::vector<Ensemble> sorted_ensembles;
  sorted_ensembles.reserve(print_num);
  sorted_fps.reserve(print_num);
  std::multimap<float, unsigned int>::iterator it, end_it = scores.end();
  for(it = scores.begin(); it != end_it; it++) {
    //std::cerr << "score = " << it->first << std::endl;
    sorted_ensembles.push_back(ensembles[it->second]);
    sorted_fps.push_back(fps[it->second]);
  }

  // output
  if(sorted_ensembles.size() > 0) {
    unsigned int ensemble_size = sorted_ensembles[0].size();
    std::string out_file_name = "ensembles_size_" +
      std::string(boost::lexical_cast<std::string>(ensemble_size)) + ".txt";
    std::ofstream out_file(out_file_name.c_str());
    std::cerr << sorted_ensembles.size () << " ensembles of size " << ensemble_size << std::endl;

    std::pair<double, double> average_and_std = get_average_and_stdev(v_scores);
    for(unsigned int i=0; i<sorted_ensembles.size(); i++) {
      // set z-score
      double zscore = (sorted_ensembles[i].get_score()-average_and_std.first)/average_and_std.second;
      sorted_ensembles[i].set_zscore(zscore);

      // output fit file
      if(i<10) { // TODO: add parameter
        std::string fit_file_name = "ensemble_size" +
          std::string(boost::lexical_cast<std::string>(ensemble_size)) + "_" +
          std::string(boost::lexical_cast<std::string>(i+1)) + ".dat";
        score_->write_fit_file(sorted_ensembles[i], sorted_fps[i], fit_file_name);
      }
    }
    score_->output(out_file, sorted_ensembles, sorted_fps);
  }
}

namespace {
struct Comparator {
  bool operator()(const boost::tuple<double, int, int>& p1,
                  const boost::tuple<double, int, int>& p2) {
    return boost::get<0>(p1) < boost::get<0>(p2);
  }
};
}

void EnsembleGenerator::add_one_member(
                    const std::vector<Ensemble>& init_ensembles,
                    std::vector<Ensemble>& new_ensembles) {

  std::priority_queue<boost::tuple<double, int, int>,
                      std::vector<boost::tuple<double, int, int> >,
                      Comparator> bestK;

  // iterate over all init ensembles and try to add a new member to each
  for(unsigned int i=0; i<init_ensembles.size(); i++) {

    unsigned int first_to_search = init_ensembles[i].get_last_member()+1;

    if(first_to_search<N_) {

      if(i>0 && i%100==0 && !bestK.empty()) {
        double curr_bestK_score = boost::get<0>(bestK.top());
        std::cerr << "Extending ensemble: " << i << " out of "
                  << init_ensembles.size() << " last best " << curr_bestK_score << std::endl;
      }

      Ensemble new_ensemble(init_ensembles[i]);
      new_ensemble.add_member(first_to_search);

      // try all possible additions of a new member
      for(unsigned int j=first_to_search; j<N_; j++) {
        new_ensemble.replace_last_member(j);

        double curr_score = score_->get_score(new_ensemble);
        if(curr_score < 0.0) continue; // invalid ensemble

        // add to bestK
        if(bestK.size() <= K_ || curr_score < boost::get<0>(bestK.top())) {
          bestK.push(boost::make_tuple(curr_score, i, j));
          if(bestK.size() > K_) bestK.pop();
        }
      }
    }
  }

  // save best scoring
  new_ensembles.assign(bestK.size(), Ensemble(0));
  int index = bestK.size()-1;
  // generate bestK new ensembles
  while(!bestK.empty()) {
    double score;
    int ensemble_index, new_member_index;
    boost::tie(score, ensemble_index, new_member_index) = bestK.top();
    Ensemble new_ensemble(init_ensembles[ensemble_index]);
    new_ensemble.add_member(new_member_index);
    new_ensemble.set_score(score);
    new_ensembles[index] = new_ensemble;
    index--;
    bestK.pop();
  }
}
