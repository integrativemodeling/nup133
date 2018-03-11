/**
 * \file EnsembleGenerator
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_ENSEMBLE_GENERATOR_H
#define IMP_ENSEMBLE_GENERATOR_H

#include "Ensemble.h"
#include "EnsembleScore.h"

class EnsembleGenerator {
public:
  EnsembleGenerator(unsigned int population_size,
                    unsigned int best_k,
                    std::vector<EnsembleScore*>& scorers);


  void generate(unsigned int ensemble_size);

private:
  void init();

  // enumerate ensembles of size N from ensembles of size N-1
  void add_one_member(const std::vector<Ensemble>& init_ensembles,
                      std::vector<Ensemble>& new_ensembles);

  void rescore_and_output(std::vector<Ensemble>& ensembles) const;

  double get_score(const Ensemble& e) const {
    double ret = 0;
    for(unsigned int i=0; i<scorers_.size(); i++) {
      ret += scorers_[i]->get_score(e);
    }
    return ret;
  }

private:
  unsigned int N_;
  unsigned int K_;
  std::vector<EnsembleScore*> scorers_; // turn into vector of scores
  // TODO: add single scores
  std::vector<std::vector<Ensemble> > ensembles_;
};

#endif /* IMP_ENSEMBLE_GENERATOR_H */
