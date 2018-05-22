// Project 2b: Solving graph coloring using a greedy algorithm
// Created by Patrick Hanbury and Cassandra Smith
// Submitted 5/25/2018
//
// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <time.h>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

int exhaustiveColoring(Graph &g, int numColors, int t);
void RGT(Graph &g, Graph &bestG, int &bestGConflicts, int numColors, Graph::vertex_iterator start, clock_t startT, int t);
void checkConflicts(Graph &g, Graph &bestG, int &bestGConflicts);
void printSolution(Graph &g, int numConflicts, string filePath_output);

struct VertexProperties
{
	pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;
	bool visited;
	bool marked;
	int weight;
	int color;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
	int weight;
	bool visited;
	bool marked;
};

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.
{
	int n, e;
	int j, k;

	fin >> n >> e;
	Graph::vertex_descriptor v;

	// Add nodes.
	for (int i = 0; i < n; i++)
		v = add_vertex(g);

	for (int i = 0; i < e; i++)
	{
		fin >> j >> k;
		add_edge(j, k, g);  // Assumes vertex list is type vecS
	}
}

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].weight = w;
	}
}

int main()
{
	char x;
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "color192-8.input";

	//   cout << "Enter filename" << endl;
	//   cin >> fileName;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		cout << "Reading graph" << endl;
		Graph g;
		int numColors;
		int numConflicts = -1;
		fin >> numColors;
		initializeGraph(g, fin);

		cout << "Num nodes: " << num_vertices(g) << endl;
		cout << "Num edges: " << num_edges(g) << endl;
		cout << endl;

		numConflicts = exhaustiveColoring(g, numColors, 600);
		string output = "/Users/Cassie/source/color192-8.output";
		printSolution(g, numConflicts, output);
		system("pause");

	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
}

//Exhaustive calculates the least number of conflicts in a graph coloring given the
//number of colors and a graph. Exits function if the time taken exceeds t.
int exhaustiveColoring(Graph &g, int numColors, int t) {
	clock_t startTime = clock();

	//gets start and end vertex iterators (which allow you to access the vertex)
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	Graph::vertex_iterator firstNode = vItrRange.first;
	Graph::vertex_iterator lastNode = vItrRange.second;

	//assign all vertex colors to 0, meaning lack of color
	for (Graph::vertex_iterator vItr = firstNode; vItr != lastNode; ++vItr) {
		g[*vItr].color = 0;
	}

	Graph bestG = g;
	int bestGConflicts = num_edges(g);

	//Calculates least number of conflicts recursively
	RGT(g, bestG, bestGConflicts, numColors, firstNode, startTime, t);

	//Returns least number of conflicts and the graph coloring that corresponds to it
	g = Graph(bestG);
	return bestGConflicts;
}

//Recursively tests every coloring combination of a graph and tracks the combination with the least number of conflicts.
//Exits the function if the time taken exceeds t.
void RGT(Graph &g, Graph &bestG, int &bestGConflicts, int numColors, Graph::vertex_iterator start, clock_t startT, int t) {
	pair<Graph::vertex_iterator, Graph::vertex_iterator> range = vertices(g);

	//Exit function if time is exceeded
	if (((float)(clock() - startT) / CLOCKS_PER_SEC) >= t) {
		return;
	}
	for (int x = 1; x <= numColors; x++) {
		g[*start].color = x; //set color of graph for start node to x

							 //check if start is the last node
		if ((start + 1) >= range.second) {
			checkConflicts(g, bestG, bestGConflicts);
		}
		//otherwise call function recursively
		else {
			RGT(g, bestG, bestGConflicts, numColors, start + 1, startT, t);
		}

		//Exit function if time is exceeded
		if (((float)(clock() - startT) / CLOCKS_PER_SEC) >= t) {
			return;
		}
	}

	//Reset node to 0 meaning lack of color
	g[*start].color = 0;
	return;
}

//Compares a graph coloring to the current best graph coloring and replaces it
//if it has less conflicts.
void checkConflicts(Graph &g, Graph &bestG, int &bestGConflicts) {
	int conflicts = 0, color1, color2;
	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);

	//Adds up all conflicts
	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr) {
		color1 = g[target(*eItr, g)].color;
		color2 = g[source(*eItr, g)].color;

		if (color1 == color2) {
			conflicts++;
		}
	}

	//Compares conflicts to current least number of conflicts
	if (conflicts < bestGConflicts) {
		bestGConflicts = conflicts;
		bestG = Graph(g);
	}
}

//Prints the best graph coloring calculated
void printSolution(Graph &g, int numConflicts, string filePath_output) {
	ofstream myfile;
	myfile.open(filePath_output.c_str());

	myfile << "Total Conflicts: " << numConflicts << endl;

	for (int counter = 0; counter < num_vertices(g); counter++) {
		myfile << counter << ": " << g[counter].color << endl;
	}

	myfile.close();
}