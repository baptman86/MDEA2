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
#include "NSGAII.h"
#include "GAChromosom.h"
#include <ga/garandom.h>
#include <algorithm>
#include <limits>
#include <chrono>
#include <sys/stat.h>
#include <fstream>

int NSGAII::fitness = NSGAII::Fitness::AVG;
std::string NSGAII::dir = "default";
unsigned int NSGAII::gen = 0;
unsigned int NSGAII::maxGen = 100;

NSGAII::NSGAII(const GAPopulation& p, unsigned int pm): GASimpleGA(p), extraStats(), popMult(pm) {
  assert(pm > 0);
}

NSGAII::~NSGAII() {}

bool NSGAII::dominate(MatrixPtr m1, MatrixPtr m2) {
  if(!*m1 && !*m2)
    return true;
  MatrixPtr sc1, sc2;
  // Case where the fitness is not dist (not 0b0000)
  if(NSGAII::fitness) {
    sc1 = std::make_shared<Matrix>(2);
    sc2 = std::make_shared<Matrix>(2);
    // Case where fitness is the sum of min and average
    if(NSGAII::fitness & Fitness::MINAVG) {
      sc1->set(0,1,m1->average() + m1->min());
      sc2->set(0,1,m2->average() + m2->min());
    }
    else {
      // If average
      if(NSGAII::fitness & Fitness::AVG) {
	sc1->set(0,1,m1->average());
	sc2->set(0,1,m2->average());
      }
      // If min
      if(NSGAII::fitness & Fitness::MIN) {
	sc1->set(0,0,m1->min());
	sc2->set(0,0,m2->min());
      }
    }
  }
  else {
    // If no fitness, the distances matrices are used
    // as fitness matrices.
    sc1 = m1;
    sc2 = m2;
  }
  auto dom = false;
  // Use the previous matrices as a fitness matrices
  for(auto i=0u;i<sc1->size();i++) {
    auto m1Row = (*sc1)[i];
    auto m2Row = (*sc2)[i];
    for(auto j=0u;j<m1Row.size();j++) {
      if(m1Row[j] < m2Row[j])
	return false;
      if(m1Row[j] > m2Row[j])
	dom = true;
    }
  }
  return dom;
}

std::shared_ptr<std::vector<std::vector<int>>>
NSGAII::fastNonDominatedSort() {
  // This method is a simple implementation of NSGA-II
  // fastNonDominatedSort. For more information about
  // how it works, please refer to Deb et al. 2002.
  std::vector<std::vector<int>> s; 
  std::vector<int> n; 
  std::map<int,int> r; 
  auto f = std::make_shared<std::vector<std::vector<int>>>();
  f->insert(f->begin(),std::vector<int>());

  for(auto i = 0;i < pop->size();i++) {
    s.insert(s.begin()+i,std::vector<int>());
    n.insert(n.begin()+i,0);
    GAChromosom& p = static_cast<GAChromosom&>(pop->individual(i));

    for(auto j = 0;j < pop->size();j++) {
      if(j != i) {
	GAChromosom& q = static_cast<GAChromosom&>(pop->individual(j));
	auto pscore = p.score();
	auto qscore = q.score();
	if(dominate(pscore[0],qscore[0]) &&
	   dominate(pscore[1],qscore[1]) &&
	   dominate(pscore[2],qscore[2]))
	  s[i].push_back(j);
	else if(dominate(qscore[0],pscore[0]) &&
	   dominate(qscore[1],pscore[1]) &&
	   dominate(qscore[2],pscore[2]))
	  n[i]++;
      }
    }
    if(n[i] == 0) {
      r.insert({i,0});
      f->at(0).push_back(i);
    }
  }

  auto i = 0;
  while(f->at(i).size()) {
    std::vector<int> t;
    for(size_t j = 0;j < f->at(i).size();j++) {
      auto p = f->at(i)[j];
      for(size_t k = 0;k < s[p].size();k++) {
	auto q = s[p][k];
	n[q]--;
	if(n[q] == 0) {
	  r[q] = i + 1;
	  t.push_back(q);
	}
      }
    }
    i++;
    f->insert(f->begin()+i,t);
  }

  return f;
}

