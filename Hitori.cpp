#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdint>
using namespace std;

/***************************************************HITORI SOlVER**************************************************/
//samllest structure in state
struct node
{
	short int value;//between 1 to 9
	char condition;//W for white & B for black & K for kandidate
};
//matrix initialed to be solved -> hitori main table
struct state
{
	int size;
	vector <vector<node> > matrix;
};
//H function-> result shows how 
int heuristic(state s)
{
	int result = 0;
	for (int i = 0; i < s.size; i++)
	{
		short int row[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int j = 0; j < s.size; j++)
		{
			short int column[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
			short int current = s.matrix[i][j].value - 1;
			column[current]++; row[current]++;
			if (column[current]>1)
				result++;
			if (row[current]>1)
				result++;
		}
	}
	return result;
}
//knadidates all the possible nodes that can become black in next moves 
state successor(state s, short int level)
{
	int array[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	for (int i = 0; i < s.size; i++)
	{
		node current = s.matrix[i][level];
		if (array[current.value - 1] != -1)
		{
			int x = array[current.value - 1];
			s.matrix[x][level].condition = 'K';
			s.matrix[i][level].condition = 'K';

		}
		else
		{
			array[current.value - 1] = i;
		}
	}
	int array[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

	for (int i = 0; i < s.size; i++)
	{
		node current = s.matrix[level][i];
		if (array[current.value - 1] != -1)
		{
			int y = array[current.value - 1];
			s.matrix[level][y].condition = 'K';
			s.matrix[level][i].condition = 'K';

		}
		else
		{
			array[current.value - 1] = i;
			
		}
	}
	return s;
}
bool rules(state s, )

{

}

state hill_climbing(state)
{

}