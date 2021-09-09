#pragma once
#include <iostream>
#include <list>
#define V 1000

class Graph
{
private:
	//the actual graph
	std::list<int> adjLists[V]; 

	//BFS attributes
	bool visited[V];
	int dist[V];

public:

	Graph();
	~Graph(){}
	const int getDist(const int v) const; //for diam
	const bool getVisited(const int v) const; //for connectivity
	const int getVertexDegree(const int i) const; //for is_isolated
	const void addEdge(const int src, const int dest);  //adding edge
	void BFS(const int startVertex); //BFS

};
