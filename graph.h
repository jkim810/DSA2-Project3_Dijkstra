#ifndef _GRAPH_H
#define  _GRAPH_H
#include "heap.h"
#include <list>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
class graph
{
private:
	//node for graphs
	//can act as both a vector and an edge
	class vertex {
	public:
		string id;
		int dist;
		bool known;
		vertex *path;
		//this list serves as edges
		list <vertex *> adj;
		vertex(string id, int dist = INT32_MAX/2, bool known = false, vertex *path = NULL): id(id), dist(dist), known(known), path(path) {};
	};

	//counter to used while printing paths
	int size;
	int capacity;
	//hashtable to efficiently find the given vertex
	hashTable *h;
	//list of all source vertices(nodes)
	list <vertex *> node_list;
	//recursive method for printpaths
	int printPath(vertex * v, string &s);

public:
	//initiate graph
	graph(int capacity);

	//insertion of source vertex, destination vertex, and distance
	//Happens at the read in phase
	//distance should be parsed into integer before input
	int insert(const string &sid, const string &did, int dist);

	//tracks the shortest path from a source vertex to all other verticies
	void dijkstra(const string &sid);

	//simple contains function that checks whether a vertex exists
	bool contains(const string &sid);

	//prints the shortest path from a source vertex to all other vertices
	void printPaths(const string &sid, const string &outputfile);

};

#endif