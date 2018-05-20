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

#include "Model.h"
#include "GAChromosom.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <fcntl.h>
#include <future>
#include <cerrno>
#include <cstring>
#include "NSGAII.h"

/* Constructors */

Model::Model(): genes(std::make_shared<Genes>()),
		domains(std::make_shared<Domains>()),
		change(true) {}

Model::Model(const Model& m): genes(std::make_shared<Genes>()),
			      domains(std::make_shared<Domains>()),
			      xcsp(m.xcsp), mm(m.mm), root(m.root),
			      grimm(m.grimm), dot(""),
			      change(true) {
 
  for(auto i=0u; i<m.genes->size(); i++) {
    genes->push_back(m.genes->at(i));
    domains->push_back(m.domains->at(i));
  }
}


Model::Model(std::string genesFile): Model() {
  std::ifstream infile(genesFile);

  // Read first line of file (genes)
  {
    std::string line;
    std::getline(infile, line);
    std::istringstream iss(line);
    int a;
    while(iss >> a) {
      genes->push_back(a);
    }
  }

  // Read metadata from other lines of file (xcsp file path, grimm file)
  {
    std::vector<std::string> tmp;
    std::string line;
    while(std::getline(infile, line)) {
      std::string x;
      std::istringstream iss(line);
      iss>>x;
      tmp.push_back(x);
    }
    xcsp = tmp[0];
    grimm = tmp[1];
    mm = tmp[2];
    root = tmp[3];
  }
  
  pugi::xml_document doc;
  doc.load_file(xcsp.c_str());
  auto inst = doc.child("instance");
  auto variables = inst.select_nodes("//variable"); 

  for(auto var : variables) {
    std::string dom = var.node().attribute("domain").value();
    auto req = "//domain[@name='"+dom+"']";
    auto domVal = inst.select_nodes(req.c_str());
    std::istringstream parser(domVal[0].node().child_value());
    std::string tmp;
    IntervalVector<int> d;
    while(parser >> tmp) {
         d.add(tmp);
    }
    domains->push_back(d);
  }
  assert(domains->size() == genes->size());
}

Model::~Model() {}

/* Accessors */

void Model::setVal(GenesPtr v) {
  change = true;
  genes = v;
}

GenesPtr Model::getVal() const {
  return genes;
}

void Model::setDomains(DomainsPtr d) {
  change = true;
  domains = d;
}

DomainsPtr Model::getDomains() const {
  return domains;
}

/* Evaluation operation */

