
#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#ifndef nullptr
#define nullptr 0
#endif


#include <limits>
#include "Exception.h"

using namespace std;

struct ll_entry; // represents each element of the linked list
struct set_info; // keeps track of the information (pointers to head and tail and the size of the set) of each set
//could we delete the above two lines?


struct ll_entry{ 
	int content;
	set_info* ptr_to_info; // ptr to the info entry of the corresponding set
	ll_entry* next;
};

struct set_info { 
	ll_entry* head;
	ll_entry* tail;
	int size;
};

class Disjoint_set {

private:
	ll_entry* nodes;
	set_info* sets;
	int set_counter;
	int initial_num_sets;
public:
	Disjoint_set(int);
	~Disjoint_set();
	int find_set(int) const;
	int num_sets() const;
	void union_sets(int, int);
};

Disjoint_set::Disjoint_set(int n) : nodes(new ll_entry[n]), 
sets (new set_info[n]), 
set_counter(n),
initial_num_sets(n) {
	for(int i = 0; i < n; ++i){
		nodes[i].content = i;
		nodes[i].ptr_to_info = &sets[i];
		nodes[i].next = nullptr;
		sets[i].head = &nodes[i];
		sets[i].tail = &nodes[i];
		sets[i].size = 1;
	}
}

Disjoint_set::~Disjoint_set() {
	delete[] nodes;
	delete[] sets;
}
int Disjoint_set::find_set(int item) const{
	if(item >= initial_num_sets || item < 0){
		throw illegal_argument();
	}
	return nodes[item].ptr_to_info->head->content; 
}

int Disjoint_set::num_sets() const {
	return set_counter;
}

void Disjoint_set::union_sets(int node_index1, int node_index2) {
	
	if (node_index1 == node_index2)
		return;
	
	set_info* si1 = nodes[node_index1].ptr_to_info;
	set_info* si2 = nodes[node_index2].ptr_to_info;

	// ni1: the index of the larger set, ni2: the index of the smaller index
	int ni1 = si1->size >= si2->size ? node_index1 : node_index2; 
	int ni2 = si1->size < si2->size ? node_index1 : node_index2;

	ll_entry * temp_node = &nodes[ni2];
	//iterate through the shorter list and modify the pointers

	nodes[ni1].ptr_to_info->tail->next = temp_node;
	nodes[ni1].ptr_to_info->tail = temp_node->ptr_to_info->tail;

	while (temp_node != nullptr) {
		temp_node->ptr_to_info = nodes[ni1].ptr_to_info;
		nodes[ni1].ptr_to_info->size += 1;
//		delete temp_node->ptr_to_info; // don't think I need this
		temp_node = temp_node->next;
	}
	set_counter--;
	// do we need to modify anything else?
	delete temp_node;
	// delete the set_info entry that no longer exists
}


#endif
