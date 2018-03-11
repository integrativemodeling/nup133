/**
 * \file EM2DEnsembleScore.h
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_EM2D_ENSEMBLE_SCORE_H
#define IMP_EM2D_ENSEMBLE_SCORE_H

#include <limits>

class EM2DEnsembleScore : public EnsembleScore {
public:
  EM2DEnsembleScore(std::string file_name) {

    //read_file(file_name);
  }

  double get_score(const Ensemble& e) const {
    //return minimum for each ensemble conformation
    double ret = 0;
    const std::vector<unsigned int>& members = e.get_members();
    unsigned int class_average_num = scores[0].size();
    for(unsigned int i = 0; i<class_average_num; i++) {
      float min_score = std::numeric_limits<float>::max();
      for(unsigned int j=0; j<members.size(); j++) {
        if(scores[j][i] < min_score) min_score = scores[j][i];
      }
      ret += min_score;
    }
    return ret;
  }

  // return uniform weights???
  IMP::saxs::WeightedFitParameters
  get_fit_parameters(Ensemble& e) const {
    IMP::Floats weights(e.size(), 1.0/e.size());
    return IMP::saxs::WeightedFitParameters(get_score(e), 1.0, 0.0, weights);
  }

  // write fit file
  void write_fit_file(Ensemble& e,
                      const IMP::saxs::WeightedFitParameters& fp,
                      const std::string fit_file_name) const {}

  // output ensemble data
  void output(std::ostream& s, Ensemble& e,
              IMP::saxs::WeightedFitParameters& p) const {}

  // output multiple ensembles data, including averages and stdevs on the set
  void output(std::ostream& s, std::vector<Ensemble>& e,
              std::vector<IMP::saxs::WeightedFitParameters>& p) const {}
private:
  // store scores - how?
  std::vector<std::vector<float> > scores;
};

#endif /* IMP_EM2D_ENSEMBLE_SCORE_H */