std::string Model::generateDotFile(int i) {
  if(!change) return dot;
  change = false;
	//std::string outdirGen = NSGAII::dir+"/output/dotgen"+std::to_string(NSGAII::gen);
  std::string outfileChrono = NSGAII::dir+"/jvmconsuption/jvmconsuption"+std::to_string(NSGAII::gen);

  if(dot != "") {
    auto cmd = "rm "+dot;
    system(cmd.c_str());
  }
  
  auto tmp = "tmp"+std::to_string(i);
  std::ofstream of(tmp);
  of << *this;
  of.flush();
  of.close();
  FILE *in;
  std::string fileName;
  char buff[512];
  std::string cmd;
  if(mm == "MyJava.ecore") {
    if(NSGAII::gen == NSGAII::maxGen)
      cmd = "java -jar grimm4java.jar -mm="+mm+" -rootClass="+root+" -configFile="+grimm+" -java=1 -val="+tmp;
    else
      cmd = "java -jar grimm4java.jar -mm="+mm+" -rootClass="+root+" -configFile="+grimm+" -java=0 -val="+tmp;
  }
  else
    cmd = "java -jar grimm.jar -mm="+mm+" -root="+root+" -cfg="+grimm+" -dot -val="+tmp;
  
  //  std::string cmd = "java -jar grimm4ga3.jar -n=6 -d=0.2 -val=tmp";

  //  auto t1 = std::chrono::high_resolution_clock::now();  
  if(!(in = popen(cmd.c_str(), "r"))) {
    errno = 0;
    std::cerr<<"Error: "<<strerror(errno)<<std::endl;
    return "";
    //exit(2);
  }

  int jvmLaunch;
  auto first = true;
  while(fgets(buff, sizeof(buff), in) != NULL) {
    if(first) {
      //     auto t2 = std::chrono::high_resolution_clock::now();  
      //jvmLaunch = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
      first = false;
    }
    //std::cout<<buff << std::endl;
    std::string cmp = buff;
    if(mm == "MyJava.ecore") {
      auto pos = cmp.find("filepath -- ");
      if(pos != std::string::npos) {
				fileName = cmp.substr(pos+12);
				pos = fileName.find("pdf");
				fileName = fileName.substr(0,pos);
				fileName += "dot";
				break;
      }
    }
    else {
      auto pos = cmp.find(" is the generated model");
      if(pos != std::string::npos) {
				pos = cmp.find("pdf");
				fileName = cmp.substr(1,pos-1);
				fileName += "dot";
				break;
      }
    }
  }
  pclose(in);
  //  std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
	//std::cout << fileName << std::endl;
	//system(("mv "+fileName+" "+outdirGen).c_str());
	//std::string rawfileName = fileName.substr(fileName.find("/"),fileName.length());
  //dot = outdirGen+rawfileName;
  //auto t2 = std::chrono::high_resolution_clock::now();
	dot = fileName;

	//display of dot file and corresponding values
	//std::cout << dot << std::endl << *this << std::endl;
  
  Logger l(outfileChrono,std::ios_base::app);
  //l<<"dot "<<jvmLaunch<<" "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<"\n";
  return dot;
}

float Model::evaluate(const Model& m1,const Model& m2) {
  return levenshteinDistance(m1,m2);
}

float Model::levenshteinDistance(const Model& m1,const Model& m2) {

  auto g1 = m1.getVal();
  auto g2 = m2.getVal();
  int d[g1->size()][g2->size()];
  int substitutionCost;

  for(unsigned i = 0; i < g1->size(); i++) {
    for(unsigned j = 0; j < g2->size(); j++) {
      d[i][j] = 0;
    }
  }
  for(unsigned k = 0; k < g1->size(); k++) { d[k][0] = k; }
  for(unsigned k = 0; k < g2->size(); k++) { d[0][k] = k; }

  for(unsigned i = 1; i < g2->size(); i++) {
    for(unsigned j = 1; j < g1->size(); j++) {
      if(g1->at(i) == g2->at(j)) {
	substitutionCost = 0;
      }
      else {
	substitutionCost = 1;
      }

      d[i][j] = std::min(std::min(
				  d[i-1][j] + 1,  // erase new char of g1
				  d[i][j-1] + 1), // insert into g1 of
			                          // g2 new char
			 d[i-1][j-1] + substitutionCost); // substitution
    }
  }
     
  return float(d[g1->size()-1][g2->size()-1]);
}

float Model::cosineDistance(const Model& m1, const Model& m2) {

  auto num = 0.0;
  auto g1 = m1.getVal();
  auto g2 = m2.getVal();
  if(g1->size() > g2->size()) {
    auto tmp = g1;
    g1 = g2;
    g2 = tmp;
  }

  long double den1 = 0.0;
  long double den2 = 0.0;

  for(size_t i = 0;i<g1->size();i++) {
    num += (g1->at(i) * g2->at(i));
  }

  for(size_t i = 0;i<g1->size();i++) {
    den1 += (g1->at(i) * g1->at(i));
  }
  for(size_t i = 0;i<g2->size();i++) {
    den2 += (g2->at(i) * g2->at(i));
  }

  //  std::cout<<"\tDistance entre \n"<<m1<<"\net\n"<<m2<<"\n= "<<1-num/(sqrt(den1)*sqrt(den2))<<std::endl;
  
  return 1-(num/(sqrt(den1)*sqrt(den2)));
}

