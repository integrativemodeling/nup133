/**
 * \file EnsembleScore base class for ensemble scoring classes
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_ENSEMBLE_SCORE_H
#define IMP_ENSEMBLE_SCORE_H

#include "Ensemble.h"
#include <IMP/saxs/WeightedFitParameters.h>

class EnsembleScore {
public:
  // ensemble score
  virtual double get_score(const Ensemble& e) const = 0;

  // ensemble score, weights and other fitting params
  virtual IMP::saxs::WeightedFitParameters
                   get_fit_parameters(Ensemble& e) const = 0;

  // write fit file
  virtual void write_fit_file(Ensemble& e,
                              const IMP::saxs::WeightedFitParameters& fp,
                              const std::string fit_file_name) const = 0;

  // output ensemble data
  virtual void output(std::ostream& s, Ensemble& e,
                      IMP::saxs::WeightedFitParameters& p) const = 0;

  // output multiple ensembles data, including averages and stdevs on the set
  virtual void output(std::ostream& s, std::vector<Ensemble>& e,
                    std::vector<IMP::saxs::WeightedFitParameters>& p) const = 0;

};

#endif /* IMP_ENSEMBLE_SCORE_H */
