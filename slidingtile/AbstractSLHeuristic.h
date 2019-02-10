#ifndef AbstractSLHeuristic_h__
#define AbstractSLHeuristic_h__

#include <stdio.h>
#include <stdlib.h>

#include "SLState.h"

class AbstractSLHeuristic {
 
public:
	virtual float getHeuristic(SLState & state) = 0;
	//virtual int getIncrementalMD(int parentHeuristic, int childBlank, int parentBlank, int parentMovedTile) = 0;
};

#endif
