# Global config

NULL = 
OS = $(shell uname -s)

# GALib config

GA_INC_DIR = lib/galib/
GA_LIB_DIR = lib/galib/ga
GA_LIB = $(GA_LIB_DIR)/libga.a

INC_DIRS = -I $(GA_INC_DIR)
LIB_DIRS = -L$(GA_LIB_DIR)

# Global compilator config

CXX = g++ 
ifeq ($(OS),Linux)
CXXFLAGS = -Wall -Wpedantic -std=c++14 -g -I .
CXX_LIBS = -lpugixml -lpthread -lm
else
CXXFLAGS = -Wall -Wpedantic -std=c++14 -g -I . -I ./lib/pugixml-1.8/ 
CXX_LIBS = -L ./lib/pugixml-1.8/ -lpugixml -lpthread -lm
endif
# Unit test config

TEST_LIBS = -lCppUTest -lCppUTestExt

# Application files

APP_SRC = \
	main.cpp \
	$(NULL)

APP_OBJ = $(APP_SRC:%.cpp=%.o)

APP = mdea

# Utils files

UTIL_SRC = \
	utils/Directory.cpp \
	utils/CommandLine.cpp \
	utils/Logger.cpp \
	$(NULL)

UTIL_OBJ = $(UTIL_SRC:%.cpp=%.o)

# GA files

GA_SRC = \
	model/Statistics.cpp \
	model/Chromosom.cpp \
	model/GAChromosom.cpp \
	model/Population.cpp \
	model/Matrix.cpp \
	model/Model.cpp \
	model/NSGAII.cpp \
	$(NULL)

GA_OBJ = $(GA_SRC:%.cpp=%.o)

# Unit test files

TEST_SRC = \
	tests/main.cpp \
	tests/test-matrix.cpp \
	tests/test-gachromosom.cpp \
	tests/test-population.cpp \
	tests/test-model.cpp \
	tests/test-chromosom.cpp \
	tests/test-intervals.cpp \
	$(NULL)

TEST_OBJ = $(TEST_SRC:%.cpp=%.o)

TEST = test

# Compile all

all: lib $(APP) $(TEST)

# Compile application

$(APP): $(APP_OBJ) $(GA_OBJ) $(UTIL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(INC_DIRS) $(LIB_DIRS) -lga -lm $(CXX_LIBS)

$(GA_OBJ): %.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@ 

$(APP_OBJ): %.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@ 

# Compile utils

$(UTIL_OBJ): %.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

# Compile test

$(TEST): $(TEST_OBJ) $(GA_OBJ) $(UTIL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(INC_DIRS) $(LIB_DIRS) -lga -lm $(CXX_LIBS) $(TEST_LIBS)

$(TEST_OBJ): %.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@ $(TEST_LIBS)

# Compile lib

lib: $(GA_LIB)

$(GA_LIB):
	make -C $(GA_INC_DIR)

# Install required lib

ifeq ($(OS),Linux)
install_lib:
	apt-get install cpputest
else
install_lib:
	/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
	brew install cpputest
endif

# Misc

clean:
	rm -f $(APP_OBJ) $(UTIL_OBJ) $(GA_OBJ) $(TEST_OBJ) *~

cleanall: clean
	make -C $(GA_INC_DIR) clean

.PHONY: all $(APP) $(TEST) clean cleanall
