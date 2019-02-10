#ifndef SlidingTileLTS_h__
#define SlidingTileLTS_h__

#include <time.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <cmath>

#include "SLState.h"
#include "AbstractSLHeuristic.h"

using namespace::std;

class SlidingTileLTS {

	private:
		AbstractSLHeuristic * hf;
		
		long nodesExpanded;
		long nodesGenerated;
		long s, end;
		bool finished;
		float cost;
		float temperature;

	public:

		SlidingTileLTS(AbstractSLHeuristic* hf)
		{
			this->hf = hf;
			this->nodesExpanded = 0;
			this->nodesGenerated = 0;
			this->cost = 0;
			this->finished = false;
			this->end = this->s = 0;
			this->temperature = sqrt((float)SLState::size);
		}

		void idlts(SLState &start)
		{
			this->nodesExpanded = 0;
			this->nodesGenerated = 0;
			this->finished = false;
			this->end = this->s = 0;

			s = clock();

			float startHeuristic = hf->getHeuristic(start);

			this->nodesExpanded = 0;
			this->nodesGenerated = 0;

			int bound = 1;
			while(!finished)
			{
				//cout << "Bound: " << bound << endl;
				costLimitedBFS(start, 0, bound, -1, start.getBlank(), startHeuristic, 1, 0);
				bound++;
			}
		}

		void compute_f_values(int op, SLState &currentState, vector<float> & f_values_children, float g)
		{
			//float h_parent = hf->getHeuristic(currentState);
			//cout << "h parent: " << h_parent << " g parent: " << g << endl;
			//currentState.print();

			vector<int>* actions = currentState.getActions(op);
			for(vector<int>::iterator it = actions->begin(); it != actions->end(); ++it)
			{
				float op_cost = sqrt(currentState.getTileToBeMoved(*it));
				currentState.applyAction(*it);
				//cout << "h child: " << this->hf->getHeuristic(currentState) << " g child: " << g + op_cost << " op: " << *it << endl;
				//currentState.print();
				f_values_children.push_back(g + op_cost + this->hf->getHeuristic(currentState));
				currentState.reverseAction(*it);
				//currentState.print();
			}

			delete actions;
			//cout << endl << endl;
		}

		void compute_probability_distribution_from_values(vector<float> &f_values, vector<float> & probability_distribution, float f_value)
		{
			float den = 0.0;

			for(vector<float>::iterator it = f_values.begin(); it != f_values.end(); ++it)
			{
				den += exp((f_value - *it)/temperature);
			}

			for(vector<float>::iterator it = f_values.begin(); it != f_values.end(); ++it)
			{
				probability_distribution.push_back(exp((f_value - *it)/temperature)/den);
			}
		}

		void costLimitedBFS(SLState &currentState, float g, int bound, int op, int parentBlank, int parentHeuristic, int depth, float p)
		{
			float heuristic = hf->getHeuristic(currentState);

			//cout << depth << endl;

			float v = log2(depth) - p;
			if (v > bound)
			{
    			return;
			}

			if(currentState.isGoal())
			{
				end = clock();
				finished = true;
				cost = g;
				return;
			}

			this->nodesExpanded++;

		    vector<float> f_values_children;
		    vector<float> probability_distribution;
		    compute_f_values(op, currentState, f_values_children, g);
		    compute_probability_distribution_from_values(f_values_children, probability_distribution, heuristic + g);
			vector<float>::iterator iterator_probs = probability_distribution.begin();

			/*
			cout << "Parent f-value: " << heuristic + g << endl;
			for(int i = 0; i < f_values_children.size(); i++)
			{
				cout << "[" << f_values_children[i] << ", " << probability_distribution[i] << "]  ";
			}
			cout << endl << endl;
*/
			vector<int>* actions = currentState.getActions(op);
			for(vector<int>::iterator it = actions->begin(); it != actions->end() && !finished; ++it)
			{
				this->nodesGenerated++;

				int parentBlank = currentState.getBlank();
				float op_cost = sqrt(currentState.getTileToBeMoved(*it));

       			currentState.applyAction(*it);
#ifdef SQRT_COST
       			costLimitedBFS(currentState, g + op_cost, bound, *it,
       					parentBlank, heuristic, depth + 1, p + log2(*iterator_probs));
#else
       			costLimitedBFS(currentState, g + 1, bound, *it, parentBlank, heuristic, depth + 1, p + log2(*iterator_probs));
#endif
				currentState.reverseAction(*it);   
				iterator_probs++;
			}

			delete actions;
		}

		float getCost()
		{
			return cost;
		}

		int getTotalNodesExpanded()
		{
			return this->nodesExpanded;
		}
};

#endif
