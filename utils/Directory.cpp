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

#include "Directory.h"
#include <iostream>

Directory::Directory(std::string path): path(opendir(path.c_str())) {}

Directory::~Directory() {
  closedir(path);
}

size_t Directory::getNbFiles(std::string ext) const {
  dirent *f;
  size_t nb = 0;
  if (path != NULL) {
    seekdir(path,0);
    while ((f = readdir(path)) != NULL) {
      std::string tmp = f->d_name;
      bool extCheck = (tmp.find(ext) == (tmp.size() - ext.size()));
      if(tmp != "." && tmp != ".."
	 && extCheck)
	nb++;
    }
  }
  return nb;
}

std::shared_ptr<std::vector<std::string>> Directory::getFiles(std::string ext) const {
  dirent *f;
  auto ret = std::make_shared<std::vector<std::string>>();
  if (path != NULL) {
    seekdir(path,0);
    while ((f = readdir(path)) != NULL) {
      std::string tmp = f->d_name; 
      bool extCheck = (tmp.find(ext) == (tmp.size() - ext.size()));
      if(tmp != "." && tmp != ".."
	 && extCheck)
	ret->push_back(f->d_name);
    }
  }
  
  return ret;
}
