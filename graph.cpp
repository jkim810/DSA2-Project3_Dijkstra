#include "graph.h"

graph::graph(int i)
{
	capacity = i;
	size = 0;
	h = new hashTable(i);
}

//insertion of source vertex, destination vertex, and distance
//Happens at the read in phase
//distance should be parsed into integer before input
int graph::insert(const string &sID, const string &dID, int dist) {
	//insertion of a vertex
	vertex *s;
	bool b;
	//if vertex does not exist, insert vertex
	if (!h->contains(sID)) {
		s = new vertex(sID);
		node_list.push_back(s);
		h->insert(sID, node_list.back());
		size++;
	}
	if (!h->contains(dID)) {
		s = new vertex(dID);
		node_list.push_back(s);
		h->insert(dID, node_list.back());
		size++;
	}

	s = static_cast<vertex *> (h->getPointer(sID, &b));

	//insertion of an edge
	vertex *d = new vertex(dID, dist, false, s);
	s->adj.push_back(d);
	return 0;
}

//tracks the shortest path from a source vertex to all other verticies
void graph::dijkstra(const string &sID) {
	vertex *s;
	bool b;
	string s1;
	int i, key;
	heap queue(capacity);
	if (!h->contains(sID)) return;
	for (vertex *itr: node_list) {
		i = queue.insert(itr->id, itr->dist, itr);
		if (i == 1) cerr << "heap full" << endl;
		if (i == 2) cerr << "id exists" << endl;
	}

	s = static_cast<vertex *> (h->getPointer(sID, &b));
	if (!b) return;

	s->dist = 0;
	i = queue.setKey(sID, 0);
	if(i==1) cerr << "Set Key failed" << endl;
	
	int cnt = 1;
	vertex *v;
	while (!queue.deleteMin(NULL, &key, &v)) {	
		//retrieve current shortest distance we see
		for (vertex *itr: v->adj) {
			vertex *w = static_cast<vertex *> (h->getPointer(itr->id, &b));
			if (!b) return;
			//update distance whenever we find a shorter distance
			if (v->dist + itr->dist < w->dist) {
				w->dist = v->dist + itr->dist;
				w->path = v;
				int j = queue.setKey(w->id, w->dist);
				if (j == 1) cerr << "Set Key failed" << endl;
			}
		}
		cnt++;
		v->known = true;
	}

}

bool graph::contains(const string &sid){
	return h->contains(sid) ? true:false;
}

//prints the shortest path from a source vertex to all other vertices
void graph::printPaths(const string &sID, const string &outFileName) {
	string result;
	//write path result
	ofstream ofs(outFileName.c_str());
	if (!ofs) {
		cerr << "failed to open " << outFileName << endl;
		return ;
	}
	if (h->contains(sID)) {
		bool b;
		vertex *s = static_cast<vertex *> (h->getPointer(sID, &b));
		for (vertex *itr: node_list) {
			string line = "";
			string path = "";
			line = line + itr->id + ": ";
			int dist = printPath(itr, path);
			path = " [" + path + "]";

			result = result + line;
			if (dist == INT32_MAX/2) {
				path = "NO PATH\n";
				result = result + path;
			}
			else {
				ostringstream ss;
				ss << dist;
				result = result + ss.str() + path + "\n";
			}
			ofs << result;
			result = "";
		}
	}

	ofs.close();

}

int graph::printPath(vertex *v, string &s) {
	if (v->path != NULL) {
		printPath(v->path, s);
		s = s + ", ";
	}
	s = s + v->id;
	return v->dist;
}