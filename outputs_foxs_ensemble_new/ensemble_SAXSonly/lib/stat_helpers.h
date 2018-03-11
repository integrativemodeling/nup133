/**
 * \file stat_helpers.h
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */
#ifndef IMP_STAT_HELPERS_H
#define IMP_STAT_HELPERS_H

#include <vector>
#include <utility>

double get_average(const std::vector<double>& v);

std::pair<double, double> get_average_and_stdev(const std::vector<double>& v);


#endif /* IMP_STAT_HELPERS_H */
