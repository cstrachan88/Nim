/*********************************
******                      ******
***      Courtney Strachan     ***
**     AI - Professor Kirlin    **
***     12/2012 - main.cpp     ***
******                      ******
**********************************/
#include "Qvals.h"
#include <iostream>
#include <time.h> // For RNG
using namespace std;

void playerTurn(int curState[]);
void computerTurn(char whoFirst, int curState[], Qvals comp);
void playing(char playerAnswer, int curState[], Qvals comp);

/*
Need to add description of Nim and how to play
Add a little info about this project and q-learning
clean up code and add comments to this and qvals.cpp

*/

int main()
{
	srand((unsigned int)time(0)); // For use with RNG

	int start[3];
	int numPlay;

	cout << "Input the pile sizes:" << endl;
	for (int i=0; i<3; i++)
	{
		cout << "   Pile " << i << ": ";
		cin >> start[i];
	};
	cout << endl << "How many games should the computer play to learn Nim? (100,000 is recommended)" << endl;
	cout << "   Number of games: ";
	cin >> numPlay;

	cout << "\nThe computer is learning with " << numPlay << " games of Nim." << endl;
	cout << "The starting configuration is [" << start[0] << ", " << start[1] << ", " << start[2] << "]\n\n";

	Qvals gameMoves(start);
	gameMoves.play(start, numPlay);
	cout << endl;

// PLAYING AGAINST THE COMPUTER:
	char playerAnswer;

	cout << "Would you like to play against the computer? Yes(1) or No(2):" << endl;
	cout << "   Response: "; cin >> playerAnswer;
	if (playerAnswer == '2') 
	{
		cout << "\nGoodbye! ";
		return 0;
	}

	while (true)
	{
		int startCopy[3];
		copy(start, start + 3, startCopy);

		cout << "\nWould you like to go first(1) or second(2)?" << endl;
		cout << "   Response: "; cin >> playerAnswer;

		playing(playerAnswer, startCopy, gameMoves);

		cout << endl << endl << endl << "Would you like to play again? Yes(1) or No(2): ";
		cin >> playerAnswer;

		if (playerAnswer == '2') break;
	}

	return 0;
}

void playerTurn(int curState[])
{
	cout << "*********************" << endl;
	cout << "***               ***" << endl;
	cout << "**   Player Turn   **" << endl;
	cout << "***               ***" << endl;
	cout << "*********************" << endl;
	cout << "The game state is currently [" << curState[0] << ", " << curState[1] << ", " << curState[2] << "]." << endl;

	int removePile, numSticks;
	char playerAnswer;

	do {
		cout << "Which pile would you like to remove from? Pile(0), pile(1), or pile(2)?" << endl;
		cout << "   Response: ";
		cin >> playerAnswer;

		removePile = playerAnswer - '0';
	} while ((0 > removePile || removePile > 2) && (cout << "\nThat is an incorrect choice. Please try again.\n\n"));

	do {
		cout << "\nHow many sticks would you like to remove? (At most, you can remove " << curState[removePile] << ")" << endl;
		cout << "   Response: ";
		cin >> playerAnswer;

		numSticks = playerAnswer - '0';
	} while ((1 > numSticks || numSticks > curState[removePile]) && (cout << "\nThat is an incorrect choice. Please try again.\n\n"));

	cout << "\nYou are removing " << numSticks << " stick from pile " << removePile << ". Is this correct? Yes(1) or no(2)?" << endl;
	cout << "   Response: ";
	cin >> playerAnswer;
	if (playerAnswer == '2')
	{
		cout << endl << endl << "Don't mess up this time!" << endl << endl;
		playerTurn(curState);
		return;
	}

	curState[removePile] -= numSticks;
	cout << endl;
}
void computerTurn(char whoFirst, int curState[], Qvals comp)
{
	cout << "*********************" << endl;
	cout << "***               ***" << endl;
	cout << "**  Computer Turn  **" << endl;
	cout << "***               ***" << endl;
	cout << "*********************" << endl;
	cout << "The game state is currently [" << curState[0] << ", " << curState[1] << ", " << curState[2] << "]." << endl;
	
	//if whofirst is '1', computer is b (wants to min)
	char who = ((whoFirst == '1') ? 'A' : 'B');
	comp.pickNextMove(who, curState);
	cout << endl;
}
void playing(char playerAnswer, int curState[], Qvals comp)
{
	if (playerAnswer == '1')
	{
		cout << "\nYou are going first.\n" << endl;
		while (true)
		{
			playerTurn(curState);
			if (curState[0] + curState[1] + curState[2] <= 0)
			{
				cout << endl << endl << endl << "YOU LOST, YOU LOST" << endl << endl;
				break;
			}
			computerTurn(playerAnswer, curState, comp);
			if (curState[0] + curState[1] + curState[2] <= 0)
			{
				cout << endl << endl << endl << "YOU WON, YOU WON" << endl << endl;
				break;
			}
		}
	}
	else if (playerAnswer == '2')
	{
		cout << "\nThe computer is going first.\n" << endl;
		while (true)
		{
			computerTurn(playerAnswer, curState, comp);
			if (curState[0] + curState[1] + curState[2] <= 0)
			{
				cout << endl << endl << endl << "YOU WON, YOU WON" << endl << endl;
				break;
			}
			playerTurn(curState);
			if (curState[0] + curState[1] + curState[2] <= 0)
			{
				cout << endl << endl << endl << "YOU LOST, YOU LOST" << endl << endl;
				break;
			}
		}
	}
}