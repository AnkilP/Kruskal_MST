
/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  a293pate@edu.uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  Fall 2019
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    -
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    -
 *****************************************/

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include "Exception.h"
#include "Disjoint_sets.h"

using namespace std;

typedef std::pair<double, std::pair<int,int> > __pair;

class Weighted_graph {
	private:
		static const double INF;
		double** graph;
		int num_nodes;
		int num_edges;

		// Do not implement these functions!
		// By making these private and not implementing them, any attempt
		// to make copies or assignments will result in errors
		Weighted_graph( Weighted_graph const & );
		Weighted_graph &operator=( Weighted_graph );

		// your choice

	public:
		Weighted_graph( int = 10 );
		~Weighted_graph();
		
		int degree( int ) const;
		int edge_count() const;
		std::pair<double, int> minimum_spanning_tree() const;

		bool insert_edge( int, int, double );
		bool erase_edge( int, int );
		void clear_edges();
		bool edge_exists(int , int );

	// Friends

	friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

/*
Constructs a weighted undirected graph with n vertices (by default 10).
Assume that initially there are no connections in the graph (edges will be inserted with the “insert”
method). Throw an illegal_argument exception if the argument is less than 0.
*/

Weighted_graph::Weighted_graph(int n ) {
	graph = new double*[n];
	for(int i = 0; i < n; ++i){
		graph[i] = new double[n];
		for(int j = 0; j < n; ++j){
			graph[i][j] = INF;
		}
	} //this is a matrix not a list
	num_edges = 0;
	num_nodes = n;
}

//Cleans up any allocated memory.

Weighted_graph::~Weighted_graph() {
	for(int i = 0; i < num_nodes; ++i){
		delete[] graph[i];
	}
	delete[] graph; 
}

/*
Returns the degree of the vertex n. Throw an illegal_argument
exception if the argument does not correspond to an existing vertex.
Assumptions: we have an undirected graph
*/

int Weighted_graph::degree(int u) const {
	if(u >= num_nodes || u < 0){
		throw illegal_argument();
	}

	int count = 0;
	for(int i = 0; i < num_nodes; i++){
		if(graph[u][i] != INF || graph[i][u] != INF){
			count += 1;
		}
	}
	return count;
}

//Returns the number of edges in the graph.

int Weighted_graph::edge_count() const {
	return num_edges;
}

/*
If an edge between nodes i and j exists, remove the edge. In this
case or if i equals j return true. Otherwise, if no edge exists, return false. If i or j are outside the
range of [0..n-1], throw an illegal_argument exception.

*/

bool Weighted_graph::erase_edge(int i, int j) {
	if(edge_exists(i,j) == false){
		return false;
	}
	graph[i][j] = INF;
	graph[j][i] = INF;

	--num_edges;
	return true;
}

//Removes all the edges from the graph.

void Weighted_graph::clear_edges() {
	for(int i = 0; i < num_nodes; ++i){
		for(int j = 0; j < num_nodes; ++j){
			graph[i][j] = INF;
		}
	}
	num_edges = 0;
}

/*
If i equals j and are in the graph, return false. Otherwise,
either add a new edge from vertex i to vertex j or, if the edge already exists, update the weight and
return true. Recall that the graph is undirected. If i or j are outside the range of [0..n-1] or if the
weight w is less than or equal to zero, throw an illegal_argument exception.
*/

bool Weighted_graph::insert_edge( int i, int j, double d ) {
	if(i > num_nodes - 1 || i < 0 || j > num_nodes - 1 || j < 0 || d <= 0){
		throw illegal_argument();
	}

	if(i == j){
		return false;
	}

	if(graph[i][j] == INF){
		num_edges++;
	}

	graph[i][j] = d;
	
	return true;
}

/*
Checks if the edge denoted by vertex i to vertex j exists. 
Since this is an undirected graph but the implementation uses 
a directed graph data structure, I assign this function to make it
appear as if the graph is undirected. 
Throws illegal argument if the indices are outside of the appropriate bounds.
*/

bool Weighted_graph::edge_exists(int i, int j){
	//Assumption: since the graph is undirected, 
	//if I find an edge going in one direction, 
	//then the arc going in the other direction also exists

	if(i > num_nodes - 1 || i < 0 || j > num_nodes - 1 || j < 0){
		throw illegal_argument();
	}

	if(graph[i][j] == INF && graph[j][i] == INF){
		return false;
	}
	return true;

}

/*
Uses Kruskal’s algorithm to find the
minimum spanning tree. It returns the weight of the minimum spanning tree and the number of
edges that were tested for adding into the minimum spanning tree. Use Disjoint sets to ensure that
the Kruskal’s algorithm does not form loops in the tree.
*/



std::pair<double, int> Weighted_graph::minimum_spanning_tree() const {
	Disjoint_set * ds = new Disjoint_set(num_nodes);
	
	std:vector<__pair> sorted_edges;

	//let's sort the edges 
	for(int i = 0; i < num_nodes; ++i){
		for(int j = 0; j < num_nodes; ++j){
			if(graph[i][j] != INF){
				sorted_edges.push_back(std::make_pair(graph[i][j], std::make_pair(i,j)));
			}
		} 
	}

	std::sort(sorted_edges.begin(), sorted_edges.end());
/*
	////Only use when testing for sorted list.

	// for(std::vector<__pair>::iterator it = sorted_edges.begin(); it != sorted_edges.end(); ++it) {
    // 	std::cout << (*it).first << ' ' << (*it).second.first << ' ' << (*it).second.second << std::endl; 
	// }
*/
	__pair temp_edge;
	int edges = 0;
	double weight = 0;
	int tries = 0;
	while(edges < num_nodes - 1){
		temp_edge = *(sorted_edges.begin());
		sorted_edges.erase(sorted_edges.begin());
		/*
			if the parent of one of the vertices of this edge is the same 
			as the parent of the other vertex of this edge, then the 
			sets are not disjoint - there is a cycle.
			if not, we union the sets and adjust appropriate meta variables
		*/
		if(ds->find_set(temp_edge.second.first) != ds->find_set(temp_edge.second.second)){
			edges++;
			ds->union_sets(temp_edge.second.first, temp_edge.second.second);
			weight += temp_edge.first;
		}
		tries++;
	}

	delete ds;

	return std::pair<double, int>( weight, tries );
}

/*
	Overloaded cout function that I didn't end up using...
*/

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	for(int i = 0; i < graph.num_nodes; ++i){
		for(int j = 0; j < graph.num_nodes; ++j){
			out << graph.graph[i][j] << ' ';
		}
		out << std::endl;
	}
	out <<flush;
	// out << "Is this working";
	return out;
}


#endif
