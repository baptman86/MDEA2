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

#include "Logger.h"

Logger::Logger(std::string fileName, std::ios_base::openmode mode):
  file(fileName,mode)  {}

Logger::~Logger() {
  file.close();
}

void Logger::write(std::string s) {
  file << s;
}

Logger& Logger::operator<<(const std::string& s) {
  write(s);
  return *this;
}

Logger& Logger::operator<<(const char* c) {
  write({c});
  return *this;
}
