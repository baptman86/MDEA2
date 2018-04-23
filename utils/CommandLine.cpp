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

#include "CommandLine.h"
#include <iostream>

CommandLine::CommandLine(std::string app): name(app), required(), options() {}

void CommandLine::addOption(std::string option, std::string help,
			    bool required) {
  addOption({option,help},required);
}

void CommandLine::addOption(std::pair<std::string,std::string> option,
			    bool required) {
  if(!required)
    this->options.insert(option);
  else
    this->required.insert(option);
}

void CommandLine::printUsage() const {
  std::cout<<"Usage:"<<std::endl;
  std::cout<<"  "<<name<<" ";
  std::string beginSpace;
  for(size_t i=0;i<name.size();i++) {
    beginSpace += " ";
  }

  bool first = true;
  for(auto p : required) {
    if(!first)
      std::cout<<"   "<<beginSpace;
    else
      first = !first;
    std::cout<<std::get<1>(p)<<std::endl;
  }
  for(auto p : options) {
    std::cout<<"   "<<beginSpace<<"["<<std::get<1>(p)<<"]"<<std::endl;
  }
  exit(0);
}

std::map<std::string,std::string>*
CommandLine::parse(int argc, char** argv) const {
  std::map<std::string,std::string>* ret =
    new std::map<std::string,std::string>();
  for(auto i=1;i<argc;) {
    try {
      required.at(argv[i]);
      if(i+1 >= argc) {
	std::cout<<"Missing parameter for "<<argv[i]<<" option."<<std::endl;
	delete ret;
	printUsage();
      }
      ret->insert({argv[i],argv[i+1]});
      i += 2;
      continue;
    }
    catch(std::out_of_range& e) {
      try {
	options.at(argv[i]);
	if(i+1 >= argc) {
	  std::cout<<"Missing parameter for "<<argv[i]<<" option."<<std::endl;
	  delete ret;
	  printUsage();
	}
	ret->insert({argv[i],argv[i+1]});
	i += 2;
	continue;
      }
      catch(std::out_of_range&) {
	std::cout<<"Unknow option: "<<argv[i]<<std::endl;
	delete ret;
	printUsage();
      } 
    }
  }

  for(auto p : required) {
    try {
      ret->at(std::get<0>(p));
    }
    catch(std::out_of_range&) {
      std::cout<<"Missing required parameter: "<<std::get<0>(p)<<std::endl;
      delete ret;
      printUsage();
    }
  }

  for(auto p : options) {
    try {
      ret->at(std::get<0>(p));
    }
    catch(std::out_of_range&) {
      ret->insert({std::get<0>(p),""});
    }
  }
  
  return ret;
}