std::map<size_t,double> NSGAII::crowdingDistanceAssignment(std::vector<int>& f) {
  std::map<size_t,double> r;
  for(size_t i = 0;i<f.size();i++) {
    r.insert({f[i],0});
  }

  // For each distances
  for(auto d = 0u; d < 3; d++) {
    for(auto l = 0u; l < Chromosom::getNbModels()-1; l++) {
      for(auto c = l + 1; c < Chromosom::getNbModels();c++) {
	// Order individuals by the fitness
	std::sort(f.begin(),
		  f.end(),
		  [this,l,c,d](auto& i,auto& j){
		    return
		      (*static_cast<GAChromosom&>(pop->individual(i))
		       .score()[d])[l][c] <
			 (*static_cast<GAChromosom&>(pop->individual(j))
			  .score()[d])[l][c];
		  });

	// Extremum are kept
	r[f[0]] = std::numeric_limits<double>::max();
	r[f[f.size()-1]] = std::numeric_limits<double>::max();

	// Give a crowding distance according to distance to
	// other individuals.
	for(size_t i = 1;i<(f.size()-1);i++) {
	  if(((*static_cast<::Population*>(pop)
	       ->best(l,c).score()[d])[l][c] -
	      (*static_cast<::Population*>(pop)
	       ->worst(l,c).score()[d])[l][c]))
	    r[f[i]] += (((*static_cast
			  <GAChromosom&>(pop->individual(f[i+1]))
			  .score()[d])[l][c] -
			 (*static_cast
			  <GAChromosom&>(pop->individual(f[i-1]))
			  .score()[d])[l][c]) /
			((*static_cast<::Population*>(pop)
			  ->best(l,c).score()[d])[l][c] -
			 (*static_cast<::Population*>(pop)
			  ->worst(l,c).score()[d])[l][c]));
	}
 
      }
    }
  }
  
  return r;
}

void NSGAII::clustering(::Population* popr,
			::Population* p,
			unsigned int popSize,
			MatrixPtr res) {
  std::vector<int> centers;
  // Random select centers
  while (centers.size() < popSize) {
    int c = GARandomInt(0,res->size()-1);
    if(std::find(centers.begin(), centers.end(), c) == centers.end())
      centers.push_back(c);
  }
    
  std::vector<int> clusters(res->size());

  for(auto v = 0; v < 200; v++) {
    // Assign centers to clusters
    for(auto i = 0u;i < centers.size();i++) {
      clusters[centers[i]] = i;
    }
    
    // For each other points, assign to cluster of nearest center
    for(auto l = 0u; l < res->size(); l++) {
      if(std::find(centers.begin(), centers.end(), l) ==
	 centers.end()) { // If individual is not a center
	auto& line = (*res)[l];
	auto min = std::numeric_limits<double>::max();
	auto minIndex = -1;
	// For each centers, find the nearest.
	for(auto i = 0u; i < line.size(); i++) {
	  if(std::find(centers.begin(), centers.end(), i) !=
	     centers.end() && line[i] < min) {
	    min = line[i];
	    minIndex = i;
	  }
	}
	clusters[l] = clusters[minIndex];
      }
    }

    // Assign new centers
    std::vector<double> max(res->size());
    for(auto l = 0u; l < res->size(); l++) {
      auto& line = (*res)[l];
      max[l] = 0;
      for(auto i = 0u; i < line.size(); i++) {
	if(clusters[i] != clusters[l]) {
	  max[l] += line[i];
	}
      }
    }

    for(auto i = 0u; i < max.size(); i++) {
      if(max[i] > max[centers[clusters[i]]]) {
	centers[clusters[i]] = i;
      }
    }
  }

  std::cout<<".";
  std::cout.flush();
    
  for (auto i=0u; i < centers.size(); i++) {
    p->add(popr->individual(centers[i]));
  }
}

