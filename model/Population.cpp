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

#include "Population.h"
#include "utils/Directory.h"
#include <dirent.h>
#include <limits>

Population::Population() {}

Population::Population(std::string dirPath): Population() {
  Directory d(dirPath);

  int nbt = d.getNbFiles(".chr");

  //doesnt use all model if (nbt % Chromosom::getNbModels() != 0)
  int nbFilesUsed = nbt - (nbt % Chromosom::getNbModels());
  
  //force use of all model
  /*if(d.getNbFiles(".chr") % Chromosom::getNbModels() != 0) {
    std::cerr<<"Error: Number of models must be a multiple of "<<Chromosom::getNbModels()<<"."<<std::endl;
    exit(1);
  }*/
  /* TODO: Throw an exception instead of exit. */
  
  auto t = d.getFiles(".chr");
  
  int i=0;
  while(i<nbFilesUsed){
    std::vector<std::string> tmp;
    for(size_t j=0;j<Chromosom::getNbModels();j++) {
      tmp.push_back(dirPath+"/"+(*t)[i+j]);
    }
    this->newChromosom(tmp);
    i += Chromosom::getNbModels();
  }
}

inline void Population::newChromosom(const std::vector<std::string>& files) {
  this->add(GAChromosom(files));
}

void Population::evaluate(GABoolean flag) const {
  for(auto i=0;i<this->size();i++) {
    auto& chr = static_cast<GAChromosom&>(this->individual(i));
    chr.evaluate(flag);
  }
}

MatrixPtr Population::popEvaluate(GABoolean flag) {
  auto score = std::make_shared<Matrix>(this->size());
  for(auto i=0;i<this->size();i++) {
    auto& chr = static_cast<GAChromosom&>(this->individual(i));
    for(auto j=i+1;j<this->size();j++) {
      auto& chr2 = static_cast<GAChromosom&>(this->individual(j));
      if(GAChromosom::method & GAChromosom::Method::COSINE) {
				auto dist = Model::evaluate(chr.getModels()[0],chr2.getModels()[0]);
				score->set(i,j,dist);
				score->set(j,i,dist);
      }
      else {
				auto dist = Model::evaluateExtern(chr.getModels()[0],chr2.getModels()[0]);
				if(GAChromosom::method & GAChromosom::Method::CENTRALITY) {
					score->set(i,j,std::get<1>(dist));
					score->set(j,i,std::get<1>(dist));
				}
				else {
					score->set(i,j,std::get<0>(dist));
					score->set(j,i,std::get<0>(dist));
				}
      }
    }
    static_cast<GAGenome>(chr).score(score->lineAverage(i));
  }
	Logger l("test");
	l << *score;
  return score;
}

GAChromosom& Population::best(size_t r,size_t c) {
  auto score = std::numeric_limits<double>::min(); 
  auto ind = 0;
  for(auto i=0;i<size();i++) {
    auto res = dynamic_cast<GAChromosom&>(individual(i)).score();
    auto sc = (*res[0])[r][c]+(*res[1])[r][c]+(*res[2])[r][c];
    if (sc > score) {
      score = sc;
      ind = i;
    }
  }
  return dynamic_cast<GAChromosom&>(individual(ind));
}

GAChromosom& Population::worst(size_t r,size_t c) {
  auto score = std::numeric_limits<double>::max(); 
  auto ind = 0;
  for(auto i=0;i<size();i++) {
    auto res = dynamic_cast<GAChromosom&>(individual(i)).score();
    auto sc = (*res[0])[r][c]+(*res[1])[r][c]+(*res[2])[r][c];
    if (sc < score) {
      score = sc;
      ind = i;
    }
  }
  return dynamic_cast<GAChromosom&>(individual(ind));
}

Logger& operator<<(Logger& log, const Population& p) {
  for(auto i = 0; i < p.size(); i++) {
    auto& ind = static_cast<GAChromosom&>(p.individual(i));
    log<<"--------------\n";
    log<<ind<<"\n";
    auto res = ind.score();
    log<<*res[0]<<"\n";
    log<<*res[1]<<"\n";
    log<<*res[2]<<"\n";
  }
  return log;
}
