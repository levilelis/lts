#ifndef SLState_H__
#define SLState_H__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <iostream>

using namespace::std;

//struct ActionAvailable {
//	int action;
//	bool isAvailable;
//};

class SLState {

private:
	int * puzzle;
	int * pos;
	int op;
	string inputFile;
	ifstream indata;
	int blank;
	long seed;
	long w;
	int opt_cost;

public:
	static int size;
	static int width;
	static int height;
	static int cut_off;
	//int op;

	const static int N;
	const static int S;
	const static int E;
	const static int W;
	const static int NO_OP;

	SLState();
	SLState(const SLState& state);
	~SLState();
	void reverseAction(int action);
	void resetSeed();
	void applyAction(int action);
	void findAndSetBlank();
	int getTileToBeMoved(int action);
	int countActions(int op);
	vector<int>* getActions(int op);
	void print();
	void shuffle();
	bool populateState();
	void restartInputProblemsFile();
	void setInputProblemsFile(string f);
	void setPos(int* p);
	void setPermutation(int* p);
	int* getPos();
	int* getPermutation();
	int getOp();
	int getOptCost();
	void setOp(int op);
	const SLState& operator = (const SLState& s);
	void setPuzzleValue(int i, int value);
	int getPuzzleValue(int i) const;
	int getBlank();
	bool isValid();
	bool operator==(const SLState& state);
	bool isGoal();
	friend bool operator< (const SLState&, const SLState&);
	void randomWalk(int size);

	long getW() const {return w;}
	void setW(long i) {w = i;}
};

SLState::SLState()
{
	puzzle = new int[size];
	pos = new int[size];
	seed = time(NULL);
	srand ( seed );

	opt_cost = -1;
}

SLState::SLState(const SLState& state)
{
	puzzle = new int[size];
	pos = new int[size];

    for (int i = 0; i < size; i++) 
	{
		this->puzzle[i] = state.puzzle[i];
		this->pos[i] = state.pos[i];
	}

	this->blank = state.blank;
	this->op = state.op;
	this->w = state.w;
	this->opt_cost = state.opt_cost;
}

SLState::~SLState()
{
    delete [] puzzle;
	delete [] pos;
}


bool SLState::isGoal()
{
   	for(int i = 0; i < size; i++)
	{
		if(puzzle[i] != i)
		{
			return false;
		}
	}

	return true;
}

bool SLState::operator==(const SLState& state)
{
    for (int i = 0; i < size; i++) 
	{
		if(this->puzzle[i] != state.puzzle[i])
		{
			return false;
		}
	}

	return true;

}

bool operator< (const SLState& o1, const SLState& o2)
{
	for (int i = 0; i < SLState::size; i++) 
	{
		if(o1.puzzle[i] != o2.puzzle[i])
		{
			return o1.puzzle[i] < o2.puzzle[i];
		}
	}

	return false;
}

bool SLState::isValid()
{
	int i, l, t;

	if (!((width & 1) > 0)) 
	{
		t = pos[0] / width;
	} 
	else 
	{
		t = 0;
	}
	
	for (i = 2; i < size; i++) 
	{
		for (l = 1; l < i; l++) 
		{
			if (pos[i] < pos[l]) 
			{
				t++;
			}
		}
	}
	return (t & 1) ^ 1;
}

int SLState::getOp()
{
	return op;
}

void SLState::setOp(int op)
{
	this->op = op;
}

int SLState::getBlank()
{
   return blank;
}

int SLState::getOptCost()
{
	return opt_cost;
}

int SLState::getPuzzleValue(int i) const
{
	return puzzle[i];
}

void SLState::setPuzzleValue(int i, int value)
{
	puzzle[i] = value;
}

const SLState& SLState::operator= (const SLState& s) 
{
	if (&s != this) 
	{
		for (int i = 0; i < size; i++) 
		{
			puzzle[i] = s.puzzle[i];
			pos[i] = s.pos[i];
		}

		blank = s.blank;
		op = s.op;
		w = s.w;
		opt_cost = s.opt_cost;

		return *this;
	}            
}

int* SLState::getPermutation() 
{
	return puzzle;
}

int* SLState::getPos()
{
   return pos;
}

void SLState::setPermutation(int* p)
{
   delete [] puzzle;
   puzzle = p;

   for(int i = 0; i < size; i++)
   {
	   pos[puzzle[i]] = i;
   }
}

void SLState::setPos(int* p)
{
   delete [] pos;

   pos = p;
}

void SLState::setInputProblemsFile(string f)
{
   this->inputFile = f;
   indata.open(inputFile.c_str());

   	if(!indata) 
	{
		cerr << "Error: input problems file could not be opened" << endl;
		exit(1);
	}
}

void SLState::restartInputProblemsFile()
{
   indata.close();
   indata.open(inputFile.c_str());

   	if(!indata) 
	{
		cerr << "Error: input problems file could not be opened" << endl;
		exit(1);
	}
}

bool SLState::populateState()
{
   	char str[2000];
	
	if(!indata.eof()) 
	{
		indata.getline(str, 2000);
		char * tile;

		tile = strtok (str, " ");
		int numberTiles = 0;

		if(tile == NULL)
		{
			return false;
		}

		while (tile != NULL && numberTiles < size)
		{		
			int t = atoi(tile);
			puzzle[numberTiles] = t;
			pos[t] = numberTiles;
			if(t == 0)
			{
				blank = numberTiles;
			}

			numberTiles++;
			tile = strtok (NULL, " ");
		}

		//has its optimal cost in the file
		if( tile != NULL )
		{
			opt_cost = atoi(tile);
		}

		return true;
	}
	else
	{
		indata.close();
		return false;
	}
}

