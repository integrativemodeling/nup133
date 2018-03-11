/**
 * \file ProfileClustering
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_PROFILE_CLUSTERING_H
#define IMP_PROFILE_CLUSTERING_H

#include <IMP/saxs/Profile.h>

#include <vector>

class ProfileClustering {
public:
  ProfileClustering(IMP::saxs::Profile* exp_profile,
                    const IMP::saxs::Profiles& profiles,
                    float chi_percentage = 0.3, float chi_threshold = 0.0);

  ProfileClustering(IMP::saxs::Profile* exp_profile,
                    const IMP::saxs::Profiles& profiles,
                    const std::vector<double>& scores,
                    float chi_percentage = 0.3, float chi_threshold = 0.0);

  const std::vector<IMP::saxs::Profiles>& get_clusters() const {
    return clusters_;
  }

  const IMP::saxs::Profiles& get_representatives() const {
    return clustered_profiles_;
  }

private:
  void cluster_profiles();
  void select_representatives();

private:
  IMP::base::PointerMember<const IMP::saxs::Profile> exp_profile_;
  const IMP::saxs::Profiles profiles_;
  const std::vector<double> scores_;

  std::vector<double> chi_scores_;
  IMP::saxs::Profiles clustered_profiles_;
  std::vector<IMP::saxs::Profiles> clusters_;
  float chi_percentage_;
  float chi_threshold_;
};

#endif /* IMP_PROFILE_CLUSTERING_H */
