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
 * \file Statistics.h
 * \brief Statistics class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 03/05/16
 *
 * This statistics class implementation for the evolutionary algorithm.
 *
 */

#pragma once
#include "Population.h"
#include "utils/Logger.h"

/**
 * \class Statistics
 * \brief A class to store genetic algorithm stats.
 *
 * This class provides utility to record statistics of the
 * evolutionary algorithm.
 *
 * \author Florian Galinier
 */
class Statistics {
 private:
  std::vector<float> med;
  std::vector<float> firstQuartile;
  std::vector<float> thirdQuartile;
  std::vector<float> min;
  std::vector<float> max;
 public:
  /**
   * The name of the output file
   */
  static std::string outfile;
  /**
   * Use population to update stats
   * \param p Population used for stats analysis.
   */
  void update(GAPopulation& p);
  /**
   * Getter for medians (each entry is a generation)
   * \return A vector with all recorded medians.
   */
  const std::vector<float>& getMed() const;
  /**
   * Getter for first quartiles (each entry is a generation)
   * \return A vector with all recorded first quartiles.
   */
  const std::vector<float>& getFirstQuartile() const;
  /**
   * Getter for third quartiles (each entry is a generation)
   * \return A vector with all recorded third quartiles.
   */
  const std::vector<float>& getThirdQuartile() const;
  /**
   * Getter for minimums (each entry is a generation)
   * \return A vector with all recorded minimums.
   */
  const std::vector<float>& getMin() const;
  /**
   * Getter for maximums (each entry is a generation)
   * \return A vector with all recorded maximums.
   */
  const std::vector<float>& getMax() const;
};

/**
 * Specialization of Logger operator<< for statistics
 * \param log The logger where to write
 * \param s The statistics to write
 * \return The used logger
 */
Logger& operator<<(Logger& log, const Statistics& s);