void SLState::shuffle()
{
    for (int i = 0; i < size; i++) 
	{
		int r = rand() % size;
		int swap = puzzle[r];
		puzzle[r] = puzzle[i];
		pos[puzzle[i]] = r;
		puzzle[i] = swap;
		pos[swap] = i;
	}
}

void SLState::print()
{
   //cout << "op = " << op << " ";
   for(int i = 0; i < size; i++)
   {
	   cout << puzzle[i] << " ";
   }

   cout << endl;

   cout << "Pos: ";

   for(int i = 0; i < size; i++)
   {
	   cout << pos[i] << " ";
   }

   cout << endl;
}

void SLState::randomWalk(int size)
{
	for(int i = 0; i < size; i++)
	{
		vector<int> * actions = getActions(-1);
		int random_action = rand() % actions->size();

		applyAction((*actions)[random_action]);
	}
}

vector<int>* SLState::getActions(int op) 
{
	vector<int>* actions = new vector<int>();

	if (blank != (width - 1) && blank != ((width * 2) - 1) && blank != ((width * 3) - 1) && blank != ((width * 4) - 1) && blank != ((width * 5) - 1) && blank != ((width * 6) - 1) && blank != ((width * 7) - 1) && blank != ((width * 8) - 1) && blank != ((width * 9) - 1) && op != W)
	{
		actions->push_back(E);
	}

	if (blank != 0 && blank != width && blank != 2 * width && blank != 3 * width && blank != 4 * width  && blank != 5 * width && blank != 6 * width && blank != 7 * width && blank != 8 * width && op != E)
	{
		actions->push_back(W);
	}

	if (blank < SLState::size - width && op != N) 
	{
		actions->push_back(S);
	}

	if (blank > width - 1 && op != S) 
	{
		actions->push_back(N);
	}

	return actions;
}

int SLState::countActions(int op) 
{
	int count = 0;

	if (blank != (width - 1) && blank != ((width * 2) - 1) && blank != ((width * 3) - 1) && blank != ((width * 4) - 1) && blank != ((width * 5) - 1) && blank != ((width * 6) - 1) && blank != ((width * 7) - 1) && blank != ((width * 8) - 1) && blank != ((width * 9) - 1) && op != W)
	{
		count++;
	}

	if (blank != 0 && blank != width && blank != 2 * width && blank != 3 * width && blank != 4 * width  && blank != 5 * width && blank != 6 * width && blank != 7 * width && blank != 8 * width && op != E)
	{
		count++;
	}

	if (blank < SLState::size - width && op != N) 
	{
		count++;
	}

	if (blank > width - 1 && op != S) 
	{
		count++;
	}

	return count;
}

int SLState::getTileToBeMoved(int action)
{
	if (action == N) 
	{
		return puzzle[blank - width];
	}

	if (action == S) 
	{
		return puzzle[blank + width];
	}

	if (action == E) 
	{
		return puzzle[blank + 1];
	}

	if (action == W) 
	{
		return puzzle[blank - 1];
	}

	return -1;
}

void SLState::findAndSetBlank()
{
	for(int i = 0; i < size; i++)
	{
		if(puzzle[i] == 0)
		{
			blank = i;
			return;
		}
	}
}

void SLState::applyAction(int action) 
{
	if (action == N) 
	{
		puzzle[blank] = puzzle[blank - width];
		pos[puzzle[blank - width]] = blank;
		puzzle[blank - width] = 0;
		pos[0] = blank - width;
		blank = blank - width;
	}

	if (action == S) 
	{
		puzzle[blank] = puzzle[blank + width];
		pos[puzzle[blank + width]] = blank;
		puzzle[blank + width] = 0;
		pos[0] = blank + width;
		blank = blank + width;
	}

	if (action == E) 
	{
		puzzle[blank] = puzzle[blank + 1];
		pos[puzzle[blank + 1]] = blank;
		puzzle[blank + 1] = 0;
		pos[0] = blank + 1;
		blank = blank + 1;
	}

	if (action == W) 
	{
		puzzle[blank] = puzzle[blank - 1];
		pos[puzzle[blank - 1]] = blank;
		puzzle[blank - 1] = 0;
		pos[0] = blank - 1;
		blank = blank - 1;
	}
}

void SLState::resetSeed()
{
	srand ( seed );

	for(int i = 0; i < size; i++)
	{
		puzzle[i] = i;
		pos[i] = i;
	}
}

void SLState::reverseAction(int action)
{
	if (action == S) 
	{
		puzzle[blank] = puzzle[blank - width];
		pos[puzzle[blank - width]] = blank;
		puzzle[blank - width] = 0;
		pos[0] = blank - width;
		blank = blank - width;
	}

	if (action == N) 
	{
		puzzle[blank] = puzzle[blank + width];
		pos[puzzle[blank + width]] = blank;
		puzzle[blank + width] = 0;
		pos[0] = blank + width;
		blank = blank + width;
	}

	if (action == W) 
	{
		puzzle[blank] = puzzle[blank + 1];
		pos[puzzle[blank + 1]] = blank;
		puzzle[blank + 1] = 0;
		pos[0] = blank + 1;
		blank = blank + 1;
	}

	if (action == E) 
	{
		puzzle[blank] = puzzle[blank - 1];
		pos[puzzle[blank - 1]] = blank;
		puzzle[blank - 1] = 0;
		pos[0] = blank - 1;
		blank = blank - 1;
	}
}

int SLState::cut_off     = 0;
int SLState::size        = 0;
int SLState::width       = 0;
int SLState::height      = 0;
const int SLState::N     = 0;
const int SLState::S     = 1;
const int SLState::E     = 2;
const int SLState::W     = 3;
const int SLState::NO_OP = 4;

#endif
