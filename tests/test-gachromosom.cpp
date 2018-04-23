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

#include <CppUTest/TestHarness.h>
#include "model/GAChromosom.h"
#include <fstream>

TEST_GROUP(GAChromosomTests) {};

SimpleString StringFrom(Model& m) {
  return SimpleString(m.to_string().c_str());
}

TEST(GAChromosomTests, OnePointCrossoverTest) {
  std::vector<std::string> dadModels;
  std::vector<std::string> momModels;
  dadModels.push_back("tests/sample-rep/m1.chr");
  dadModels.push_back("tests/sample-rep/m2.chr");
  momModels.push_back("tests/sample-rep/m3.chr");
  momModels.push_back("tests/sample-rep/m4.chr");
  Model m1("tests/sample-rep/m1.chr");
  Model m2("tests/sample-rep/m2.chr");
  Model m3("tests/sample-rep/m3.chr");
  Model m4("tests/sample-rep/m4.chr");
  GAChromosom* dad = new GAChromosom(dadModels);
  GAChromosom* mom = new GAChromosom(momModels);
  GAChromosom* sis = new GAChromosom();
  GAChromosom* bro = new GAChromosom();

  CHECK_EQUAL(const_cast<Model&>(mom->getModels().at(0)),m3);
  CHECK_EQUAL(const_cast<Model&>(mom->getModels().at(1)),m4);
  
  GAChromosom::onePointCrossover(*dad,*mom,sis,bro);
  
  CHECK_EQUAL(const_cast<Model&>(sis->getModels().at(0)),m1);
  CHECK_EQUAL(const_cast<Model&>(sis->getModels().at(1)),m4);
  CHECK_EQUAL(const_cast<Model&>(bro->getModels().at(0)),m3);
  CHECK_EQUAL(const_cast<Model&>(bro->getModels().at(1)),m2);
  
  delete dad;
  delete mom;
  delete sis;
  delete bro;
}
