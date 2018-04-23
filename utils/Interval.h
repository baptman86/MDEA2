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
 * \file Interval.h
 * \brief Interval class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 11/04/16
 *
 * Interval manipulation.
 *
 */

#pragma once
#include <stdexcept>

/**
 * \class Interval
 * \brief An interval representation.
 *
 * This class is an interval representation. Every class
 * that implement comparison operator can be used as
 * template type.
 *
 * \author Florian Galinier
 */
template <typename T>
class Interval {
 private:
  T min;
  T max;
 public:
  /**
   * Constructor for a new interval.
   * \param min Lower bound of interval
   * \param max Upper bound of interval
   */
 Interval(const T& min,const T& max): min(min), max(max) {
    if(min > max)
      throw std::logic_error("Lower bound must be lower than upper bound.");
  }
  /**
   * Parse the string and construct new interval.
   * String must be of type N or N..M
   * \param bounds The string to parse
   */
 Interval(const std::string& bounds): Interval(0,0) {
    auto pos = bounds.find("..");
    if(pos == std::string::npos)
      min = max = std::stoi(bounds);
    else {
      min = std::stoi(bounds.substr(0,pos));
      max = std::stoi(bounds.substr(pos+2,bounds.length()));
    }

    if(min > max)
      throw std::logic_error("Lower bound must be lower than upper bound.");
  }
  /**
   * Destructor for an Interval
   */
  virtual ~Interval() {}
  /**
   * Return lower bound of interval;
   * \return Lower bound of the interval. 
   */
  virtual const T& lbound() const {
    return this->min;
  }
  /**
   * Return upper bound bound of interval;
   * \return Upper bound of the interval. 
   */
  virtual const T& ubound() const {
    return this->max;
  }

  /**
   * Test if a value is included in the interval.
   * \param a The value to test.
   * \return true if a is included in the interval; false elsewhere.
   */
  virtual bool include(const T& a) const  {
    return a >= this->min && a <= this->max;
  }
  /**
   * Convert an interval vector to a string.
   * \return A string representation of the interval
   */
  virtual std::string to_string() const {
    std::string tmp = "[";
    tmp += std::to_string(this->min);
    tmp += ",";
    tmp += std::to_string(this->max);
    tmp += "]";
    return tmp;
  }
};
