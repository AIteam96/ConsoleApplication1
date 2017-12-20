#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdint>
#include <algorithm>

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
	int counter1 = 0;
	int counter2 = 0;
	int counter3 = 0;
	int counter4 = 0;
	if (x > 0) {
		if (x + 2 < s.size)
			if (s.matrix[x + 2][y].condition == 'B')
				counter1++;
		if (y + 1 < s.size && x + 1 < s.size)
			if (s.matrix[x + 1][y + 1].condition == 'B')
				counter1++;
		if (y - 1 > 0 && x + 1 < s.size)
			if (s.matrix[x + 1][y - 1].condition == 'B')
				counter1++;
		//////////////////////////////////////////////
		if (x - 2 > 0)
			if (s.matrix[x - 2][y].condition == 'B')
				counter3++;
		if (y + 1 < s.size && x - 1 > 0)
			if (s.matrix[x - 1][y + 1].condition == 'B')
				counter3++;
		if (y - 1 > 0 && x - 1 > 0)
			if (s.matrix[x - 1][y - 1].condition == 'B')
				counter3++;
		//////////////////////////////////////////////
		if (y - 2 > 0)
			if (s.matrix[x][y - 2].condition == 'B')
				counter4++;
		if (x + 1 < s.size && y - 1 > 0)
			if (s.matrix[x + 1][y - 1].condition == 'B')
				counter4++;
		if (x - 1 > 0 && y - 1 > 0)
			if (s.matrix[x - 1][y - 1].condition == 'B')
				counter4++;
		///////////////////////////////////////////////
		if (y = 2 < s.size)
			if (s.matrix[x][y + 2].condition == 'B')
				counter2++;
		if (x + 1 < s.size && y + 1 < s.size)
			if (s.matrix[x + 1][y + 1].condition == 'B')
				counter2++;
		if (x - 1 > 0 && y + 1 < s.size)
			if (s.matrix[x - 1][y + 1].condition == 'B')
				counter2++;
		/////////////////////////////////////////////
		if (counter1 != 3 || counter2 != 3 || counter3 != 3 || counter4 != 3)
			return true;// condition convert to black
		return false;
	}
	return false;
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
		if (check_rule_one(s, csp_[i].x) == false || check_rule_one(s, csp_[i].y) == false)
			csp_[i].domine.erase(std::remove(csp_[i].domine.begin(), csp_[i].domine.end(), 'W'), csp_[i].domine.end());
		if (check_rule_two(s, csp_[i].x, csp_[i].y) == false || repeated(s, csp_[i].x, csp_[i].y) == false || check_rule_three(s, csp_[i].x, csp_[i].y) == false)
			csp_[i].domine.erase(std::remove(csp_[i].domine.begin(), csp_[i].domine.end(), 'B'), csp_[i].domine.end());

	}
}
state CSP(state s)
{
	vector<csp_struct> csp_;
	for (int i = 0; i < s.size; i++)
	{
		for (int j = 0; j < s.size; j++)
		{
			csp_[i + j].x = i;
			csp_[i + j].y = j;
		}
	}
	int i=0, j=0;
	while (!csp_.empty())
	{
		csp_.po;
		AC3(s,csp_)
	}

}
state read()
{
	vector<vector<node> > vec(MAX, vector<node>(MAX));
	//initiate all condition to NONE
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			vec[i][j].condition == 'N';
		}
	}
	fstream inputfile("sample1.txt");
	if (inputfile.is_open()) {
		for (int i = 0; i < MAX; i++) {
			for (int j = 0; j < MAX; j++) {
				inputfile >> vec[i][j].value;
			}
		}
		inputfile.close();
		state s;
		s.matrix = vec;
		return s;
	}
	else
	{
		cout << "Couldn't read file!\n";
		exit(0);
	}
}