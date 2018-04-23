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
 * \file Matrix.h
 * \brief Matrix class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 27/04/16
 *
 * Declaration of a square matrix class.
 *
 */

#pragma once
#include <vector>
#include <iostream>
#include "utils/Logger.h"

/**
 * \class Matrix
 * \brief Class that implement a square matrix.
 *
 * This class implement is used for store results of multi-dimensionnal
 * score.
 *
 * \author Florian Galinier
 */
class Matrix {
 private:
  std::vector<std::vector<double>> m;
 public:
  /**
   * Create a new square matrix of size t.
   * \param t The number of rows and columns.
   */
  Matrix(size_t t);
  /**
   * Destructor for Matrix class.
   */
  virtual ~Matrix() noexcept;
  /**
   * Return true if and only if the matrix is null.
   * \return true if matrix is null, false elsewhere.
   */
  virtual bool operator!() const;
  /**
   * Return true if the matrix is positive, i.e. if
   * all the matrix elements are greater or equal to 0.
   * \return true if matrix is positive.
   */
  virtual bool isPositive() const;
  /**
   * Substract two matrices and return the resulting matrix.
   * \param m2 The matrix to substract to current one.
   * \return A new pointer Matrix that is the result of substraction.
   */
  Matrix* operator-(const Matrix& m2);
  /**
   * Change element at position (i,j).
   * \param i The number of the row.
   * \param j The number of the column.
   * \param v The new value of Matrix at (i,j).
   */
  virtual void set(size_t i,size_t j,double v);
  /**
   * Compare two matrices and return true only if current matrix
   * is greater or equal than m2.
   * \param m2 The matrix to compare.
   * \return true only if current matrix is greater or equal than m2.
   */
  virtual bool operator>=(const Matrix& m2);
  /**
   * Compare two matrices and return true only if current matrix
   * is greater than m2.
   * \param m2 The matrix to compare.
   * \return true only if current matrix is greater than m2.
   */
  virtual bool operator>(const Matrix& m2);
  /**
   * Accessor for line of Matrix. Allow to access to matrix
   * element by using [][].
   * \param i The line to access.
   * \return The ith line.
   */
  const std::vector<double>& operator[](size_t i) const;
  /**
   * Return an average value of upper triangular matrix.
   * \return Average value of upper triangular matrix.
   */
  virtual double average() const;
  /**
   * Return the average of a line.
   * \param line The line where to compute average.
   * \return The average of the line.
   */
  virtual double lineAverage(int line) const;
  /** 
   * Return the minimum of upper triangular matrix.
   * \return Minimum of upper triangular matrix.
   */
  virtual double min() const;
  /**
   * Return a CSV string representation of matrix.
   * \return CSV string representation of matrix.
   */
  virtual std::string to_csv_string() const;
  /**
   * Return a string representation of matrix.
   * \return string representation of matrix.
   */
  virtual std::string to_string() const;
  /**
   * Return the size of matrix (height and width, both are equal).
   * \return The size of matrix.
   */
  size_t size() const;
};
/**
 * operator<< overload for iostream.
 * \param os The ostream where to write.
 * \param m The matrix to write.
 * \return The resulting ostream.
 */
std::ostream& operator<<(std::ostream& os, const Matrix& m);
/**
 * Specialization of Logger operator<< for Matrix
 * \param log The logger where to write
 * \param m The matrix to write
 * \return The used logger
 */
Logger& operator<<(Logger& log, const Matrix& m);
