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
 * \file CommandLine.h
 * \brief CommandLine class header.
 * \author Florian Galinier
 * \version 0.1
 * \date 12/04/16
 *
 * Set of functions for command line options.
 *
 */

#pragma once
#include <map>
#include <string>

/**
 * \class CommandLine
 * \brief Class for command line options parsing.
 *
 * This class allow usage of command line interface by parsing options.
 *
 * \author Florian Galinier
 */
class CommandLine {
 private:
  std::string name;
  std::map<std::string,std::string> required;
  std::map<std::string,std::string> options;
 public:
  /**
   * Constructor for new CommandLine parser.
   * \param app The application name.
   */
  CommandLine(std::string app);
  /**
   * Add an option to parse.
   * \param option The option value.
   * \param help The help to display with this option.
   * \param required Specify if option is optional or required (true for required and by default)
   */
  void addOption(std::string option, std::string help,
		 bool required = true);
  /**
   * Add option method (see addOption(std::string,std::string,bool)) 
   * with std::pair. 
   * \param option std::pair composed of option value and help to display.
   * \param required Specify if option is optional or required (true for required and by default)
   */
  void addOption(std::pair<std::string,std::string> option,
		 bool required = true);
  /**
   * Print usage method. This method will exit program.
   */
  void printUsage() const;
  /**
   * Parse command line options.
   * \param argc The main argc argument.
   * \param argv The main argv argument.
   * \return Options passed in command.
   */
  std::map<std::string,std::string>* parse(int argc, char** argv) const;
};
