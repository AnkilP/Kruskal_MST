
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

struct edge{
	int node1;
	int node2;
	double weight;

	edge(int node1, int node2, double weight): node1(node1), node2(node2), weight(weight) {}
};

struct edge_comparator
{
    inline bool operator() (const edge& struct1, const edge& struct2)
    {
        return (struct1.weight < struct2.weight);
    }
};

class Weighted_graph {
	private:
		static const double INF;
		double** graph;
		int num_nodes;
		int num_edges;

		std::vector< std::pair< double, std::pair<int,int> > > sorted_edges;

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
		bool edge_exists(int i, int j);

	// Friends

	friend std::ostream &operator<<( std::ostream &, Weighted_graph const & );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

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

Weighted_graph::~Weighted_graph() {
	for(int i = 0; i < num_nodes; ++i){
		delete[] graph[i];
	}
	delete[] graph; 
}

int Weighted_graph::degree(int u) const {
	int count = 0;
	for(int i =0; i < num_nodes; ++i){
		if(graph[u][i] != INF){
			count++;
		}
	}
	return count;
}

int Weighted_graph::edge_count() const {
	return num_edges;
}

bool Weighted_graph::erase_edge(int i, int j) {
	if(edge_exists(i,j) == false){
		return false;
	}
	graph[i][j] = INF;
	graph[j][i] = INF;

	--num_edges;
	return true;
}

void Weighted_graph::clear_edges() {
	for(int i = 0; i < num_nodes; ++i){
		for(int j = 0; j < num_nodes; ++j){
			graph[i][j] = INF;
		}
	}
}

bool Weighted_graph::insert_edge( int i, int j, double d ) {
	if(i > num_nodes - 1 || i < 0 || j > num_nodes - 1 || j < 0 || d <= 0){
		throw illegal_argument();
	}

	if(i == j && edge_exists(i,j)){
		return false;
	}

	graph[i][j] = d;
	graph[j][i] = d;

	//if(std::find(sorted_edges.begin().second, sorted_edges.end().second ))

	++num_edges;
	return true;
}

bool Weighted_graph::edge_exists(int i, int j){
	//Assumption: since the graph is undirected, 
	//if I find an edge going in one direction, 
	//then the arc going in the other direction also exists

	if(graph[i][j] == INF){
		return false;
	}
	return true;

}

std::pair<double, int> Weighted_graph::minimum_spanning_tree() const {
	Disjoint_set ds(this->num_nodes);
	//let's sort the edges 
	for(int i = 0; i < num_nodes; ++i){
		for(int j = 0; j < num_nodes; ++j){
			if(graph[i][j] != INF){
				sorted_edges.push_back(std::make_pair(graph[i][j], std::make_pair(i,j)));
			}
		} 
	}
	std::sort(sorted_edges.begin(), sorted_edges.end());


	return std::pair<double, int>( 0.0, 0 );
}

std::ostream &operator<<( std::ostream &out, Weighted_graph const &graph ) {
	

	return out;
}


#endif
