#ifndef SlidingTileIDA_h__
#define SlidingTileIDA_h__

#include <time.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <cmath>

#include "SLState.h"
#include "AbstractSLHeuristic.h"

using namespace::std;

class SlidingTileIDA {

	private:
		AbstractSLHeuristic * hf;
		
		long nodesExpanded;
		long nodesGenerated;
		long s, end;
		bool finished;
		float cost;
		map<int, long> nodesExpandedByLevel;
		map<int, long> nodesExpandedByLevelByIteration;

	public:

		SlidingTileIDA(AbstractSLHeuristic* hf)
		{
			this->hf = hf;
			this->nodesExpanded = 0;
			this->nodesGenerated = 0;
			this->cost = 0;
			this->finished = false;
			this->end = this->s = 0;
		}

		void idaStar(SLState &start)
		{
			this->nodesExpanded = 0;
			this->nodesGenerated = 0;
			this->finished = false;
			this->end = this->s = 0;

			s = clock();

			float thresh = hf->getHeuristic(start);

			this->nodesExpanded = 0;
			this->nodesGenerated = 0;

			float startHeuristic = thresh;
			while(!finished)
#endif
			{
#ifdef VERBOSE
				cout << "Depth " << thresh << " ";
#endif

				float newThresh = costLimitedBFS(start, 0, thresh, -1, start.getBlank(), startHeuristic);
#ifdef VERBOSE
				cout << "Nodes expanded " << this->nodesExpanded <<" nodes generated "<< this->nodesGenerated << endl;
#endif
				thresh = newThresh;
			}
#ifdef VERBOSE
			cout << "Execution time " << (double)(end - s) / (double)CLOCKS_PER_SEC << endl;
			cout << "Number of nodes generated was " << this->nodesGenerated << endl;
			cout << "Number of nodes expanded was " << this->nodesExpanded << endl << endl << endl;
#endif
		}

		float costLimitedBFS(SLState &currentState, float g, float thresh, int op, int parentBlank, int parentHeuristic)
		{
			float heuristic = hf->getHeuristic(currentState);

			float newThresh = 23049358; //a large number
			float funcReturn;

			if (g + heuristic > thresh) 
			{
    			return (g + heuristic);
			}

			if(currentState.isGoal())
			{
				end = clock();
				finished = true;
				cost = g;
				return newThresh;
			}

			this->nodesExpanded++;

			vector<int>* actions = currentState.getActions(op);

			for(vector<int>::iterator it = actions->begin(); it != actions->end() && !finished; ++it)
			{
				this->nodesGenerated++;

				int parentBlank = currentState.getBlank();
#ifdef SQRT_COST
				float op_cost = sqrt(currentState.getTileToBeMoved(*it));
#endif

       			currentState.applyAction(*it);
#ifdef SQRT_COST
       			funcReturn = costLimitedBFS(currentState, g + op_cost, thresh, *it, parentBlank, heuristic);
#else
       			funcReturn = costLimitedBFS(currentState, g + 1, thresh, *it, parentBlank, heuristic);
#endif
				currentState.reverseAction(*it);   
				newThresh = newThresh < funcReturn ? newThresh : funcReturn;
			}

			delete actions;

			return newThresh;
		}

		float getCost()
		{
			return cost;
		}

		int getTotalNodesExpanded()
		{
			return this->nodesExpanded;
		}

		map<int, long>& getNodesExpanded()
		{
			return nodesExpandedByLevel;
		}
};

