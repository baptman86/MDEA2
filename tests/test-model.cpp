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
#include "model/Model.h"
#include <fstream>
#include <iostream>

TEST_GROUP(ModelTests) {};

TEST(ModelTests, FirstTest) {
  Model* testc = new Model("chrom-sample.chr");
  std::ifstream infile("chrom-sample.chr");
  int a, nb = 0;
  while(infile >> a) {
    LONGS_EQUAL(a,testc->getVal()->at(nb++));
  }
  LONGS_EQUAL(testc->getVal()->size(),testc->getDomains()->size());
  delete testc;
}