std::tuple<float,float,float> Model::evaluateExtern(Model& m1,Model& m2) {
  auto fut1 = std::async(std::launch::async,&Model::generateDotFile,&m1,0);
  auto fut2 = std::async(std::launch::async,&Model::generateDotFile,&m2,1);

  auto dot1 = fut1.get();
  auto dot2 = fut2.get();
  
  auto centrality = 0.0,
    hamming = 0.0,
    levenshtein = 0.0;
  std::string cmd = "java -jar gDistances2.jar ";
  cmd += dot1 + " " + dot2 + "";
  
  FILE *in;
  char buff[512];
  auto t1 = std::chrono::high_resolution_clock::now();  
  if(!(in = popen(cmd.c_str(),"r"))) {
    errno = 0;
    std::cerr<<strerror(errno)<<std::endl;
    return std::make_tuple(-1,-1,-1);
  }
  auto maxEdges = 1.0;

  auto jvmLaunch = 0;
  auto first = true;
  while(fgets(buff, sizeof(buff), in) != NULL) {
    if(first) {
      auto t2 = std::chrono::high_resolution_clock::now();  
      jvmLaunch = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
      first = false;
    }
    std::string tmp = buff;
    std::string strToMatch = "max edges=";
    auto pos = tmp.find(strToMatch);
    if(pos != std::string::npos) {
      maxEdges = std::stoi(tmp.substr(strToMatch.length()));
      // std::cout<<maxEdges<<std::endl;
    }
    else {
      pos = tmp.find("euclide=");
      if((GAChromosom::method & GAChromosom::Method::CENTRALITY) &&
	 pos != std::string::npos) {
	centrality = std::stof(tmp.substr(pos+std::string("euclide=")
					  .length(),
					  tmp.find(",",pos) -
					  (pos+std::string("euclide=")
					   .length())
					  ));
      }
      else if((GAChromosom::method & GAChromosom::Method::LEVEXTERN)) {
	pos = tmp.find("Levenshtein=");
	if(pos != std::string::npos) {
	  levenshtein = std::stof(tmp.substr(
					 pos + 
					 std::string("Levenshtein=")
					 .length()));
	}
      }
      else {
	pos = tmp.find("HammingCycle=");
	if((GAChromosom::method & GAChromosom::Method::HAMMING) &&
	   pos != std::string::npos) {
	  hamming = std::stof(tmp.substr(
					 pos +
					 std::string("HammingCycle=")
					 .length(),
					 tmp.find("]",pos) -
					 (pos +
					 std::string("HammingCycle=")
					  .length()) 
					 )) / maxEdges;
	}
      }
    }
  }
  pclose(in);
  auto t2 = std::chrono::high_resolution_clock::now();  
  auto outfile = NSGAII::dir+"/jvmconsuption/jvmconsuption"+std::to_string(NSGAII::gen);
  
  Logger l(outfile,std::ios_base::app);
  l<<"ev "<<jvmLaunch<<" "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<"\n";
  return std::make_tuple(hamming,centrality,levenshtein);
}

