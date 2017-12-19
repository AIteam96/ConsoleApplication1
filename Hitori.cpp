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
	char condition;//W for white & B for black 
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
vector<state> successor(state s, short int level)
{
	vector<state> vec;
	int array[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	for (int i = 0; i < s.size; i++)
	{
		state temp = s;
		
		node current = temp.matrix[i][level];
		if (current.condition == 'W')
		{
			if (array[current.value - 1] >=0)
			{
				int x = array[current.value - 1];
				temp.matrix[x][level].condition = 'B';
				vec.push_back(temp);
				temp.matrix[x][level].condition = 'W';
				temp.matrix[i][level].condition = 'B';
				vec.push_back(temp);
				temp.matrix[i][level].condition = 'W';
				array[current.value - 1] = -2;
			}
			else if (array[current.value - 1] == -2)
			{
				temp.matrix[i][level].condition = 'B';
				vec.push_back(temp);
				temp.matrix[i][level].condition = 'W';
			}
			else if (array[current.value - 1] == -2)
			{
				array[current.value - 1] = i;
			}
		}
	}
	int array[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

	for (int i = 0; i < s.size; i++)
	{
		state temp = s;
		node current = temp.matrix[level][i];
		if (array[current.value - 1] >= 0)
		{
			int x = array[current.value - 1];
			temp.matrix[level][x].condition = 'B';
			vec.push_back(temp);
			temp.matrix[level][x].condition = 'W';
			temp.matrix[level][i].condition = 'B';
			vec.push_back(temp);
			temp.matrix[level][i].condition = 'W';
			array[current.value - 1] = -2;
		}
		else if (array[current.value - 1] == -2)
		{
			temp.matrix[level][i].condition = 'B';
			vec.push_back(temp);
			temp.matrix[level][i].condition = 'W';
		}
		else if (array[current.value - 1] == -2)
		{
			array[current.value - 1] = i;
		}
	}
	return vec;
}
bool check_rule_two(state s, int x, int y)
{
	//Rule #2: black nodes do not touch each other vertically or horizontally
	bool result=true;
	if (x > 0)
	{
		if (s.matrix[x - 1][y].condition == 'B')
		{
			result = false;
			return result;
		}
	}
	if (x < s.size -1)
	{
		if (s.matrix[x - 1][y].condition == 'B')
		{
			result = false;
			return result;
		}
	}
	if (y > 0)
	{
		if (s.matrix[x][y - 1].condition == 'B')
		{
			result = false;
			return result;
		}
	}
	if (y < s.size - 1)
	{
		if (s.matrix[x][y + 1].condition == 'B')
		{
			result = false;
			return result;
		}
	}
	

}

//Rule #3:all un-shaded (white) squares create a single continuous area T_T
bool check_rule_three(state s, int x, int y)
{

}
//Rule #1(is goal in level): No number appears in a row or column more than once
bool check_rule_one(state s, int level)
{
	int array[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < s.size; i++)
	{
		node current = s.matrix[i][level];
		if (current.condition == 'W')
		{
			if (array[current.value - 1] != 0)
			{
				return false;

			}
			else
			{
				array[current.value - 1] ++;
			}
		}
	}
	int array[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < s.size; i++)
	{
		node current = s.matrix[level][i];
		if (current.condition == 'W')
		{
			if (array[current.value - 1] != 0)
			{
				return false;

			}
			else
			{
				array[current.value - 1] ++;
			}
		}
	}
	return true;
}
vector<pair<int,int>> repeated(state s)
{
	vector<pair<int, int>> result;
	int check[8][3] =
	{
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, -1 },
	};
	for (int i = 0; i < s.size; i++)
	{
		for (int j = 0; j < s.size; j++)
		{
			node current = s.matrix[i][j];
			if (check[current.value][0] == -1)
			{
				check[current.value][0] ++;
				check[current.value][1] = i;
				check[current.value][2] = j;
				
			}
			else
			{
				check[current.value][0] ++;
				pair<int, int> temp;
				temp.first = i;
				temp.second = j;
				result.push_back(temp);
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (check[i][0] > 0)
		{
			pair<int, int> temp;
			temp.first = check[i][1];
			temp.second = check[i][2];
			result.push_back(temp);
		}
	}
	return result;
}

state hill_climbing(state)
{

}
state simulated_annaling(state)
{

}
