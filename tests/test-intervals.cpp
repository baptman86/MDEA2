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
#include "utils/Interval.h"
#include "utils/IntervalVector.h"

TEST_GROUP(IntervalTests) {};

TEST(IntervalTests, TestInterval) {
  Interval<float>* i1 = new Interval<float>(1,1);
  Interval<float>* i2 = new Interval<float>(-9,-5);
  Interval<float>* i3 = new Interval<float>("3");
  Interval<float>* i4 = new Interval<float>("-9..-5");
  CHECK_THROWS(std::logic_error,Interval<float>(1,0));

  LONGS_EQUAL(1,i1->lbound());
  LONGS_EQUAL(1,i1->ubound());
  LONGS_EQUAL(-9,i2->lbound());
  LONGS_EQUAL(-5,i2->ubound());
  LONGS_EQUAL(3,i3->lbound());
  LONGS_EQUAL(3,i3->ubound());
  LONGS_EQUAL(-9,i4->lbound());
  LONGS_EQUAL(-5,i4->ubound());
  CHECK(i1->include(1));
  CHECK_FALSE(i1->include(2));
  CHECK_FALSE(i1->include(0));
  CHECK(i2->include(-9));
  CHECK(i2->include(-5));
  CHECK(i2->include(-7));
  CHECK_FALSE(i2->include(-10));
  CHECK_FALSE(i2->include(-4));
  
  delete i1;
  delete i2;
  delete i3;
  delete i4;
}


TEST(IntervalTests, TestIntervalVector) {
  IntervalVector<float>* iv = new IntervalVector<float>();
  CHECK_THROWS(std::out_of_range,iv->lbound());
  CHECK_THROWS(std::out_of_range,iv->ubound());
  CHECK_FALSE(iv->include(8));

  LONGS_EQUAL(0,iv->add(5,9));
  LONGS_EQUAL(0,iv->add(1,3));
  LONGS_EQUAL(0,iv->add(11,13));
  LONGS_EQUAL(1,iv->add("4..6"));
  LONGS_EQUAL(1,iv->add(7,10));
  LONGS_EQUAL(2,iv->add(6,8));

  CHECK_FALSE(iv->include(0));
  CHECK(iv->include(1));
  CHECK(iv->include(2));
  CHECK(iv->include(3));
  CHECK_FALSE(iv->include(3.5));
  CHECK(iv->include(4));
  CHECK(iv->include(6));
  CHECK(iv->include(10));
  CHECK_FALSE(iv->include(10.5));
  CHECK(iv->include(11));
  CHECK(iv->include(12));
  CHECK(iv->include(13));
  CHECK_FALSE(iv->include(13.5));
  
  delete iv;
}
