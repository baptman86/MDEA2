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

/*
 * Generation of models that conform to a meta-model is one of the 
 * solutions from model driven engineering for models transformations 
 * testing or meta-models validation. Some combinatorial techniques 
 * have been explored in previous works, but few have focused on problem
 * of diversity. Amongst previous works, genetic algorithms usage seems
 * to be one of the most promising for diversity generation.
 * In this work, we experiment two approach, based on the evolutionnary
 * algorithm NSGA-II, to generate more diverse models. These approaches
 * provide results with an increased distance, enabling creation of more
 * realistic and complete benchmark.
 *
 * -------------
 *
 * If you have any questions, or if you have implementations issues,
 * please be free to contact me via gitlab or join me by email at
 * galinierflo@gmail.com
 *
 */

#include <ga/ga.h>
#include <ga/std_stream.h>
#include <iostream>
#include <sstream>
#include "model/GAChromosom.h"
#include "model/NSGAII.h"
#include "model/Population.h"
#include "model/Model.h"
#include "utils/CommandLine.h"
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main2(int argc, char* argv[]) {
	Model m("tmp0");
	m.generateDotFile(1);
	return 0;
}

int main(int argc, char* argv[]) {
  /* Configure CLI */
  CommandLine cl("mdea");
  cl.addOption("-in","-in <models directory>");
  cl.addOption("-out","-out <output directory>",false);
  cl.addOption("-nb","-nb <number of model in a single chromosom>",false);
  //  cl.addOption("-cfg","-cfg <config file>",false);
  // TODO: Add possibility to create a config file for inputs  
  cl.addOption("-m","-m <percentage of mutation chance>",false);
  cl.addOption("-dist","-dist <cosine|levenshtein|smartLevenshtein|hamming|centrality>",false);
  cl.addOption("-g","-g <number of generations>",false);
  cl.addOption("-cx","-cx <inter|intra>",false);
  cl.addOption("-fitness","-fitness <min|avg|minavg|minavgs|dist>",false);
  cl.addOption("-freq","-freq <dot files generation frequency in addition to the last generation, 0 = only last generation>, -1 = no generation",false);

  /* Parse CLI input */
  auto opt = cl.parse(argc,argv);

  auto pm = 1u; //< Multiplier of population
  // (On new generation, P -> P + pm*P)
  
  if(opt->at("-nb") == "") // Number of models in an individual
    Chromosom::setNbModels(2);
  else
    Chromosom::setNbModels(std::stof(opt->at("-nb")));

  // In the case m = 1, crossover are necessary intra.
  if(Chromosom::getNbModels() == 1) {
    GAChromosom::crossover = GAChromosom::Cross::INTRA;
    pm = 1; // TODO: Do not hardcode this value.
  }
  

  
  if(opt->at("-cx") == "intra") // Are the crossover intra?
    GAChromosom::crossover = GAChromosom::Cross::INTRA;

  // Choice of fitness (default is each distance is a fitness)
  if(opt->at("-fitness") == "min") 
    NSGAII::fitness = NSGAII::Fitness::MIN;
  if(opt->at("-fitness") == "avg")
    NSGAII::fitness = NSGAII::Fitness::AVG;
  if(opt->at("-fitness") == "minavgs")
    NSGAII::fitness = NSGAII::Fitness::MINAVG;
  if(opt->at("-fitness") == "minavg")
    NSGAII::fitness = NSGAII::Fitness::AVG | NSGAII::Fitness::MIN;

  // Mutation percentage chance
  auto mut = 0.f;
  if(opt->at("-m") != "")
    mut = std::stof(opt->at("-m"));
    


  // Number of generations
  NSGAII::maxGen = 100;
  if(opt->at("-g") != "")
    NSGAII::maxGen = std::stof(opt->at("-g"));
  
  // Choice of distance to use
  auto method = opt->at("-dist");
  if(method != "") {
    std::stringstream split(method);
    std::string s;
    GAChromosom::method = 0;
    while(std::getline(split,s,'+')) {
      if(s == "hamming")
	GAChromosom::method |= GAChromosom::Method::HAMMING;
      if(s == "centrality")
	GAChromosom::method |= GAChromosom::Method::CENTRALITY;
      if(s == "levenshtein")
	GAChromosom::method |= GAChromosom::Method::LEVENSHTEIN;
      if(s == "smartLevenshtein")
	GAChromosom::method |= GAChromosom::Method::LEVEXTERN;
      if(s == "cosine")
	GAChromosom::method |= GAChromosom::Method::COSINE;
    }
  }
  else{
    cout << "pas de methode choisie" << endl;
    exit(0);
  }
  
  
  
  // Input population
  auto pop = Population(opt->at("-in"));
  

  //output directory
  if (opt->at("-out") != "")
    NSGAII::dir = opt->at("-out");
  else {
    std::ostringstream dir;
    dir << "output-"+opt->at("-in") << "-" << opt->at("-cx") << "-" << opt->at("-dist") << "-" << opt->at("-fitness") << "-" << opt->at("-g") << "-" << opt->at("-m") << "-" << opt->at("-nb");
    NSGAII::dir = dir.str();
  }
  
  
  system(("rm -rf "+NSGAII::dir).c_str());
  mkdir(NSGAII::dir.c_str(),0777);
  mkdir((NSGAII::dir+"/jvmconsuption").c_str(),0777);
  mkdir((NSGAII::dir+"/output").c_str(),0777);
  
  std::ostringstream oss;
  oss << NSGAII::dir << "/stat";
  Statistics::outfile = oss.str();

  // Evaluation of initial population
  if(GAChromosom::getNbModels() != 1)
    pop.evaluate();
  else
    pop.popEvaluate();
    
  
  // Initialize algorithm
  GAChromosom genome;
  GAParameterList params;
  NSGAII ga(pop,pm);
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNflushFrequency, 10);
  params.set(gaNnGenerations, NSGAII::maxGen);
  params.set(gaNpConvergence,0.99);
  params.set(gaNnConvergence,100);
  params.set(gaNpMutation,mut);
  params.set(gaNpCrossover,1);
  params.set(gaNscoreFrequency,1);
  params.set(gaNselectScores,GAStatistics::Maximum|GAStatistics::Minimum|GAStatistics::Mean);
  params.set(gaNpopulationSize,pop.size());
  params.set(gaNscoreFilename,Statistics::outfile.c_str());
  ga.parameters(params);

  // Set selector type (NSGA-II algorithm uses tournament selector)
  ga.selector(GATournamentSelector());
  
  // ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);
  // ^ Uncomment this line to terminate upon convergence.

	std::cout << ga << std::endl;
  // do the evolution
  ga.evolve();
  
  
  
  //generate all dot files
  
  int frequency = -1;
  if(opt->at("-freq") != ""){
  	frequency = std::stoi(opt->at("-freq"));
  }
  
  if(frequency>0){
		for(int i =1; i<=NSGAII::maxGen; i++){
			if((i % (std::stoi(opt->at("-freq")))==0) || NSGAII::gen==NSGAII::maxGen){
				ga.generateDotGen(opt->at("-in"),NSGAII::dir+"/output/gen"+std::to_string(i),NSGAII::dir+"/output/dotgen"+std::to_string(i));
			}
		}
  }
  if(frequency==0){
  	ga.generateDotGen(opt->at("-in"),NSGAII::dir+"/output/gen"+std::to_string(NSGAII::maxGen),NSGAII::dir+"/output/dotgen"+std::to_string(NSGAII::maxGen));
  }
  

  // print out the results
  cout << ga.statistics() << endl;

  // Print statistics in the outfile
  std::string outfile = Statistics::outfile;
  Logger l(outfile);
  l << ga.extraStatistics();
  
  delete opt;
  return 0;
}
