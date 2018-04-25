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
 * \file NSGAII.h
 * \brief NSGAII class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 15/04/16
 *
 * NSGA-II class implementation for the evolutionary algorithm.
 *
 */

#pragma once
#include <ga/GASimpleGA.h>
#include <vector>
#include <map>
#include "Statistics.h"
#include "Population.h"

/**
 * \class NSGAII
 * \brief Implementation of NSGA-II algorithm.
 *
 * This class implement the GAGeneticAlgorithm interface of GAlib API
 * in order to provide the NSGA-II algorithm.
 *
 * \author Florian Galinier
 */
class NSGAII : public GASimpleGA {
 private:
  Statistics extraStats;
  unsigned int popMult;
 public:
  /**
   * \brief Mask for fitness choice.
   * Represent each fitness choice. Fitness can be
   * used as mask with logical operator.
   * \author Florian Galinier
   */
  enum Fitness {
    MIN = 0b0001,
    AVG = 0b0010,
    DIST = 0b0000,
    MINAVG = 0b0100
  };
  /**
   * Output Directory
   */
  static std::string dir;
  /**
   * Choosed fitness
   */
  static int fitness;
  /**
   * Total number of generations.
   */
  static unsigned int maxGen;
  /**
   * Current generation.
   */
  static unsigned int gen;
  /* Identity definition for GAlib */
  GADefineIdentity("NSGAII", 288);
  /**
   * Constructor for new NSGA-II Algorithm
   * \param p The initial population
   */
  NSGAII(const GAPopulation& p,unsigned int pm = 1);
  /**
   * Destructor
   */
  virtual ~NSGAII() noexcept;
  /**
   * Compare two matrices of score and return true if
   * the first one dominate the second one.
   * \param m1 The first matrix to test.
   * \param m2 The second one.
   * \return true iff m1 dominates m2.
   */
  virtual bool dominate(MatrixPtr m1, MatrixPtr m2);
  /**
   * Sort population in Pareto front.
   * \return A vector of front composed by key of front's individual.
   */
  virtual std::shared_ptr<std::vector<std::vector<int>>>
    fastNonDominatedSort();
  /**
   * Assign to each individual of a front a crowding distance value.
   * \param f A vector of individual in a front
   * \return A map that associate at each 
   * individual a crowding distance.
   */
  virtual std::map<size_t,double>
    crowdingDistanceAssignment(std::vector<int>& f);
  /**
   * Select popSize individual from popr in p by using
   * clustering based on our k-max approach.
   * \param popr The population where to select individuals
   * \param p The out population
   * \param popSize Number of individual to select in population
   * \param res The matrix of distances used for clustering 
   */
  virtual void clustering(::Population* popr,
			  ::Population* p,
			  unsigned int popSize,
			  MatrixPtr res);
  /**
   * Create and evaluate a new generation.
   */
  virtual void step();
  /**
   * Return the recorded extra-statistics of the algorithù.
   * \return Statistics of the run
   */
  virtual Statistics& extraStatistics();
  /**
   * Create dot files associate to a generation
   */
  virtual void generateDotGen(std::string metaModelName, int genNumber);
  /**
   * Operator that create a new generation.
   * \return The NSGA-II algorithm with a new population.
   */
  NSGAII & operator++();
};
