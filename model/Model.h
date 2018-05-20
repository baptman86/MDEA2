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
 * \file Model.h
 * \brief Model class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 06/04/16
 *
 * The representation of a model in our problem is an int vector. Due to the fact that a chromosom can be composed by more than one model vector, we choose to reify it in a Model class.
 *
 */

#pragma once
#include <vector>
#include <memory>
#include <lib/pugixml-1.8/src/pugixml.hpp>
#include "utils/IntervalVector.h"

/** 
 * In our problem, a gene vector is a vector of int.
 * We create an alias for convenience.
 */
typedef std::vector<int> Genes;
/**
 * Smart pointer to a genes vector.
 */
typedef std::shared_ptr<Genes> GenesPtr;
/**
 * A vector of all domains. For all i in {0..Genes.size()},
 * Domains[i] = domains of Genes[i]
 */
typedef std::vector<IntervalVector<int>> Domains;
/**
 * Smart pointer to a domains vector.
 */
typedef std::shared_ptr<Domains> DomainsPtr;

class GAChromosom;

/**
 * \class Model
 * \brief Class that represent a model.
 *
 * This class is a gene vector representation of a model.
 * Due to the fact that a model can be composed by more than a model,
 * we create a class that encapsulate the idea of model.
 *
 * \author Florian Galinier
 */
class Model {
  friend GAChromosom;
 protected:
  GenesPtr genes;
  DomainsPtr domains;
  std::string xcsp;
  std::string mm;
  std::string root;
  std::string grimm;
  std::string dot;
  bool change;
 public:
  /**
   * Create a new empty Model.
   */
  Model();
  /**
   * Create a Model that is a copy of Model m.
   * \param m The Model to copy.
   */
  Model(const Model& m);
  /**
   * Create a new model with data from input file. 
   * \param genesFile The file path that contains vector value for model.
   */
  Model(std::string genesFile);
  /**
   * Model destructor.
   */
  virtual ~Model() noexcept;
  
  /**
   * Change actual value of Model
   * \deprecated
   * \param v The new value of Model 
   */
  virtual void setVal(GenesPtr v);
  
  /**
   * Return actual value of Model
   * \return The current value of Model
   */
  virtual GenesPtr getVal() const;

  /**
   * Change actual value of Model
   * \deprecated
   * \param v The new value of Model 
   */
  virtual void setDomains(DomainsPtr d);
  
  /**
   * Return domains of genes in the Model
   * \return The variable domains of Model
   */
  virtual DomainsPtr getDomains() const;

  /**
   * Take two models and return an evaluation of the distance between them.
   * \param m1 First model to test.
   * \param m2 Second model to test.
   * \return The distance between the two models.
   */
  static std::tuple<float,float,float> evaluateExtern(Model& m1, Model& m2);

  /**
   * Take two models and return an evaluation of the distance between them.
   * \param m1 First model to test.
   * \param m2 Second model to test.
   * \return The distance between the two models.
   */
  static float evaluate(const Model& m1,const Model& m2);
  
  /**
   * Take two models and return an evaluation of the cosine distance between
   * them.
   * \param m1 First model to test.
   * \param m2 Second model to test.
   * \return The cosine distance between the two models.
   */
  static float cosineDistance(const Model& m1,const Model& m2);
  
  /**
   * Take two models and return an evaluation of the levenshtein distance 
   * between them.
   * \param m1 First model to test.
   * \param m2 Second model to test.
   * \return The levenshtein distance between the two models.
   */
  static float levenshteinDistance(const Model& m1,const Model& m2);
  
  std::string generateDotFileScaffold(std::string fileName);

  /**
   * Check if the Model is a valid model or not.
   * \return true if Model is valid, elsewhere false.
   */
  virtual bool isValid() const;
  
  /**
   * Compare two models and return true if models are equals.
   * \param b Other model to compare
   * \return true if this and b are equal, false elsewhere.
   */
  virtual bool operator==(const Model& b);
  /**
   * Compare two models and return true if models are differents.
   * \param b Other model to compare
   * \return true if this and b are not equal, false elsewhere.
   */
  virtual bool operator!=(const Model& b); 
  /**
   * Return a string that represent current model.
   * \return A string that represent current model.
   */
  virtual std::string to_string() const;

  /**
   * Generate a dot file from actual model.
   * \return The dot file path.
   */
  virtual std::string generateDotFile(int i);
};
/**
 * operator<< overload for iostream.
 * \param os The ostream where to write.
 * \param m The model to write.
 * \return The resulting ostream.
 */
std::ostream& operator<<(std::ostream& os,const Model& m);
