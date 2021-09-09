#include "Graph.h"


Graph::Graph() {
	for (int i = 0; i < V; i++) {
		visited[i] = false;
		dist[i] = -1;
	}
}

const void Graph::addEdge(const int src, const int dest) {

	//in adjacency list, vertices that are connected with source and destination. since this is an undirected graph, we do a source <--> destination input instead of a source --> destination input

	adjLists[src].push_back(dest);
	adjLists[dest].push_back(src);
}

const bool Graph::getVisited(const int v) const{

	//returned result (true/false) depends on the bfs run. 

	return this->visited[v];
}

const int Graph::getDist(const int v) const{

	//returned result (dist from starting vertex after bfs) depends on the bfs run 

	return this->dist[v];
}

const int Graph::getVertexDegree(const int i) const{

	//in adjList[i].size() will give back the amount of lists connected to said list
	//which means if the size = 0, that means no list is connected to adjList[i], indicating a vertex without edges (aka an isolated vertex)

	return adjLists[i].size(); 
}

void Graph::BFS(const int startVertex){
	//initializing arrays

	std::list<int> queue; //creating queue

	//setting starting vertex
	visited[startVertex] = true;
	dist[startVertex] = 0;
	queue.push_back(startVertex);

	std::list<int>::iterator i; //starting the iterator


	//the actual BFS algorithm
	while (!queue.empty()) {
		int currVertex = queue.front();
		queue.pop_front();

		for (i = adjLists[currVertex].begin(); i != adjLists[currVertex].end(); ++i) {
			int adjVertex = *i;
			if (!visited[adjVertex]) { //if current vertex has neighboors, update accordingly
				visited[adjVertex] = true;
				dist[adjVertex] = dist[currVertex] + 1;
				queue.push_back(adjVertex); //add new vertices to queue
			}
		}
	}
}
