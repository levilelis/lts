#define COMPACT_OUTPUT
//#define VERBOSE

//#define RANDOM_GENERATE_STATES 2

#define USE_MD
#define SQRT_COST
#define WEIGHT 1

#include <string>
#include <map>
#include <time.h>
#include <iomanip>
#include <math.h>

#include "slidingtile/SlidingTileLTS.h"
#include "slidingtile/AbstractSLHeuristic.h"
#include "slidingtile/MDHeuristic.h"

using namespace::std;

int main(int argc, char **argv)
{
	if( argc != 4 )
	{
		cout << "Invalid arguments!" << endl;
		cout << "USAGE: SLSolveer size width input-problems" << endl;
		exit(-1);
	}

	SLState::size = atoi(argv[1]);
	SLState::width = atoi(argv[2]);
	string inputProblemsFile(argv[3]);

	cout << "# Solving the following problems " << inputProblemsFile << endl << endl;
	cerr << "# Solving the following problems " << inputProblemsFile << endl << endl;

	SlidingTileLTS * lts = 0;
	AbstractSLHeuristic * hf = new MDHeuristic();

	lts = new SlidingTileLTS(hf);

	int * goal_permutation = new int[SLState::size];
	int * goal_pos = new int[SLState::size];
	for(int i = 0; i < SLState::size; i++)
	{
		goal_permutation[i] = i;
		goal_pos[i] = i;
	}

	SLState goal;
	goal.setPermutation(goal_permutation);
	goal.setPos(goal_pos);
	goal.findAndSetBlank();

#ifndef RANDOM_GENERATE_STATES
	SLState state;
	state.setInputProblemsFile(inputProblemsFile);
#endif

	long sIda, endIda, sPrediction, endPrediction, s, e;

	cout << setiosflags(ios::left);
	cerr << setiosflags(ios::left);

	double totalCost = 0;
	double totalTime = 0;
	double totalExpanded = 0;

#ifdef COMPACT_OUTPUT
		cout << setw(15) << "Solution" << setw(15) << "CPU Time" << setw(15) << "Nodes Expanded" << endl;
#endif

	s = clock();

#ifdef RANDOM_GENERATE_STATES
	int * permutation = new int[SLState::size];
	int * pos = new int[SLState::size];
	for(int i = 0; i < SLState::size; i++)
	{
		permutation[i] = i;
		pos[i] = i;
	}

	SLState state;
	state.setPermutation(permutation);
	state.setPos(pos);

	int numberStates = 0;
	while(numberStates < RANDOM_GENERATE_STATES)
#else
	while(state.populateState())
#endif
	{

#ifdef RANDOM_GENERATE_STATES
		state.shuffle();
		state.findAndSetBlank();

		if(!state.isValid())
		{
			continue;
		}

		numberStates++;
#endif
		//int* p = state.getPermutation();

		sIda = clock();
		lts->idlts(state);
		endIda = clock();

		long nodesExpanded = lts->getTotalNodesExpanded();
		float cost = lts->getCost();

		totalCost += cost;
		totalTime += (double)(endIda - sIda) / (double)CLOCKS_PER_SEC;
		totalExpanded += nodesExpanded;

#ifdef COMPACT_OUTPUT
		cout << setw(15) << cost << setw(15) << (double)(endIda - sIda) / (double)CLOCKS_PER_SEC << setw(15) << nodesExpanded << endl;
#endif

	}
	e = clock();

	//cout << "# Execution time: " << (double)(e - s) / (double)CLOCKS_PER_SEC << " s." << endl;
	cout << setw(15) << "Total cost" << setw(15) << "Total CPU Time" << setw(15) << "Total Nodes Expanded" << endl;
	cout << setw(15) << totalCost << setw(15) << totalTime << setw(15) << totalExpanded << endl;

	delete lts;

	return 0;
}
