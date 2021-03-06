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


void greedyKnapsack(knapsack &k);
void orderKnapsack(knapsack &k, vector<int> &items);

int main()
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	string inputName = "knapsack1024";
	fileName = inputName + ".input";
	cout << "file name is " << fileName << endl;

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

		greedyKnapsack(k);
		string output = "/Users/patri/Desktop/Proj2-Git/GreedyAlgorithmsProject2/Output Files/" + inputName + ".output";
		cout << "output name: " << output << endl;


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


void greedyKnapsack(knapsack &k) {
	vector<int> items;
	items.resize(k.getNumObjects());
	for (int i = 0; i < k.getNumObjects(); i++) {
		items[i] = i;
	}

	//Order knapsack items by decreasing value 
	orderKnapsack(k, items, startTime, t);

	//Loop through items
	//Choose first/next item that does not have a cost more than the cost limit
	for (int i = 0; i < k.getNumObjects(); i++) {
		if ((k.getCost() + k.getCost(items[i])) <= k.getCostLimit()) {
			k.select(items[i]);
		}
	}
}

void orderKnapsack(knapsack &k, vector<int> &items) {
	int temp, j;

	for (int i = 1; i < k.getNumObjects(); i++) {
		temp = items[i];
		j = i - 1;

		while (j >= 0 && k.getRatio(items[j]) < k.getRatio(temp)) {
			items[j + 1] = items[j];
			j = j - 1;
		}
		items[j + 1] = temp;
	}
}