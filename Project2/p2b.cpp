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

typedef adjacency_list<vecS, vecS, undirectedS, VertexProperties, EdgeProperties> Graph;

int greedyColoring(Graph &g, int numColors);
void placeColor(Graph &g, Graph::vertex_iterator node, int &numConflicts, int numColors);
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

		numConflicts = greedyColoring(g, numColors);
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

int greedyColoring(Graph &g, int numColors) {
	int numConflicts = 0;
	//gets start and end vertex iterators (which allow you to access the vertex)
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	Graph::vertex_iterator firstNode = vItrRange.first;
	Graph::vertex_iterator lastNode = vItrRange.second;

	//assign all vertex colors to 0, meaning lack of color
	for (Graph::vertex_iterator vItr = firstNode; vItr != lastNode; ++vItr) {
		g[*vItr].color = 0;
	}

	//assign vertex colors using the greedy algorithm
	for (Graph::vertex_iterator vItr = firstNode; vItr != lastNode; ++vItr) {
		placeColor(g, vItr, numConflicts, numColors);
	}

	return numConflicts;
}


void placeColor(Graph &g, Graph::vertex_iterator node, int &numConflicts, const int numColors) {
	vector<int> conflicts;
	conflicts.resize(numColors + 1);

	// Get a pair containing iterators pointing to the beginning and end of the
	// list of nodes adjacent to node v
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> adjRange = adjacent_vertices(*node, g);

	// Adjacent node list is incorrect for our purposes
	// Loop over adjacent nodes in the graph
	for (Graph::adjacency_iterator Itr = adjRange.first; Itr != adjRange.second; ++Itr) {
		conflicts[g[*Itr].color] += 1;
	}

	int minConflicts = numColors + 1;
	int color; //variable to hold the color number

	//find minimum of conflicts vector
	//assign color corresponding to minimum number of conflicts
	for (int i = 1; i <= numColors; i++) {
		if (conflicts[i] < minConflicts) {
			minConflicts = conflicts[i];
			color = i;
		}
	}

	//place color
	g[*node].color = color;

	//check if this node contains the max conflicts
	if (minConflicts > numConflicts) {
		numConflicts++;
	}

	return;
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