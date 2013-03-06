/*********************************
******                      ******
***      Courtney Strachan     ***
**     AI - Professor Kirlin    **
***      12/2012 - Qvals.h     ***
******                      ******
**********************************/
#pragma once

#include <vector>

struct Node
{
	char player;
	int state[3];
	int action[2];
	double qVal;

	Node(char playerIn, int stateIn[], int pileRemove, int numRemove);

	bool operator>(const Node &other) const; // Returns true if "other" Node can result from this state/action pair
	bool operator<(const int &other) const; // Returns true if this state/action pair results in a win/loss (compare against 0)
	bool operator==(const Node &other) const; // Compares all values of this node with the "other" to check if equal
	friend std::ostream& operator<<(std::ostream &out, Node &aNode); // Prints out state, action, and Q value
};

bool sortByVal(Node i, Node j);
bool sortByState(Node i, Node j);

class Qvals
{
private:
	std::vector<Node> nodes;
	std::vector<Node>::iterator it;

	void update(Node &node);

public:
	Qvals(int state[]);

	void generate(char player, int state[]);
	void play(int state[], int numTimes);
	void pickNextMove(char whoFirst, int state[]);

	void print();
};