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
 * \file Population.h
 * \brief Population class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 06/04/16
 *
 * Population that will be used in our genetic algorithm.
 *
 */

#pragma once
#include "GAChromosom.h"
#include "utils/Logger.h"
#include <ga/GAPopulation.h>

/**
 * \class Population
 * \brief This class manage all chromosoms (especially the initial ones).
 *
 * This class provide methods for population management.
 *
 * \author Florian Galinier
 */
class Population : public GAPopulation {
 public:
  /**
   * Default constructor.
   */
  Population();
  /**
   * Create a population from files given in directory.
   * \param dirPath The directory where the models files are.
   */
  Population(std::string dirPath);
  /**
   * Evaluate all individuals of population
   * \param Force or not the evaluation
   */
  void evaluate(GABoolean flag = gaFalse) const;
  /**
   * Evaluate all individuals of population via a population point of
   * view.
   * \param Force or not the evaluation
   * \return The matrix of distances.
   */
  MatrixPtr popEvaluate(GABoolean flag = gaFalse);
  /**
   * Return the best element of population for the
   * objectif in matrix score at m[r][c].
   * \param r Matrix row of the objective.
   * \param c Matrix column of the objective
   * \return Best individual for objective score[r][c] 
   */
  GAChromosom& best(size_t r,size_t c);
  /**
   * Return the worst element of population for the
   * objectif in matrix score at m[r][c].
   * \param r Matrix row of the objective.
   * \param c Matrix column of the objective
   * \return Worst individual for objective score[r][c] 
   */
  GAChromosom& worst(size_t r,size_t c);
 private:
  /**
   * Add a new chromosom from the given models
   * \param files The models used for chromosom creation.
   */
  virtual void newChromosom(const std::vector<std::string>& files); 
};
/**
 * Specialization of Logger operator<< for Population
 * \param log The logger where to write
 * \param p The population to write
 * \return The used logger
 */
Logger& operator<<(Logger& log, const Population& p);