void NSGAII::step() {
  // Initialization of the generation
  gen++;
  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout<<gen;
  std::cout.flush();
  auto p = new ::Population();
  auto popr = static_cast<::Population*>(pop);
  unsigned int popSize = pop->size();
  auto mutLost = 0;
  auto nbMut = 0;
  auto lost = 0;
  auto tMin = std::numeric_limits<double>::max();
  auto tMax = 0.;
  auto tMoy = 0.;

  // Until offspring population is not filled
  while(static_cast<unsigned int>(p->size()) < popMult*popSize) {
    // Choose a mom and a dad
    auto& mom = popr->select();
    auto& dad = popr->select();
      
    while(dad.compare(mom)) {	
      dad = popr->select();
    }

    // Crossover to obtain childs
    GAGenome* sis = new GAChromosom;
    GAGenome* bro = new GAChromosom;
    stats.numcro += (*scross)(dad, mom, sis, bro);
    if(GAChromosom::crossover != GAChromosom::Cross::INTRA) {
      p->add(sis);
      p->add(bro);
    }
    else {
      // Crossover is intra, we need to check if generated models
      // are valid models.
      auto tm = std::chrono::high_resolution_clock::now();
      auto sisValid = static_cast<GAChromosom*>(sis)->isValid();
      auto broValid = static_cast<GAChromosom*>(bro)->isValid();
      auto tmend = std::chrono::high_resolution_clock::now();
      auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(tmend-tm).count();
      tMin = (dur < tMin)?dur:tMin;
      tMax = (dur > tMax)?dur:tMax;
      tMoy += dur;
      if(sisValid)
	p->add(sis);
      else
	lost++;
      if(broValid)
	p->add(bro);
      else
	lost++;
    }
  }

  std::cout<<".";
  std::cout.flush();
  
  auto i = 0;
  for(auto i=0;i<p->size();) {
    // Apply mutations to offspring population
    auto ind = new GAChromosom(*dynamic_cast<GAChromosom*>(&p->individual(i)));
    auto mut = dynamic_cast<GAGenome*>(ind)->mutate(pMutation());
    if(mut) {
      nbMut++;
      auto tm = std::chrono::high_resolution_clock::now();
      auto valid = static_cast<GAChromosom&>(*ind).isValid(); 
      auto tmend = std::chrono::high_resolution_clock::now();
      auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(tmend-tm).count();
      tMin = (dur < tMin)?dur:tMin;
      tMax = (dur > tMax)?dur:tMax;
      tMoy += dur;
      if(valid) {
				i++;
				stats.nummut += mut;
				popr->add(ind);
      }
      else {
				mutLost++;
      }
    }
    else {
      popr->add(ind);
      i++;
    }
  }
  
  std::cout<<".";
  std::cout.flush();

  // Evaluation of population
  auto tm = std::chrono::high_resolution_clock::now();
  MatrixPtr res;
  if(GAChromosom::getNbModels() == 1)
    res = popr->popEvaluate();
  else
    popr->evaluate();
  auto tmend = std::chrono::high_resolution_clock::now();
  auto dur = std::chrono::
    duration_cast<std::chrono::milliseconds>(tmend-tm).count();
  tMin = (dur < tMin)?dur:tMin;
  tMax = (dur > tMax)?dur:tMax;
  tMoy += dur;

  delete p;
  p = new ::Population();
  // Case m = 1, we have to use clustering
  if(GAChromosom::getNbModels() == 1) {
    clustering(popr,p,popSize,res);
  }
  else {
    // Case m > 1, we apply classical NSGA-II algorithm
    auto f = fastNonDominatedSort();
    i = 0;
    while(p->size()+f->at(i).size()
	  <= static_cast<unsigned int>(popSize)) {
      for(size_t j = 0;j<f->at(i).size();j++) {
	p->add(popr->individual(f->at(i)[j]));
      }
      i++;
    }

    std::cout<<".";
    std::cout.flush();
  
    std::map<size_t,double> t = crowdingDistanceAssignment(f->at(i));
    std::sort(f->at(i).begin(),f->at(i).end(),
	      [this,&t](auto& i,auto& j) {
		return t[i] > t[j];
	      });
  
    auto targ = popSize-p->size();
    for(auto j = 0u;j < targ;j++) {
      p->add(popr->individual(f->at(i)[j]));
    }
  }
  
  std::cout<<".";
  std::cout.flush();

  population(*p);
  stats.update(*p);
  extraStats.update(*p);
  
  popr = static_cast<::Population*>(p);
  // Evaluation for statistics
  tm = std::chrono::high_resolution_clock::now();
  if(GAChromosom::getNbModels() == 1)
    res = popr->popEvaluate();
  else
    popr->evaluate();
  tmend = std::chrono::high_resolution_clock::now();
  dur = std::chrono::
    duration_cast<std::chrono::milliseconds>(tmend-tm).count();
  tMin = (dur < tMin)?dur:tMin;
  tMax = (dur > tMax)?dur:tMax;
  tMoy += dur;

	

  // Output population
  std::string outfile = NSGAII::dir+std::string("/output/gen")+std::to_string(gen);
	
  Logger l(outfile);
  if(GAChromosom::getNbModels() == 1){
    l << *res;
	}
	l << *popr;

  // Output mutation status
  auto t2 = std::chrono::high_resolution_clock::now();  
  outfile = NSGAII::dir+std::string("/output/mutstate");
  Logger l2(outfile,std::ios_base::app);
  l2<<gen<<" "<<lost<<" "<<popMult*popSize<<" "<<
    nbMut<<" "<<mutLost<<" "<<
    std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
    <<" "<<tMin<<" "<<tMax<<" "<<tMoy<<" "<<tMoy/nbMut<<"\n";
  
  delete p;
  std::cout<<"."<<std::endl;

  // Each 5 generations, output statistics
  // TODO: Add possibility to change record frequence
  if(!(gen % 5)) {
    std::string outfile = Statistics::outfile;
    Logger l(outfile);
    l << extraStats;
  }
}


