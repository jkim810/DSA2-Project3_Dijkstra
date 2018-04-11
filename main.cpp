#include "graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

int main() {
	string graphFileName, sourceVertex, outFileName, line;
	ifstream ifs;
	ofstream ofs;
	graph *g = new graph(100000);
	
	do{
		cout << "Enter a name of graph file: ";
		cin >> graphFileName;

		//open file
		ifs.open(graphFileName);
		//terminate program if file open error
		if (!ifs) {
			cerr << "Error opening graph file: " <<  graphFileName << endl;
		}
	}while(!ifs);

	//read in graph
	while (!ifs.eof()) {
		getline(ifs, line);
		if (line == "") break;
		istringstream iss(line);
		string source, destination, distString;
		int dist;
		getline(iss, source, ' ');
		getline(iss, destination, ' ');
		getline(iss, distString, ' ');
		iss.str("");
		istringstream (distString) >> dist;
		g->insert(source, destination, dist);
	}
	
	ifs.close();

	do{
		///input source vertex
		cout << "Enter a valid vertex id for the starting vertex: ";
		cin >> sourceVertex;
	} while(!g->contains(sourceVertex));

	//apply dijkstra's algorithm
	clock_t t1, t2;
	t1 = clock();
	g->dijkstra(sourceVertex);
	t2 = clock();
	cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << ((double) (t2 - t1)) / CLOCKS_PER_SEC << endl;
	
	do{
		cout << "Enter name of output file: ";
		cin >> outFileName;
		ofs.open(outFileName.c_str());
		//terminate program if file open error
		if (!ofs) {
			cerr << "Error opening output file: " <<  outFileName << endl;
		}
	}while(!ofs);

	ofs.close();

	g->printPaths(sourceVertex, outFileName);

}