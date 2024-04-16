# Replace /Users/username with the absolute path to your home directory
REPL_PATH=/Users/josephine/Local/Github/Project3

export CURLHOME=${REPL_PATH}/dependencies/curl
export BRIDGES_CXX_INSTALL=${REPL_PATH}/dependencies/bridges-cxx-3.4.3-arm64-apple-darwin22.3.0

export BRIDGES_CXXFLAGS=-I${BRIDGES_CXX_INSTALL}/include -I${CURLHOME}/include/
export BRIDGES_LDFLAGS=-L${BRIDGES_CXX_INSTALL}/lib -L${CURLHOME}/lib/

export BRIDGES_CXX=clang++
export BRIDGES_CC=clang++
export BRIDGES_LD=clang++

export LD_LIBRARY_PATH:=${CURLHOME}/lib:${LD_LIBRARY_PATH}

CXX=$(BRIDGES_CXX)
LD=$(BRIDGES_LD)
CC=$(BRIDGES_CC)

TARGET =  all 

# Add your source files here
SRCS = main.cpp

# Add your executable name here
EXE = my_program

OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -g  -std=c++14 $(BRIDGES_CXXFLAGS)
LDFLAGS = -g $(BRIDGES_LDFLAGS) -pthread
LIBS =  -lcurl -lbridges 

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LDFLAGS) $(LIBS)

.SUFFIXES: .cpp

.cpp.o:  
	$(CXX) $(CXXFLAGS) -c $< -o $@

all : $(EXE)
	./$(EXE)

run: $(EXE)
	./$(EXE)

clean:
	-rm $(OBJS)
	-rm $(EXE)