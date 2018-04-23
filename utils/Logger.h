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
 * \file Logger.h
 * \brief Logger class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 29/04/16
 *
 * A logger to write on a file.
 *
 */

#pragma once
#include <string>
#include <fstream>
#include <ios>

/**
 * \class Logger
 * \brief Class that allow to log information in a file.
 *
 * This class provide an interface to write informations in a file,
 * like population state or rejected mutation rate.
 *
 * \author Florian Galinier
 */
class Logger {
  std::ofstream file;
 public:
  /**
   * Create a new log file
   * \param fileName name of the file to create
   * \param mode The mode to use to open
   */
  Logger(std::string fileName,
	 std::ios_base::openmode mode = std::ios_base::out);
  /**
   * Destructor of Logger
   */
  virtual ~Logger() noexcept;
  /**
   * Write the given string in the log file
   * \param s The string to write
   */
  virtual void write(std::string s);
  /**
   * Overloaded operator to use log as a stream
   * \param t the object to write
   * \return A reference to current Logger
   */
  template <typename T>
    Logger& operator<<(const T& t) {
      write(std::to_string(t));
      return *this;
  }
  /**
   * Specialization of operator<<
   * \param s the string to write
   * \return A reference to current Logger
   */
  virtual Logger& operator<<(const std::string& s);
  /**
   * Specialization of operator<<
   * \param c the string to write
   * \return A reference to current Logger
   */
  virtual Logger& operator<<(const char* c);
};

