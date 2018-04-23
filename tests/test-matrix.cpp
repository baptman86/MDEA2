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
#include "model/Matrix.h"

TEST_GROUP(MatrixTests) {};

TEST(MatrixTests, MatrixAccessor) {
  auto m0 = new Matrix(2);

  m0->set(0,1,0.5);

  DOUBLES_EQUAL(0.5,(*m0)[0][1],0.0001);
  
  delete m0;
}

TEST(MatrixTests, MatrixSubstraction) {
  auto m1 = new Matrix(2);
  auto m2 = new Matrix(2);
  auto m3 = new Matrix(2);
  auto m4 = new Matrix(2);

  m1->set(0,1,0.5);
  m2->set(0,1,0.5);
  m3->set(0,1,0.7);
  m4->set(0,1,0.2);
  
  auto m5 = *m1 - *m2;
  auto m6 = *m3 - *m2;
  auto m7 = *m2 - *m3;
  auto m8 = *m4 - *m2;
  auto m9 = *m2 - *m4;
  
  DOUBLES_EQUAL(0,(*m5)[0][1],0.0001);
  CHECK_TEXT(!*m5,("This matrix is not null: \n"+
  		   m5->to_string()).c_str());
  DOUBLES_EQUAL(0.2,(*m6)[0][1],0.0001);
  DOUBLES_EQUAL(-0.2,(*m7)[0][1],0.0001);
  DOUBLES_EQUAL(-0.3,(*m8)[0][1],0.0001);
  DOUBLES_EQUAL(0.3,(*m9)[0][1],0.0001);
  
  delete m1;
  delete m2;
  delete m3;
  delete m4;
  delete m5;
  delete m6;
  delete m7;
  delete m8;
  delete m9;
}
