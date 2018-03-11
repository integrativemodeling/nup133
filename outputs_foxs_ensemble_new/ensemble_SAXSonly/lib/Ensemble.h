/**
 * \file Ensemble
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_ENSEMBLE_H
#define IMP_ENSEMBLE_H

#include <vector>
#include <iostream>
#include <iomanip>

class Ensemble {
public:
  Ensemble(unsigned int size) { members_.reserve(size); score_=0; zscore_=0; }

  /* Modifiers */

  void add_member(unsigned int new_member) {
    members_.push_back(new_member);
    // invalidate score
    score_ =  0.0;
  }

  void set_score(double score) { score_ = score; }

  void set_zscore(double score) { zscore_ = score; }

  void replace_last_member(unsigned int new_member) {
    members_[members_.size()-1] = new_member;
  }

  /* Access */

  bool is_member(unsigned int candidate) const {
    for(unsigned int i=0; i<members_.size(); i++)
      if(members_[i] == candidate) return true;
    return false;
  }

  const std::vector<unsigned int>& get_members() const { return members_; }

  double get_score() const { return score_; }

  double get_zscore() const { return zscore_; }

  unsigned int size() const { return members_.size(); }

  unsigned int get_last_member() const { return members_.back(); }

  friend std::ostream& operator<<(std::ostream& s, const Ensemble& e) {
    for(unsigned int i=0; i<e.members_.size(); i++)
      s << std::setw(4) << e.members_[i] << " ";
    s  << std::setw(6) << std::setprecision(4) << e.score_;
    return s;
  }

private:
  std::vector<unsigned int> members_;

  double score_;
  double zscore_;
};



class CompareEnsembles {
public:
  bool operator()(const Ensemble& e1, const Ensemble& e2) const{
    return (e1.get_score() < e2.get_score());
  }
};


#endif /* IMP_ENSEMBLE_H */