void NSGAII::generateDotGen(std::string metaModelDir, std::string genFile, std::string outputDir){
	  std::ifstream file(genFile);
	  if(file){
	      std::string line;
	      int vectorNumber=0;

	      while(getline(file, line)){
	          if(isdigit(line[0])){
								mkdir(outputDir.c_str(),0777);
	          		std::string outputPath = outputDir+"/c"+std::to_string(vectorNumber).c_str()+".chr";
	              std::ofstream output(outputPath);
	              output << line << std::endl;
								std::string metaModel = metaModelDir+"/c"+std::to_string(vectorNumber)+".xml";
								std::string grimm = "";
								std::string ecore = "";
								std::string dir = "";
								if(metaModelDir == "scaffold"){
									grimm = "Graph.grimm";
									ecore = "ScaffoldGraph.ecore";
									dir = "Graph";
								}
								if(metaModelDir == "javasmall"){
										grimm = "ProjectSmall.grimm";
										ecore = "MyJava.ecore";
										dir = "Project";
								}
								if(grimm == ""){
									std::cout << "meta-model need to be scaffold or javasmall" << std::endl;
								}
								else{
									output << metaModel << std::endl;
									output << grimm << std::endl;
									output << ecore << std::endl;
									output << dir << std::endl;
			            output.close();
									Model m(outputPath);
									
									std::string dotFilePath = m.generateDotFileScaffold(outputDir+"/c"+std::to_string(vectorNumber).c_str()+".dot");
									//std::string dotFilePath = m.generateDotFile(0);
									//std::string rawfileName = dotFilePath.substr(dotFilePath.find("/"),dotFilePath.length());
									//system(("mv "+dotFilePath+" "+outputDir).c_str());
									
									//std::string s0 = outputDir+"/"+rawfileName;
									//std::string s1 = outputDir+"/c"+std::to_string(vectorNumber).c_str()+".dot";
									
									//rename(s0.c_str(),s1.c_str());
									vectorNumber++;
								}
	          }
	      }
	  } else {
	      std::cout << "Could not open the target file" << std::endl;
	  }
	  file.close();
}

NSGAII& NSGAII::operator++() {
  NSGAII::step();
  return *this;
}

Statistics& NSGAII::extraStatistics() {
  return extraStats;
}
