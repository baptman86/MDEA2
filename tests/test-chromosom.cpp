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
#include "model/Chromosom.h"
#include <fstream>

TEST_GROUP(ChromosomTests) {};

SimpleString StringFrom(Model& m);

TEST(ChromosomTests, FirstTest) {
  Chromosom::setNbModels(2);
  std::vector<std::string> models;
  models.push_back("tests/sample-rep/m1.chr");
  models.push_back("tests/sample-rep/m2.chr");
  Model m1("tests/sample-rep/m1.chr");
  Model m2("tests/sample-rep/m2.chr");
  Chromosom* testc = new Chromosom(models);
  LONGS_EQUAL(Chromosom::getNbModels(),testc->getModels().size());
  CHECK_EQUAL(const_cast<Model&>(testc->getModels().at(0)),m1);
  CHECK_EQUAL(const_cast<Model&>(testc->getModels().at(1)),m2);
  delete testc;
}

TEST(ChromosomTests, MutationTest) {
  std::vector<std::string> models1;
  models1.push_back("tests/sample-rep/m1.chr");
  models1.push_back("tests/sample-rep/m2.chr");
  Model m1("tests/sample-rep/m1.chr");
  Model m2("tests/sample-rep/m2.chr");
  std::vector<std::string> models2;
  models2.push_back("tests/sample-rep/m3.chr");
  models2.push_back("tests/sample-rep/m4.chr");
  Model m3("tests/sample-rep/m3.chr");
  Model m4("tests/sample-rep/m4.chr");
  Chromosom* c1 = new Chromosom(models1);
  Chromosom* c2 = new Chromosom(models2);
  c1->mutate(1.0);
  c2->mutate(0.0);
  
  CHECK_TEXT(const_cast<Model&>(c1->getModels().at(0)) != m1,m1.to_string().c_str());
  CHECK_TEXT(const_cast<Model&>(c1->getModels().at(1)) != m2,m2.to_string().c_str());  
  CHECK_TEXT(const_cast<Model&>(c2->getModels().at(0)) == m3,m3.to_string().c_str());
  CHECK_TEXT(const_cast<Model&>(c2->getModels().at(1)) == m4,m4.to_string().c_str());  

  delete c1;
  delete c2;
}
