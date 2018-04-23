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

#include "Statistics.h"

std::string Statistics::outfile = "";

void Statistics::update(GAPopulation& pop) {
  auto size = pop.size();
  pop.sort();
  max.push_back(pop.individual(0).score());
  min.push_back(pop.individual(size-1).score());
  auto is_odd = (size%2 == 1);
  auto quart_pos = (size%4 == 0)?size/4:(size+1)/4;
  
  if(is_odd) {
    med.push_back(pop.individual(size/2 + 1).score());
  }
  else {
    med.push_back((pop.individual(size/2).score()+
		   pop.individual(size/2+1).score())/2);
  }

  thirdQuartile.push_back(pop.individual(quart_pos).score());
  firstQuartile.push_back(pop.individual(3*quart_pos).score());
}

const std::vector<float>& Statistics::getMed() const {
  return med;
}

const std::vector<float>& Statistics::getFirstQuartile() const {
  return firstQuartile;
}

const std::vector<float>& Statistics::getThirdQuartile() const {
  return thirdQuartile;
}

const std::vector<float>& Statistics::getMin() const {
  return min;
}

const std::vector<float>& Statistics::getMax() const {
  return max;
}

Logger& operator<<(Logger& log, const Statistics& s) {
  log<<"gen med uq lq max min\n";
  auto med = s.getMed();
  auto fq = s.getFirstQuartile();
  auto tq = s.getThirdQuartile();
  auto min = s.getMin();
  auto max = s.getMax();
  for(auto i = 0u;i < med.size();i++) {
    log<<i<<" "<<med[i]<<" "<<tq[i]<<" "<<fq[i]<<" "
    <<max[i]<<" "<<min[i]<<"\n";
  }
  return log;
}


