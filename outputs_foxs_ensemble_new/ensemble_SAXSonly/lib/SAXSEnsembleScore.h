/**
 * \file SAXSEnsembleScore
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_SAXS_ENSEMBLE_SCORE_H
#define IMP_SAXS_ENSEMBLE_SCORE_H

#include "EnsembleScore.h"
#include "Ensemble.h"

#include <IMP/saxs/Profile.h>
#include <IMP/saxs/WeightedProfileFitter.h>
#include <IMP/base/Object.h>

#include <vector>

class SAXSEnsembleScore : public EnsembleScore {
public:

  /* if c1_c2_approximate=true, get_score will return approximate score
     based on average c1/c2
     min_weight_threshold, ensembles with one or more weights below
     min_weight_threshold will get a negative score
  */
  SAXSEnsembleScore(const IMP::saxs::Profiles& profiles,
                    const IMP::saxs::Profile* exp_profile,
                    bool c1_c2_approximate, float min_weight_threshold = 0.05);

  double get_score(const Ensemble& e) const;

  IMP::saxs::WeightedFitParameters get_fit_parameters(Ensemble& e) const;

  void write_fit_file(Ensemble& e,
                      const IMP::saxs::WeightedFitParameters& fp,
                      const std::string fit_file_name) const;

  void output(std::ostream& s, Ensemble& e,
              IMP::saxs::WeightedFitParameters& p) const;

  void output(std::ostream& s, std::vector<Ensemble>& e,
              std::vector<IMP::saxs::WeightedFitParameters>& p) const;

  void output_matrix(std::ostream& s, float c1, float c2) const;

  float get_average_c1() const { return average_c1_; }
  float get_average_c2() const { return average_c2_; }

private:
  void resample(const IMP::saxs::Profile* exp_profile,
                const IMP::saxs::Profiles& profiles,
                IMP::saxs::Profiles& resampled_profiles);

  void set_average_c1_c2(IMP::saxs::WeightedProfileFitter* score,
                         const IMP::saxs::Profiles& profiles,
                         float& c1, float& c2);

private:
  // input profiles
  const IMP::saxs::Profiles profiles_;
  IMP::base::PointerMember<const IMP::saxs::Profile> exp_profile_;

  // resampled on experimental profile q's
  IMP::saxs::Profiles resampled_profiles_;

  // scoring with exp_profile_
  IMP::saxs::WeightedProfileFitter* score_;

  float average_c1_, average_c2_;
  bool c1_c2_approximate_;

  float min_weight_threshold_; // default=5%
};

#endif /* IMP_SAXS_ENSEMBLE_SCORE_H */