bool Model::isValid() const {
  pugi::xml_document doc;
  doc.load_file(xcsp.c_str());
  auto inst = doc.child("instance");

  /*
    <predicate name="eq">
      <parameters>int v1 int v2</parameters>
      <expression>
        <functional>eq(v1,v2)</functional>
      </expression>
    </predicate>
  */

  auto pred = inst.select_nodes("//predicates")[0].node();
  auto nb = std::stoi(pred.attribute("nbPredicates").value());
  pred.attribute("nbPredicates").set_value(++nb);
  auto eq = pred.append_child("predicate");
  eq.append_attribute("name") = "eq";
  auto param = eq.append_child("parameters");
  param.text() = "int v1 int v2";
  eq.append_child("expression").append_child("functional")
    .text() = "eq(v1,v2)";

  auto variables = inst.select_nodes("//variable");
  auto constraints = inst.select_nodes("//constraints")[0].node();
  nb = std::stoi(constraints.attribute("nbConstraints").value());
  int i = 0;
  for(auto var : variables) {
    std::string v = var.node().attribute("name").value();
    auto c = constraints.append_child("constraint");
    c.append_attribute("name") = (v+"_val").c_str();
    c.append_attribute("arity") = "1";
    c.append_attribute("scope") = v.c_str();
    c.append_attribute("reference") = "eq";
    c.append_child("parameters").text() =
      (std::to_string(genes->at(i++)) + " " + v).c_str();
    /*
      <constraint name="F_Project_1_name_val" arity="1" scope="F_Project_1_name" reference="eq">
      <parameters>1 F_Project_1_name</parameters>
      </constraint>
    */
  }
  constraints.attribute("nbConstraints").set_value(nb+i);
  doc.save_file("tmp.xml");

  FILE *in;
  char buff[512];
  //  auto t1 = std::chrono::high_resolution_clock::now();  
  if(!(in = popen("java -jar abssol.jar tmp.xml","r"))) {
    errno = 0;
    std::cerr<<strerror(errno)<<std::endl;
    return false;
  }

  auto ret = false;
  auto first = true;
  auto jvmLaunch = 0;
  while(fgets(buff, sizeof(buff), in) != NULL) {
    if(first) {
      //  auto t2 = std::chrono::high_resolution_clock::now();  
      //jvmLaunch = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
      first = false;
    }
    std::string tmp = buff;
    auto pos = tmp.find("SATISFIABLE");
    if(pos != std::string::npos) {
      pos = tmp.find("UNSATISFIABLE");
      ret = pos == std::string::npos;
      break;
    }
  }
  pclose(in);
  
  //auto t2 = std::chrono::high_resolution_clock::now();  
  auto outfile = NSGAII::dir+"/jvmconsuption/jvmconsuption"+std::to_string(NSGAII::gen);
  
  Logger l(outfile,std::ios_base::app);
  //l<<"sol "<<jvmLaunch<<" "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<"\n";
  return ret;
}

const std::vector<std::string> explode(const std::string& s, const char& c)
{
	std::string buff{""};
	std::vector<std::string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

std::string Model::generateDotFileScaffold(std::string fileName){
	pugi::xml_document doc;
  doc.load_file(xcsp.c_str());
  auto inst = doc.child("instance");
  auto variables = inst.select_nodes("//variable");
  GenesPtr genVal = this->getVal();
  std::ofstream output(fileName);
  output << "digraph mon_graphe {" << std::endl;
  
  bool trio = false;
  std::string line;
  int i = 0;
  
  for(auto var : variables){
  	std::string v = var.node().attribute("name").value();
  	std::vector<std::string> vector{explode(v, '_')};
  	if(!vector.empty()){
  		if(vector.at(3) == "vertices"){
  			output << std::stoi(vector.at(4)) << std::endl;
  		}
  		if(vector.at(0) == "F" && vector.at(1) == "Edge"){
				trio = true;
  		}
  		else{
  			if(trio){
  				int gen = (*(genVal.get()))[i];
  				if(vector.at(3) == "EVin"){
						line = std::to_string(gen) + " -> ";
  				}
  				if(vector.at(3) == "EVout"){
  					if(!(gen == 2 && line[0] == '2')){
  						output << line + std::to_string(gen) << std::endl;
  					}
  					trio = false;
  				}
				}
  		}
  	}
  	i++;
  }
  output << "}";
  output.close();
  return "";
}

/* Comparison operators */

bool Model::operator==(const Model& b) {
  return *(this->genes) == *(b.genes);
}

bool Model::operator!=(const Model& b) {
  return !(*this == b);
}

/* String methods and operators */

std::string Model::to_string() const {
  std::ostringstream oss;
  for(auto a : *genes) {
    oss << a << " ";
  }
  return oss.str();
}

std::ostream& operator<<(std::ostream& os,const Model& m) {
  os << m.to_string();
  return os;
}
