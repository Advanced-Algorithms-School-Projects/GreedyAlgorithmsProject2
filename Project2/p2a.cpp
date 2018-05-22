// Project 2a: Solving knapsack using a greedy algorithm
// Created by Patrick Hanbury and Cassandra Smith
// Submitted 5/25/2018
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

void exhaustiveKnapsack(knapsack &k, int t); //finds the best knapsack
void RKT(knapsack &k, knapsack &bestSol, int start, int numUnSelect, int unSelIndex, clock_t startT, int t);
void checkKnapsack(knapsack &k, knapsack &bestSol);
void softReset(knapsack &k, int currPos, int newPos);
void hardReset(knapsack &k, int numUnSelect);
void moveUnSelection(knapsack &k, int oldPos, int newPos);

int main()
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "knapsack1024.input";

	//cout << "Enter filename" << endl;
	//cin >> fileName;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		cout << "Reading knapsack instance" << endl;
		knapsack k(fin);

		exhaustiveKnapsack(k, 120);
		string output = "/Users/Cassie/source/knapsack1024.output";

		//cout << endl << "Best solution" << endl;
		k.printSolution(output);
	}

	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}

	system("pause");
}

//Function takes a knapsack input and a time input to exhaustively determine the best combination
//of items that maximizes the knapsack value while remaining under the cost limit.
//Exits function if time taken exceeds t.
void exhaustiveKnapsack(knapsack &k, int t) {
	clock_t startTime = clock();
	knapsack bestSol = knapsack(k);
	int numObjects = k.getNumObjects();

	//Checks if all selected knapsack is best
	checkKnapsack(k, bestSol);

	//Loops through changing number of unselected items
	for (int j = 1; j < numObjects; j++) {
		int start = 0;
		hardReset(k, j);

		RKT(k, bestSol, start, j, 0, startTime, t);
	}

	//Checks if all unselected is best
	hardReset(k, 8);
	checkKnapsack(k, bestSol);

	//Sets knapsack to best solution
	k = knapsack(bestSol);
}

//Recursively tests every knapsack combination and tracks the combination with the highest value.
//Exits the function if the time taken exceeds t.
void RKT(knapsack &k, knapsack &bestSol, int start, int numUnSelect, int unSelIndex, clock_t startT, int t) {
	//Exit function if time is exceeded
	if (((float)(clock() - startT) / CLOCKS_PER_SEC) >= t) {
		return;
	}

	//Calculates end point of current unselected item
	int end = k.getNumObjects() + unSelIndex - numUnSelect;

	for (int i = start; i <= end; i++) {
		checkKnapsack(k, bestSol);
		if (i == end) {
			//Resets current unselected item for the next iteration
			softReset(k, i, start + 1);
			return;
		}
		if ((i + 1) < k.getNumObjects()) {
			if (!k.isSelected(i + 1)) {
				//Moves the next unselected item
				RKT(k, bestSol, i + 1, numUnSelect, unSelIndex + 1, startT, t);
				//Exit function if time is exceeded
				if (((float)(clock() - startT) / CLOCKS_PER_SEC) >= t) {
					return;
				}
			}
			if (k.isSelected(i + 1)) {
				//Moves the current unselected item
				moveUnSelection(k, i, i + 1);
			}
		}
	}
}

//Checks if the given knapsack is valid and has a higher values than the current best knapsack
void checkKnapsack(knapsack &k, knapsack &bestSol) {
	if ((k.getCost() <= k.getCostLimit()) && (k.getValue() > bestSol.getValue())) {
		bestSol = knapsack(k);
	}
}

//Resets the given unselected item and all proceeding unselected item to the next start
//position for the next iteration
void softReset(knapsack &k, int currPos, int newPos) {
	int j = 0;
	for (int i = currPos; i < k.getNumObjects(); i++) {
		moveUnSelection(k, i, newPos + j);
		j++;
	}
	return;
}

//Resets the given number of unselected items to the beginning of the knapsack
void hardReset(knapsack &k, int numUnSelect) {
	for (int i = 0; i < numUnSelect; i++) {
		k.unSelect(i);
	}
	for (int i = numUnSelect; i < k.getNumObjects(); i++) {
		k.select(i);
	}
}

//Moves an unselected item to a new position
void moveUnSelection(knapsack &k, int oldPos, int newPos) {
	k.select(oldPos);
	k.unSelect(newPos);
}