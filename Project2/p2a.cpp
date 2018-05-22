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


void greedyKnapsack(knapsack &k, int t);
void orderKnapsack(knapsack &k, vector<int> &items, clock_t startT, int t);

int main()
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "knapsack48.input";

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

		greedyKnapsack(k, 600);
		string output = "/Users/Cassie/source/knapsack48.output";

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


void greedyKnapsack(knapsack &k, int t) {
	clock_t startTime = clock();

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
		if (((float)(clock() - startTime) / CLOCKS_PER_SEC) >= t) {
			return;
		}
	}

	//cout << "Total value: " << k.getValue() << ", Total cost: " << k.getCost() << endl;
	//for (int m = 0; m < k.getNumObjects(); m++) {
	//	cout << "Item " << m << " selected: " << k.isSelected(m) << endl;
	//}

}

void orderKnapsack(knapsack &k, vector<int> &items, clock_t startT, int t) {
	int temp, j;

	for (int i = 1; i < k.getNumObjects(); i++) {
		temp = items[i];
		j = i - 1;

		while (j >= 0 && k.getRatio(items[j]) < k.getRatio(temp)) {
			items[j + 1] = items[j];
			j = j - 1;

			if (((float)(clock() - startT) / CLOCKS_PER_SEC) >= t) {
				return;
			}
		}
		items[j + 1] = temp;
		if (((float)(clock() - startT) / CLOCKS_PER_SEC) >= t) {
			return;
		}
	}

	//for (int m = 0; m < k.getNumObjects(); m++) {
	//	cout << "Item " << items[m] << ", Ratio = " << k.getRatio(items[m]) << endl;
	//}
}