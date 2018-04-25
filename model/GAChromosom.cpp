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

#include "GAChromosom.h"
#include <ga/garandom.h>
#include <limits>
#include <stdexcept>
#include <future>
#include <cstdio>

/* Constructors */

GAChromosom::GAChromosom(): Chromosom(), GAGenome(GAChromosom::init, GAChromosom::mutate, GAChromosom::compare), sc(3) {
  if(GAChromosom::crossover == GAChromosom::Cross::INTRA)
    GAGenome::crossover(onePointIntraCrossover);
  else GAGenome::crossover(onePointCrossover);
  GAGenome::evaluator(avgEvaluator);
}

GAChromosom::GAChromosom(const std::vector<std::string>& files): Chromosom(files), GAGenome(GAChromosom::init, GAChromosom::mutate, GAChromosom::compare), sc(3) {
  if(GAChromosom::crossover == GAChromosom::Cross::INTRA)
    GAGenome::crossover(onePointIntraCrossover);
  else GAGenome::crossover(onePointCrossover);
  GAGenome::evaluator(avgEvaluator);
}
  
GAChromosom::GAChromosom(const GAChromosom& c): sc(c.sc) {
  copy(c);
}

GAChromosom::~GAChromosom() {}

/* Copy methods */

GAGenome* GAChromosom::clone(CloneMethod) const {
  return new GAChromosom(*this);
}

void GAChromosom::copy(const GAGenome& c) {
  GAGenome::copy(c);
  GAChromosom& t = (GAChromosom&) c;
  this->models = t.models;
}

/* Accessors */

void GAChromosom::setModels(std::vector<Model> models) {
  Chromosom::setModels(models);
  this->_evaluated = gaFalse;
}

/* Stream methods */

std::string GAChromosom::to_string() const {
  std::string ret;
  for(auto& a : models) {
    ret += a.to_string()+"\n";
  }
  return ret;
}

int GAChromosom::write(std::ostream& os) const {
  os << to_string();
  return 0;
}

Logger& operator<<(Logger& log, const GAChromosom& c) {
  log << c.to_string();
  return log;
}

/* Genetic methods */

float GAChromosom::compare(const GAGenome& a, const GAGenome& b) {
  return 0;
}

void GAChromosom::init(GAGenome&) {}

int GAChromosom::onePointIntraCrossover(const GAGenome& dad, const GAGenome& mom, GAGenome* sis, GAGenome* bro) {
   GAChromosom& d = (GAChromosom&) dad;
  GAChromosom& m = (GAChromosom&) mom;
  auto sisModels = std::make_shared<std::vector<Model>>();
  auto broModels = std::make_shared<std::vector<Model>>();

  auto& dadModels = d.getModels();
  auto& momModels = m.getModels();
  auto modelsSize = dadModels[0].getVal()->size();
  
  size_t cutPoint =
    GARandomInt(1,Chromosom::getNbModels()*modelsSize-1);
  
  for(auto i = 0u;i<dadModels.size();i++) {
    std::cout.flush();
    auto dm = dadModels[i].getVal();
    auto mm = momModels[i].getVal();
    auto s = std::make_shared<Genes>();
    auto b = std::make_shared<Genes>();
    auto dd = dadModels[i].getDomains();
    auto md = momModels[i].getDomains();
    auto sd = std::make_shared<Domains>();
    auto bd = std::make_shared<Domains>();
    for(auto j = 0u;j<modelsSize;j++) {
      if((i*modelsSize + j) < cutPoint) {
	s->push_back({dm->at(j)});
	b->push_back({mm->at(j)});
	sd->push_back({dd->at(j)});
	bd->push_back({md->at(j)});
      }
      else {
	s->push_back({mm->at(j)});
	b->push_back({dm->at(j)});
	sd->push_back({md->at(j)});
	bd->push_back({dd->at(j)});
      }
    }
    Model sm(momModels[i]), bm(dadModels[i]);
    sm.setVal(s);
    bm.setVal(b);
    sm.setDomains(sd);
    bm.setDomains(bd);
    sisModels->push_back(sm);
    broModels->push_back(bm);
  }
  
  dynamic_cast<GAChromosom*>(sis)->setModels(*sisModels);
  dynamic_cast<GAChromosom*>(bro)->setModels(*broModels);
  
  return 2;
}

