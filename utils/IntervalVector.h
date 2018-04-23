/*
 * This file is part of MDEA.
 * MDEA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * MDEA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with MDEA.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file IntervalVector.h
 * \brief IntervalVector class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 11/04/16
 *
 * This class provide a vector of interval.
 *
 */

#pragma once
#include <vector>
#include "Interval.h"

/**
 * \class IntervalVector
 * \brief A set of disjoint intervals.
 *
 * This class provide a set of interval.
 *
 * \author Florian Galinier
 */
template <typename T>
class IntervalVector {
 private:
  std::vector<Interval<T>> intervals;
 public:
  /**
   * Constructor for a new interval vector.
   */
 IntervalVector() : intervals(std::vector<Interval<T>>()) {}
  /**
   * Constructor for a new interval vector.
   */
 IntervalVector(const IntervalVector& iv) :
  intervals(std::vector<Interval<T>>()) {
    for(auto i : iv.intervals) {
      intervals.push_back({i});
    }
  }
  /**
   * Destructor for an IntervalVector
   */
  virtual ~IntervalVector() {}
  /**
   * Return lower bound of all intervals;
   * \return Lower bound of the interval vector. 
   */
  virtual const T& lbound() const {
    if(!intervals.size()) throw std::out_of_range("No interval set.");
    return intervals[0].lbound();
  }
  /**
   * Return upper bound bound of all intervals;
   * \return Upper bound of the interval vector. 
   */
  virtual const T& ubound() const {
    if(!intervals.size()) throw std::out_of_range("No interval set.");
    return intervals[intervals.size()-1].lbound();
  }
  /**
   * Test if a value is included in one of the intervals.
   * \param a The value to test.
   * \return true if a is included in the interval vector; false elsewhere.
   */
  virtual bool include(const T& a) const {
    for(auto& i : intervals) {
      if(a <= i.ubound())
	return a >= i.lbound();
    }
    return false;
  }
  /**
   * Add an interval to the vector
   * \param i The new interval to add
   * \return 1 if the interval intersect another existing one (there will be merged), 2 if the interval is already covered by an existing one, 0 elsewhere.
   */
  virtual int add(const Interval<T>& i)  {
    for(auto it = intervals.begin(); it < intervals.end(); it++) {
      if(i.ubound() < it->lbound()) {
	intervals.insert(it,i);
	return 0;
      }
      else if(i.lbound() > it->ubound())
	continue;
      else if (i.ubound() <= it->ubound()) {
	if(i.lbound() >= it->lbound())
	  return 2;
	auto ubound = it->ubound();
	intervals.erase(it);
	intervals.insert(it,Interval<T>(i.lbound(),ubound));
	return 1;
      }
      else {
	auto lbound = it->lbound();
	intervals.erase(it);
	intervals.insert(it,Interval<T>(lbound,i.ubound()));
	return 1;
      }
    }
    intervals.push_back(i);
    return 0;
  }
  /**
   * Add an interval to the vector
   * \param bounds The string to parse
   * \return 1 if the interval intersect another existing one (there will be merged), 0 elsewhere.
   */
  virtual int add(std::string bounds) {
    return add(Interval<T>(bounds));
  }
  /**
   * Add an interval to the vector
   * \param min Lower bound of new interval
   * \param max Upper bound of new interval
   * \return 1 if the interval intersect another existing one (there will be merged), 0 elsewhere.
   */
  virtual int add(const T& min, const T& max) {
    return add(Interval<T>(min,max));
  }
  /**
   * Convert an interval to a string.
   * \return A string representation of the interval vector
   */
  virtual std::string to_string() const {
    std::string tmp;
    for(auto i : intervals) {
      tmp += i.to_string();
      tmp += " ";
    }
    return tmp;
  }
};
