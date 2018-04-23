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
 * \file Chromosom.h
 * \brief Chromosom class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 06/04/16
 *
 * Definition of our problem that will be inherited for the genetic algorithm.
 *
 */

#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Model.h"

/**
 * \class Chromosom
 * \brief Class that represent a genome in our problem.
 *
 * This class is a genome representation of our problem.
 *
 * \author Florian Galinier
 */
class Chromosom {
 protected:
  std::vector<Model> models;
  static size_t nbModels;
 public:
  int front;
  /**
   * Create a new empty chromosom. 
   */
  Chromosom();
  /**
   * Create a new chromosom with data from input files. 
   * \param file The file path that contains vector value for chromosom.
   */
  Chromosom(const std::vector<std::string>& files);
  /**
   * Destructor for Chromosom.
   */
  virtual ~Chromosom() noexcept;
  /**
   * Return vector of models
   * \return The vector of models
   */
  virtual std::vector<Model>& getModels();
  /**
   * Change actual value of Chromosom
   * \param v The new value of Chromosom
   */
  static size_t setNbModels(size_t);
  /**
   * Change actual value of Chromosom
   * \param v The new value of Chromosom
   */
  static size_t getNbModels();
  /**
   * Change actual value of Chromosom
   * \param models The new value of Chromosom
   */
  virtual void setModels(std::vector<Model> models);
  /**
   * Mutate models with a chance of pmut percent.
   * \param pmut The percentage chance of mutation
   * \return The number of mutation
   */
  virtual int mutate(float pmut);
  /**
   * Return true if the chromosom own only valid models.
   * \return true iff the chromosom is valid.
   */
  virtual bool isValid() const;
};