int GAChromosom::onePointCrossover(const GAGenome& dad, const GAGenome& mom, GAGenome* sis, GAGenome* bro) {
  GAChromosom& d = (GAChromosom&) dad;
  GAChromosom& m = (GAChromosom&) mom;
  auto sisModels = std::make_shared<std::vector<Model>>();
  auto broModels = std::make_shared<std::vector<Model>>();

  size_t cutPoint = GARandomInt(1,Chromosom::getNbModels()-1);

  auto& dadModels = d.getModels();
  auto& momModels = m.getModels();

  for(auto i = 0u;i<dadModels.size();i++) {
    if(i < cutPoint) {
       sisModels->push_back(Model(dadModels[i]));
       broModels->push_back(Model(momModels[i]));
    }
    else {
      sisModels->push_back(Model(momModels[i]));
      broModels->push_back(Model(dadModels[i]));
    }
  }
  
  dynamic_cast<GAChromosom*>(sis)->setModels(*sisModels);
  dynamic_cast<GAChromosom*>(bro)->setModels(*broModels);
  
  return 2;
}

std::vector<MatrixPtr> GAChromosom::evaluate(GABoolean flag) {
  auto sco = this->score();
  if(flag || (!sco[0] && !sco[1] && !sco[2])) {
    auto ev = GAChromosom::evaluate(*this);
    // this->_evaluated = gaTrue;
    return this->score(ev);
  }
  else
    return sco;
}

std::vector<MatrixPtr> GAChromosom::score(std::vector<MatrixPtr> m) {
  GAGenome::score((method & Method::COSINE)?m[0]->average():0 +
		  (method & Method::HAMMING)?m[1]->average():0 +
		  (method & Method::CENTRALITY)?m[2]->average():0 +
		  (method & Method::LEVENSHTEIN)?m[3]->average():0 +
		  (method & Method::LEVEXTERN)?m[4]->average():0);
  sc = m;
  return sc;
}

std::vector<MatrixPtr> GAChromosom::score() const {
  return sc;
}

std::vector<MatrixPtr> GAChromosom::evaluate(GAGenome& g) {
  GAChromosom& c = (GAChromosom&) g;
  MatrixPtr levenshtein = std::make_shared<Matrix>(nbModels),
    cosine = std::make_shared<Matrix>(nbModels),
    hamming = std::make_shared<Matrix>(nbModels),
    centrality = std::make_shared<Matrix>(nbModels),
    levextern = std::make_shared<Matrix>(nbModels);
  auto minDist = c.score();
  auto i = 0u;
  for(auto m1 = std::begin(c.models), end = std::end(c.models);m1 < end; m1++) {
    auto j = i + 1;
    for(auto m2 = m1 + 1; m2 < end; m2++) {
      if(*m1 != *m2) {
				if(GAChromosom::method & GAChromosom::Method::LEVENSHTEIN){
					levenshtein->set(i,j,Model::levenshteinDistance(*m1,*m2));
				}
				if(GAChromosom::method & GAChromosom::Method::COSINE){
					cosine->set(i,j,Model::cosineDistance(*m1,*m2));
				}
				if((GAChromosom::method & GAChromosom::Method::HAMMING) ||(GAChromosom::method & GAChromosom::Method::CENTRALITY) ||(GAChromosom::method & GAChromosom::Method::LEVEXTERN)) {
					auto res = Model::evaluateExtern(*m1,*m2);
					hamming->set(i,j,std::get<0>(res));
					centrality->set(i,j,std::get<1>(res));
					levextern->set(i,j,std::get<2>(res));
				}
				j++;
      }
    }
    i++;
  }
  return {cosine,hamming,centrality,levenshtein,levextern};
}

float GAChromosom::avgEvaluator(GAGenome& g) {
  auto res = evaluate(g);
  return (res[0]->average() + res[1]->average() + res[2]->average() + res[3]->average() + res[4]->average()) /
    ((GAChromosom::method & GAChromosom::Method::COSINE && 1) +
     (GAChromosom::method & GAChromosom::Method::HAMMING && 1) +
     (GAChromosom::method & GAChromosom::Method::CENTRALITY && 1) +
     (GAChromosom::method & GAChromosom::Method::LEVENSHTEIN && 1) +
     (GAChromosom::method & GAChromosom::Method::LEVEXTERN && 1));
}
  
int GAChromosom::mutate(GAGenome& g, float pmut) {
  GAChromosom& c = (GAChromosom&) g;
  return c.Chromosom::mutate(pmut);
}

int GAChromosom::method = GAChromosom::Method::COSINE;
GAChromosom::Cross GAChromosom::crossover = GAChromosom::Cross::INTER;
 
