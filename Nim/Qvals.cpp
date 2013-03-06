/*********************************
******                      ******
***      Courtney Strachan     ***
**     AI - Professor Kirlin    **
***     12/2012 - Qvals.cpp    ***
******                      ******
**********************************/
#include "Qvals.h"
#include <iostream>
#include <algorithm>

/**********************************
* Node Class - Member Definitions *
***********************************/
Node::Node(char playerIn, int stateIn[], int pileRemove, int numRemove)
{
	player = playerIn;
	std::copy(stateIn, stateIn + 3, state);
	action[0] = pileRemove;
	action[1] = numRemove;
	qVal = 0;
}

bool Node::operator>(const Node &other) const // Returns true if "other" Node can result from this state/action pair
{
	int temp[3];
	std::copy(this->state, this->state + 3, temp);
	temp[this->action[0]] -= this->action[1];

	if (this->player != other.player &&
		temp[0] == other.state[0] &&
		temp[1] == other.state[1] &&
		temp[2] == other.state[2])
		return true;
	else return false;
}

bool Node::operator<(const int &other) const // Returns true if this state/action pair results in a win/loss
{
	int temp[3];
	std::copy(this->state, this->state + 3, temp);
	temp[this->action[0]] -= this->action[1];

	if (temp[0] == other &&
		temp[1] == other &&
		temp[2] == other)
		return true;
	else return false;
}

bool Node::operator==(const Node &other) const // Prints out state, action, and Q value
{
	if (this->player == other.player &&
		this->state[0] == other.state[0] &&
		this->state[1] == other.state[1] &&
		this->state[2] == other.state[2] &&
		this->action[0] == other.action[0] &&
		this->action[1] == other.action[1])
		return true;
	else return false;
}

std::ostream& operator<<(std::ostream &out, Node &aNode)
{
	out << "Q[" << aNode.player << aNode.state[0] << aNode.state[1] << aNode.state[2]
		<< ", " << aNode.action[0] << aNode.action[1] << "] = " << aNode.qVal;
	return out;
}

/***************************************
* Node comparison function definitions *
****************************************/
bool sortByVal(Node i, Node j)
{
	return i.qVal > j.qVal;
}

bool sortByState(Node i, Node j)
{
	int iVal ((i.player = 'A') ? 0 : 100000);
	iVal += i.state[0] * 10000;
	iVal += i.state[1] * 1000;
	iVal += i.state[2] * 100;
	iVal += i.action[0] * 10;
	iVal += i.action[1];

	int jVal ((j.player = 'A') ? 0 : 100000);
	jVal += j.state[0] * 10000;
	jVal += j.state[1] * 1000;
	jVal += j.state[2] * 100;
	jVal += j.action[0] * 10;
	jVal += j.action[1];

	return iVal < jVal;
}

/***********************************
* Qvals Class - Member Definitions *
************************************/
Qvals::Qvals(int state[])
{
	generate('A', state);
}

void Qvals::generate(char player, int state[])
{
	for (int pile=0; pile<3; pile++)
	{
		for (int numRemove = state[pile]; numRemove > 0; numRemove--)
		{
			Node temp(player, state, pile, numRemove);
			int add = 0;

			for (it = nodes.begin(); it != nodes.end(); it++)
				if (temp == *it) add++;

			if (add == 0) nodes.push_back(temp);

			char newPlayer = ((player == 'A') ? 'B' : 'A');

			int newState[3];
			std::copy(state, state + 3, newState);
			newState[pile] -= numRemove;

			generate(newPlayer, newState);
		}
	}
}

void Qvals::play(int state[], int numTimes)
{
	std::vector<int> starters; // List to hold positions of all possible starting states
	Node temp('B', state, 0,0);
	for (int i=0; i<int(nodes.size()); i++)
		if (temp > nodes[i]) starters.push_back(i);

	int count = 0;
	while (count < numTimes)
	{
		update(nodes[starters[count%starters.size()]]); // Update from the starting states
		count++;
	}
	sort(nodes.begin(), nodes.end(), sortByVal);

	std::cout << "After using Q-learning, the states have been updated with the following values:" << std::endl;
	this->print();
}

void Qvals::pickNextMove(char whoFirst, int state[])
{ // If whoFirst is '1', then player is first, so comp is B (wants to minimize)
	std::vector<int> moves;
	
	Node temp(whoFirst, state, 0,0);
	for (int i=0; i<int(nodes.size()); i++)
	{
		if (temp > nodes[i]) 
			moves.push_back(i);
	}

	int choice = 0;
	if (whoFirst == 'A')
	{
		for (int i=1; i<int(moves.size()); i++)
		{
			if (nodes[moves[choice]].qVal > nodes[moves[i]].qVal)
				choice = i;
		}
	}
	else
	{
		for (int i=1; i<int(moves.size()); i++)
		{
			if (nodes[moves[choice]].qVal < nodes[moves[i]].qVal)
				choice = i;
		}

	}
	std::cout << "The computer has chosen to remove " << nodes[moves[choice]].action[1] << " stick(s) from pile " << nodes[moves[choice]].action[0] << ".\n";
	state[nodes[moves[choice]].action[0]] -= nodes[moves[choice]].action[1];
}

void Qvals::print()
{
	for (it = nodes.begin(); it != nodes.end(); it++)
		std::cout << "   " << *it << std::endl;
}

void Qvals::update(Node &node)
{
	if (node < 0)
	{
		int minMax ((node.player == 'A') ? -1 : 1);
		node.qVal = minMax*1000.0; // Actual equation: node.qVal + alpha * (minMax*1000 + gamma * 0 - node.qVal)
	}
	else
	{
		std::vector<int> possibles; // Holds the position of the next possible states
		for (int i = 0; i < int(nodes.size()); i++)
			if (node > nodes[i]) possibles.push_back(i);

		int	next = rand() % possibles.size();

		node.qVal = 0.9 * nodes[possibles[next]].qVal; // Actual equation: node.qVal + alpha * (0 + gamma * nextNode.qVal - node.qVal)
		update(nodes[possibles[next]]);
	}		
}