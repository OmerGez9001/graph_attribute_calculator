#include "Graph.h"
#include<time.h>
#include<fstream>

Graph* build_random_graph(const double p, const int v){ //implemented the graph using adjacency list. 
	Graph *g = new Graph();
	int i, j;
	for (i = 0; i < v; i++){
		for (j = i + 1; j < v; j++) {
			if ((double)rand() / RAND_MAX <= p) {
				g->addEdge(i, j);
			}
		}
	}
	return g;
}

const int diameter(Graph& g) { 
	//before entering this function, i do a connectivity test. if the graph is connected, i enter this func and run bfs on each vertex besides the one i checked before with the connectivity (vertex 0)
	int i, j, DIAM = 1;
	for (i = 0; i < V; i++) {
		if (DIAM < g.getDist(i))
			DIAM = g.getDist(i);
	}
	for (i = 1; i < V; i++) {
		g.BFS(i);
		for (j = 0; j < V; j++) {
			if (DIAM < g.getDist(j))
				DIAM = g.getDist(j);
		}
	}
	return DIAM;
}

const int is_isolated(const Graph& g) { 
	//to find an isolated vertex, i need to find a vertex that it's degree is 0. 
	for (int i = 0; i < V; i++) {
		if (g.getVertexDegree(i) == 0)
			return 1;
	}
	return 0;
}

const int connectivity(Graph& g) { 
	//running bfs on a single vertex (for simplicity sake, i chose 0). the bfs run will updated my dist and visited array accordingly. if the bfs didn't visit a vertex, that means G is not connected.
	g.BFS(0);
	for (int i = 0; i < V; i++) {
		if (g.getVisited(i) == false)
			return 0;
	}
	return 1;
}

const void writing_to_csv(int attribute_v[], const double p_arr[], const short index)
{ // funtion to create a CSV file with the results of each Attribute
	std::ofstream file;
	int i;

	file.open("results.csv", std::ios::app);

	if (index == 1)
		file << "Probabilities - CONNECTIVITY:,";
	else if (index == 2)
		file << "Probabilities - DIAM = 2:,";
	else if (index == 3)
		file << "Probabilities - ISOLATED VERTEX:,";

	for (i = 0; i < 10; i++) //printing the probabilities according to the threholds
		file << p_arr[i] << ",";

	file << std::endl;

	file << "Amount of graphs that maintain the Attribute:,";
	for (i = 0; i < 10; i++) //printing the amount of graph that maintain the attribute
		file << attribute_v[i] << ",";

	file << std::endl;

	file << "Assessment of the probability that the feature exists:,";
	for (i = 0; i < 10; i++) //printing the Assessment of the probability that the feature exists
		file << attribute_v[i] / 500.0 << ",";

	file << std::endl << std::endl;
	file.close();
}


int main() {
	Graph *graph;
	int i, j, counter_2, counter_1[10]; 
	const double param_1[10] = { 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.008, 0.009, 0.01, 0.02 }; // for threshold 1
	const double param_2[10] = { 0.015, 0.035, 0.055, 0.075, 0.085, 0.115, 0.12, 0.125, 0.13, 0.14 }; // for threshold 2
	const double param_3[10] = { 0.0015, 0.0025, 0.0035, 0.0045, 0.0055, 0.0075, 0.0085, 0.0095, 0.015, 0.025 }; // for threshold 3

	srand((unsigned)time(NULL));


	//---------------------------------------------------------------------TEST 1 ---> CONNECTIVITY--------------------------------------------------------------------

	counter_2 = 0;

	for (i = 0; i < 10; i++) {
		counter_1[i] = 0;
		for (j = 0; j < 500; j++) {
			graph = build_random_graph(param_1[i], V); //building random graph

			counter_1[i] += connectivity(*graph); //adding result of connectivity 500 times in order to count (in batches of 500) how many graphs are connected

			delete graph; //deleting after every connectivity test since i do not need to keep the actual graph and it saves memory
		}
		counter_2 += counter_1[i]; //counter_2 represents the sum of all connected graphs of a single program run
		std::cout << "batch " << i + 1 << "- " << counter_1[i] << " out of 500 are connected" << std::endl;
	}
	std::cout << std::endl << "there's a total of " << counter_2 << " connected graphs" << std::endl;
	writing_to_csv(counter_1, param_1, 1); //the writing function
	std::cout << "---------------------------------------------------------------------------------------" << std::endl;


	//------------------------------------------------------------------------TEST 2 ---> DIAM--------------------------------------------------------------------------

	counter_2 = 0;

	for (i = 0; i < 10; i++) {
		counter_1[i] = 0; //reseting counter
		for (j = 0; j < 500; j++) {
			graph = build_random_graph(param_2[i], V); //same as above

			if (connectivity(*graph) == 0) { //checking connectivity first BEFORE entering the DIAM function

				std::cout << "Graph " << j + 1 << ": DIAM = inf" << std::endl; // if 0, graph is not conencted (aka equals INF)
			}
			else {
				counter_2 = diameter(*graph); //counter_2 now keeps the value returned from diameter in order to output it on the CMD
				std::cout << "Graph " << j + 1 << ": DIAM = " << counter_2 << std::endl;
				if (counter_2 == 2) 
					counter_1[i]++; //keeping count of how many times diam = 2 in batches of 500 (is used for the CSV file later)
			}
			delete graph; //same as above
		}
	}
	writing_to_csv(counter_1, param_2, 2); //same as above
	std::cout << "---------------------------------------------------------------------------------------" << std::endl;


	//-------------------------------------------------------------------------TEST 3 ---> ISOLATED VERTEX---------------------------------------------------------------------------

	counter_2 = 0;

	for (i = 0; i < 10; i++) {
		counter_1[i] = 0; //same as above
		for (j = 0; j <  500; j++) {
			graph = build_random_graph(param_3[i], V); //same as above
			counter_1[i] += is_isolated(*graph); //counting how many graphs (in batches of 500) have 1 or more isolated vertices 
			delete graph; //same as above
		}
		counter_2 += counter_1[i]; //works the same way as in the connectivity test
		std::cout << "batch " << i + 1 << "- " << counter_1[i] << " out of 500 has 1 or more isolated vertices" << std::endl;
	}
	std::cout << std::endl << "there are " << counter_2 << " graphs total with 1 or more isolated vertices" << std::endl;
	writing_to_csv(counter_1, param_3, 3); //same as above
	std::cout << "---------------------------------------------------------------------------------------" << std::endl;

	return 0;
}
