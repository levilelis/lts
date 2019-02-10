#ifndef MDHeuristic_h__
#define MDHeuristic_h__

#include <stdio.h>
#include <stdlib.h>

#include "SLState.h"
#include "AbstractSLHeuristic.h"

class MDHeuristic : public AbstractSLHeuristic {
 
public:
	
	float getHeuristic(SLState & state)
	{
		float h = 0;

		for(int i = 0; i < SLState::size; ++i)
		{
			if(state.getPuzzleValue(i) != 0)
			{
				//if(i == state.getBlank())
				//	continue;
#ifdef SQRT_COST
				h = h + sqrt(state.getPuzzleValue(i)) * getDistance(state.getPuzzleValue(i), i);
#else
				h = h + getDistance(state.getPuzzleValue(i), i);
#endif
			}
		}

		return WEIGHT * h;
	}

	int getIncrementalMD(int parentHeuristic, int childBlank, int parentBlank, int parentMovedTile)
	{		
		return parentHeuristic + (getDistance(parentMovedTile, parentBlank) - getDistance(parentMovedTile, childBlank));
	}
	
private:
	/*
	int getDistance(int puzzleValue, int position) 
	{
		for (int i = 0; i < SLState::size; ++i) 
		{
			if (i == puzzleValue) 
			{
				return abs((position % SLState::width) - (i % SLState::width)) + abs((position / SLState::width) - (i / SLState::width));
			}
		}
		return 0;
	}*/

	int getDistance(int i, int j) 
	{
		return abs((i % SLState::width) - (j % SLState::width)) + abs((i / SLState::width) - (j / SLState::width));
	}
};

#endif
