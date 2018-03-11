/**
 * \file SAXSClustering
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_SAXSCLUSTERING_H
#define IMP_SAXSCLUSTERING_H

class SAXSClustering {
public:
  SAXSClustering();



  std::vector<Profile*> profiles_;
  std::vector<Profile*> clustered_profiles_;
  std::vector<std::vector<Profile*> > clusters_;
};

#endif /* IMP_SAXSCLUSTERING_H */
