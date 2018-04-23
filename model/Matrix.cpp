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

#include "Matrix.h"
#include <limits>

Matrix::Matrix(size_t t): m(t,std::vector<double>(t,0.f)) {}

Matrix::~Matrix() {}

bool Matrix::operator!() const {
  auto res = true;
  for(auto& l : m) {
    for(auto& c : l) {
      res &= (c > -0.0001 && c < 0.0001);
      if(!res) return res;
    }
  }
  return res;
}

bool Matrix::isPositive() const {
  auto res = true;
  for(auto& l : m) {
    for(auto& c : l) {
      res &= (c >= 0);
      if(!res) return res;
    }
  }
  return res;
}

inline void Matrix::set(size_t i,size_t j,double v) {
  m[i][j] = v;
}

Matrix* Matrix::operator-(const Matrix& m2) {
  if(m2.m.size() != m.size())
    throw
      std::invalid_argument("Two matrices of substraction must be of same dimensions: "+
			    std::to_string(m.size())+"x"+
			    std::to_string(m.size())+" and "+
			    std::to_string(m2.m.size())+"x"+
			    std::to_string(m2.m.size())+
			    " were provided.");
  auto ret = new Matrix(m.size());
  auto i = 0;
  for(auto& l : m) {
    auto j = 0;
    for(auto c : l) {
      auto b = m2[i][j];
      ret->set(i,j,c-b);
      j++;
    }
    i++;
  }
  return ret;
}

bool Matrix::operator>(const Matrix& m) {
  auto c = *this - m;
  return c->isPositive() && !(!(*c));
}

bool Matrix::operator>=(const Matrix& m) {
  auto c = *this - m;
  auto res = c->isPositive();
  delete c;
  return res;
}

const std::vector<double>& Matrix::operator[](size_t i) const {
  if(i>=m.size())
    throw std::out_of_range("Try to access to unknown "+std::to_string(i)+"th element of Matrix.");
  return m[i];
}

double Matrix::min() const {
  double ret = std::numeric_limits<float>::max();
  for(auto i = 0u;i<m.size();i++) {
    for(auto j = std::begin(m[i])+i+1, end = std::end(m[i]);
	j<end;j++) {
      ret = (*j < ret)?*j:ret;
    }
  }
  return ret;
}

double Matrix::average() const {
  double avg = 0;
  for(auto& l : m) {
    for(auto& c : l) {
      avg += c;
    }
  }
  return avg/((m.size()*(m.size()-1))/2);
}

double Matrix::lineAverage(int line) const {
  double avg = 0;
  for(auto i=0;i<static_cast<int>(size());i++) {
    if(i < line) {
      avg += m[i][line];
    }
    else if (i > line) {
      avg += m[line][i];
    }
  }
  return avg/size();
}

std::string Matrix::to_string() const {
  std::string ret;
  for(auto& l : m) {
    ret += "[ ";
    for(auto& c : l) {
      ret += std::to_string(c)+" ";
    }
    ret += "]\n";
  }
  return ret;
}

std::string Matrix::to_csv_string() const {
  std::string ret;
  for(auto& l : m) {
    for(auto c = std::begin(l), end = std::end(l);c < end;c++) {
      ret += std::to_string(*c)+((c != end-1)?",":"");
    }
    ret += "\n";
  }
  return ret;
}

Logger& operator<<(Logger& log, const Matrix& m) {
  log<<m.to_string();
  return log;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
  os<<m.to_string();
  return os;
}

size_t Matrix::size() const {
  return m.size();
}
