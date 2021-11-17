include Make.inc
#include Make_msys2.inc
#include Make_osx.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS += -O3 -march=native -Wall
endif

default:	help

all:	HelloWorld.exe HelloWorldOutput.exe Circulation_jeton.exe Calcul_de_pi.exe Calcul_de_pi_bloquant.exe Diffusion_hypercube.exe

HelloWorld.exe:	HelloWorld.cpp
	$(CXX) $(CXXFLAGS) -o HelloWorld.exe HelloWorld.cpp $(LIB)
HelloWorldOutput.exe:	HelloWorldOutput.cpp
	$(CXX) $(CXXFLAGS) -o HelloWorldOutput.exe HelloWorldOutput.cpp $(LIB)
Circulation_jeton.exe:	Circulation_jeton.cpp
	$(CXX) $(CXXFLAGS) -o Circulation_jeton.exe Circulation_jeton.cpp $(LIB)
Calcul_de_pi.exe:	Calcul_de_pi.cpp
	$(CXX) $(CXXFLAGS) -o Calcul_de_pi.exe Calcul_de_pi.cpp $(LIB)
Calcul_de_pi_bloquant.exe:	Calcul_de_pi_bloquant.cpp
	$(CXX) $(CXXFLAGS) -o Calcul_de_pi_bloquant.exe Calcul_de_pi_bloquant.cpp $(LIB)
Diffusion_hypercube.exe:	Diffusion_hypercube.cpp
	$(CXX) $(CXXFLAGS) -o Diffusion_hypercube.exe Diffusion_hypercube.cpp $(LIB)

help: 
	@echo "Available targets : "
	@echo "    all                      : compile all executables"
	@echo "    HelloWorld.exe           : compile HelloWorld executable"
	@echo "    HelloWorldOutput.exe     : compile HelloWorldOutput executable"
	@echo "    Circulation_jeton.exe    : compile Circulation_jeton executable"
	@echo "    Calcul_de_pi.exe         : compile Calcul_de_pi executable"
	@echo "    Calcul_de_pi_bloquant.exe         : compile Calcul_de_pi_bloquant executable"
	@echo "    Diffusion_hypercube.exe  : compile Diffusion_hypercube executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

clean:
	@rm -f *.exe *~ 

