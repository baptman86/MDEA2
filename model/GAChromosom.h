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
 * \file GAChromosom.h
 * \brief GAChromosom class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 06/04/16
 *
 * Declaration of our own genome class.
 *
 */

#pragma once
#include "Chromosom.h"
#include <ga/GAGenome.h>
#include <functional>
#include "utils/Logger.h"
#include "Matrix.h"

/**
 * Smart pointer to a Matrix.
 */
typedef std::shared_ptr<Matrix> MatrixPtr;

/**
 * \class GAChromosom
 * \brief Class that implement a genome for our problem.
 *
 * This class implement the GAGenome interface of GAlib API and is a modelisation of our problem.
 *
 * \author Florian Galinier
 */
class GAChromosom : public Chromosom, public GAGenome {
 private:
  std::vector<MatrixPtr> sc;
 public:
  enum Method {
    COSINE      = 0b00001,
    HAMMING     = 0b00010,
    CENTRALITY  = 0b00100,
    LEVENSHTEIN = 0b01000,
    LEVEXTERN   = 0b10000, 
  };
  static int method;
  static enum Cross { INTRA, INTER } crossover;
  /* Identity definition for GAlib */
  GADefineIdentity("GAChromosom", 201);
  /** 
   * Default constructor that create a new genome.
   */
  GAChromosom();
  /** 
   * Create a new genome from files.
   * \param path The vector of files path
   */
  GAChromosom(const std::vector<std::string>& files);
  /**
   * Create genome that is a copy of parameter
   * \param g The chromosom to copy
   */
  GAChromosom(const GAChromosom& g);
  /**
   * GAChromosom destructor
   */
  virtual ~GAChromosom() noexcept;
  /**
   * Set the matrix score.
   * \param m The new matrix score.
   * \return The new matrix score.
   */
  virtual std::vector<MatrixPtr> score(std::vector<MatrixPtr> m);
  /**
   * Return the matrix score (if matrix is null, score is
   * not yet set).
   * \return The matrix score.
   */
  virtual std::vector<MatrixPtr> score() const;
  /**
   * Return a new GAGenome that is a copy of actual one.
   * \param method Clone method to use for copy (optional)
   * \return A clone of actual GAChromosom
   */
  virtual GAGenome* clone(CloneMethod method) const;
  /**
   * Change actual GAChromosom by copying g values.
   * \param g GAGenome to copy 
   */
  virtual void copy(const GAGenome& g);

  /**
   * Change actual value of GAChromosom
   * \param models The new value of GAChromosom
   */
  virtual void setModels(std::vector<Model> models);
  /**
   * Convert chromosom to a string representation.
   * \return A string representation of current chromosom.
   */
  std::string to_string() const;
  /**
   * Print GAChromosom value in param stream.
   * \param os The stream where to write
   * \return Undefined 
   */
  virtual int write(std::ostream& os) const;
  
  /**
   * Function that compare two GAChromosom and return a distance value.
   * \param g1 First GAGenome to compare
   * \param g2 Second GAGenome to compare
   * \return A float that is equal to 0 if g1 and g2 are the same, -1 if an error occurred and another float greater than 0 that is a degree of difference if g1 and g2 can be compared.
   */
  static float compare(const GAGenome& g1, const GAGenome& g2);
  /**
   * Initialize a new GAChromosom.
   * \param g The GAChromosom to initialize
   */
  static void init(GAGenome& g);
  /**
   * Cross two GAChromosom with intra crossover and
   * return two new one by translocation. 
   * \param dad First GAChromosom to cross
   * \param mom Second GAChoromosom to cross
   * \param sis The first offspring crossed chromosom
   * \param bro The second offspring crossed chromosom
   * \return Number of new chromosom obtain by crossover
   */
  static int onePointIntraCrossover(const GAGenome& dad, const GAGenome& mom, GAGenome* sis, GAGenome* bro);
  /**
   * Cross two GAChromosom and return two new one by translocation. 
   * \param dad First GAChromosom to cross
   * \param mom Second GAChoromosom to cross
   * \param sis The first offspring crossed chromosom
   * \param bro The second offspring crossed chromosom
   * \return Number of new chromosom obtain by crossover
   */
  static int onePointCrossover(const GAGenome& dad, const GAGenome& mom, GAGenome* sis, GAGenome* bro);
  /**
   * Give a fitness value of the actual GAChromosom
   * \param gaFlag Force or not the evaluation
   * \return The fitness value of the GAChromosom
   */
  virtual std::vector<MatrixPtr> evaluate(GABoolean gaFlag = gaFalse);
  /**
   * Give an average fitness value for 
   * the given GAChromosom (greater is better)
   * \param g The GAChromosom to evaluate
   * \return The fitness value of the GAChromosom
   */
  static float avgEvaluator(GAGenome& g);
  /**
   * Give a matrix of distance value for
   * the given GAChromosom (greater is better)
   * \param g The GAChromosom to evaluate
   * \return The fitness value of the GAChromosom
   */
  static std::vector<MatrixPtr> evaluate(GAGenome& g);
  /**
   * Mutate the given GAChromosom.
   * \param g The GAChromosom to mutate
   * \param pmut The percentage chance of mutation
   * \return The number of mutation
   */
  static int mutate(GAGenome& g, float pmut);
};
Logger& operator<<(Logger& log, const GAChromosom& c);
