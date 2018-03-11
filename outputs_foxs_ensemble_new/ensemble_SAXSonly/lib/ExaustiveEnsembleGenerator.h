/**
 * \file ExaustiveEnsembleGenerator.h
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_EXAUSTIVE_ENSEMBLEGENERATOR_H
#define IMP_EXAUSTIVE_ENSEMBLEGENERATOR_H

#include "Ensemble.h"
#include "EnsembleScore.h"

class ExaustiveEnsembleGenerator {
public:
  ExaustiveEnsembleGenerator(unsigned int population_size,
                             EnsembleScore *score);

  void generate(unsigned int ensemble_size, std::string out_file_name);
  void generate_intervals(unsigned int ensemble_size, std::string out_file_name);

private:
  void add_one_member(const std::vector<Ensemble>& init_ensembles,
                      std::vector<Ensemble>& new_ensembles);

  void add_one_member_from_interval(
                    const std::vector<Ensemble>& init_ensembles,
                    std::vector<Ensemble>& new_ensembles,
                    std::pair<unsigned int, unsigned int> interval);
  void score(std::string out_file_name);

private:
  unsigned int N_;
  EnsembleScore *score_; // turn into vector of scores
  std::vector<Ensemble> ensembles_;
};

#endif /* IMP_EXAUSTIVE_ENSEMBLEGENERATOR_H */
