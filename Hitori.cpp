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
	char condition;//W for white & B for black & N for non that means the node was not checked yet
};
//matrix initialed to be solved -> hitori main table
struct state
{
	int size;
	vector <vector<node> > matrix;
};
//struct used in CSP and AC3
struct csp_struct {
	int x;
	int y;
	vector<char> domine = {'W','B','N'};
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
//Rule #2: black nodes do not touch each other vertically or horizontally
bool check_rule_two(state s, int x, int y)
{
	
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
	return result;

}

//Rule #3:all un-shaded (white) squares create a single continuous area T_T
bool check_rule_three(state s, int x, int y)
{

}
//checks if the chosen node's value is repeated
bool repeated(state s,int x,int y)
{
	node K = s.matrix[x][y];
	for (int i = 0; i < s.size; i++)
	{
		node current = s.matrix[i][y];
		if (current.value == K.value)
		{
			return false;

		}
	}
	for (int j = 0; j < s.size; j++)
			{
				node current = s.matrix[x][j];
				if (current.value==K.value)
				{
					return false;
				
				}
			}
	return true;
}
//returns all the possible states in next moves 
vector<state> successor(state s, int x,int y)
{
	vector<state> vec;
	/*int array[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	for (int i = 0; i < s.size; i++)
	{
		state temp = s;
		
		node current = temp.matrix[i][level];
		if (current.condition == 'N')
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
	}*/
	state temp = s;
	if (check_rule_one(s, x) == true && check_rule_one(s, y) == true)
	{
		temp.matrix[x][y].condition = 'W';
		vec.push_back(temp);
	}
	if (check_rule_two(s, x, y) == true && repeated(s,x,y)==true&& check_rule_three(s,x,y)==true)
	{
		temp.matrix[x][y].condition = 'B';
		vec.push_back(temp);
	}

	return vec;
}


state hill_climbing(state)
{

}
state simulated_annaling(state)
{

}
void AC3(state &s, vector<csp_struct> &csp_)
{
	for (int i = 0; i < csp_.size(); i++)
	{
		if (check_rule_one(s,csp_[i].x)==false)
			csp_[i].
	}
}
