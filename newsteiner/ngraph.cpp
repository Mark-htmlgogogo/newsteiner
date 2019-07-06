#include "ngraph.h"

///////////////////////
////// Base graph /////
///////////////////////

// Add node i_ to V
void BaseGraph::add_node(NODE i_) { V.insert(i_); }

// Add u_ to N(v_), v_ to N(u_)
// Add arc (u_,v_) and (v_,u_) to E
void BaseGraph::add_arc(NODE u_, NODE v_) {
    map_i_Ni[u_].insert(v_);
    map_i_Ni[v_].insert(u_);

    E.insert(ARC(u_, v_));
    E.insert(ARC(v_, u_));
}

// DFS: mark nodes in C(i) as comp_index in map_i_Comp
void BaseGraph::dfs(NODE i_, COMP_INDEX comp_index_,
                    map<NODE, COMP_INDEX> map_i_Comp_) {
    // Mark the current node as comp_index_
    map_i_Comp_.at(i_) = comp_index_;

    // Recur for all the nodes adjacent to this node
    for (auto j : map_i_Ni.at(i_)) {
        if (map_i_Comp_.at(j) != comp_index_)
            BaseGraph::dfs(j, comp_index_, map_i_Comp_);
    }
}

int BaseGraph::connected_components(map<NODE, COMP_INDEX> map_i_Comp_) {
    // Initialize all labels to 0
    for (auto i : V) {  // for each node i in V
        map_i_Comp_[i] = 0;
    }

    COMP_INDEX comp_index = 1;
    for (auto i : V) {                 // for each node i in V
        if (map_i_Comp_.at(i) == 0) {  // for a unvisited node i
            dfs(i, comp_index, map_i_Comp_);
            comp_index++;
        }
    }

    return (comp_index - 1);
}

// Find node neighbors of set_input in graph
void BaseGraph::neighbors_of_nodeSet(NODE_SET set_output, NODE_SET set_input) {
    for (auto i : set_input) {           // for each i in set_input
        for (auto j : map_i_Ni.at(i)) {  // for each j adjacent to i
            if (set_input.find(j) !=
                set_input.end()) {  // if j is not in set_input
                set_output.insert(j);
            }
        }
    }
}

///////////////////////
/////Graph for SMP/////
///////////////////////

// Set weight of node i as map_i_ci[i] = ci
void Graph::add_weight(NODE i, double ci) { map_i_ci[i] = ci; }

// Set totoal number of partitions as P_
void Graph::set_index_max(INDEX_MAX P_) { P = P_; }

// Add node i to V_k in map_k_Vk
void Graph::add_partition_node(INDEX k_, NODE i_) { map_k_Vk[k_].insert(i_); }

// Add terminal t_ in partition k_ to T and T_k
void Graph::add_partition_terminal(NODE t_, INDEX k_) {
    T.insert(t_);
    map_k_Tk[k_].insert(t_);
}

// Extract partition subgrpahs from G_cb, and store them in map_k_Gk
void Graph::extract_partitions() {
    for (int k = 1; k <= this->get_P(); k++) {  // for each k
        NODE_SET V_k = this->get_map_k_Vk().at(k);
        auto G_k = make_shared<BaseGraph>();

        for (auto i : V_k) {  // for each i in V_k
            // add i to G_k
            G_k->add_node(i);

            for (auto j :
                 this->get_map_i_Ni().at(k)) {   // for each adjacent node of i
                if (V_k.find(j) != V_k.end()) {  // if j is in V_k, add arc(i,j)
                    G_k->add_arc(i, j);
                }
            }
        }
    }
}

/////////////////////////////////////
/////Utility Functions for graph/////
/////////////////////////////////////
