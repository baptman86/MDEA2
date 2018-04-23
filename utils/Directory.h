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
 * \file Directory.h
 * \brief Directory class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 06/04/16
 *
 * Utils function for directory manipulation.
 *
 */

#pragma once
#include <dirent.h>
#include <vector>
#include <string>
#include <memory>

/**
 * \class Directory
 * \brief This class provide methods for directory manipulation.
 *
 * This class provide methods for directory manipulation.
 *
 * \author Florian Galinier
 */
class Directory {
 private:
  DIR* path;
 public:
  /**
   * Default constructor with directory path.
   * \param path The directory path
   */
  Directory(std::string path);
  /**
   * Directory destructor
   */
  virtual ~Directory();
  /**
   * Return the number of files in the directory.
   * \param ext Optional file extension.
   * \return The number of files in directory.
   */
  virtual size_t getNbFiles(std::string ext = "") const;
  /**
   * Return a vector of all files in directory.
   * \param ext Optional file extension.
   * \return A vector with files path.
   */
  virtual std::shared_ptr<std::vector<std::string>> getFiles(std::string ext = "") const;
};
