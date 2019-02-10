CC = g++
#OPT = -g
#OPT = -Wall -g -fno-inline
#OPT = -Wall -pg -g -O3 -fno-inline
#OPT = -Wall -O3 -ffast-math -combine

#CC = icc
OPT = -O3 -fomit-frame-pointer -ffast-math

all: PKSolver SLSolver

PKSolver: MainPancakeSolver.cpp
	$(CC) $(OPT) MainPancakeSolver.cpp -o $@

SLIDA: MainSlidingTileIDA.cpp
	$(CC) $(OPT) MainSlidingTileIDA.cpp -o $@

SLLTS: MainSlidingTileLTS.cpp
	$(CC) $(OPT) MainSlidingTileLTS.cpp -o $@
	
clean:
	rm -f PKSolver SLIDA SLLTS
