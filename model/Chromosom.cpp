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

#include "Chromosom.h"
#include <fstream>
#include <iostream>
#include <ga/garandom.h>

/* Static variable initialization */

size_t Chromosom::nbModels = 2;

/* Constructors */

Chromosom::Chromosom(): models(std::vector<Model>()) {}

Chromosom::Chromosom(const std::vector<std::string>& files): models(std::vector<Model>()) {
  for(auto file : files) {
    models.push_back(file);
  }
}

Chromosom::~Chromosom() {}

/* Accessors */

void Chromosom::setModels(std::vector<Model> models) {
  this->models = models;
}

std::vector<Model>& Chromosom::getModels() {
  return this->models;
}

size_t Chromosom::setNbModels(size_t nb) {
  return (Chromosom::nbModels = nb);
}

size_t Chromosom::getNbModels() {
  return Chromosom::nbModels;
}

/* Mutate operator */

int Chromosom::mutate(float pmut) {
  if(pmut <= 0.0) return 0;
  int nb = 0;
  for(auto& m : models) {
    int nbDom = 0;
    for(auto& val : *(m.getVal())) {
      if(GARandomFloat(0.0,1.0) <= pmut) {
	auto dom = m.getDomains()->at(nbDom);
	do {
	  val = GARandomInt(dom.lbound(),dom.ubound());
	} while(!dom.include(val));
	nb++;
      }
      nbDom++;
    }
  }
  return nb;
}

bool Chromosom::isValid() const {
  auto valid = true;
  for(auto m : models) {
    valid &= m.isValid();
    if(!valid) break;
  }
  return valid;
}
