#pragma once

#include <map>
#include <memory>

#include "ntype.h"

///////////////////////
////// Base graph /////
///////////////////////

// Base class for graph, containing only nodes and arcs with NO weight
// dfs and connected_components are implemented also here.
class BaseGraph {
   public:
    BaseGraph(){};

    // Generate graph:
    void add_node(NODE i_);
    void add_arc(NODE u_, NODE v_);

    // Accessors:
    const NODE_SET& get_V() const { return V; }
    const ARC_SET& get_E() const { return E; }
    const map<NODE, NODE_SET>& get_map_i_Ni() const { return map_i_Ni; }

    // Utility functions for connected component
    void dfs(NODE i_, COMP_INDEX comp_index_,
             map<NODE, COMP_INDEX> map_i_Comp_);
    int connected_components(map<NODE, COMP_INDEX> map_i_Comp_);
    void neighbors_of_nodeSet(NODE_SET set_output, NODE_SET set_input);

   protected:
    NODE_SET V;
    ARC_SET E;
    map<NODE, NODE_SET> map_i_Ni;  // Ni: neighbors of i
};

///////////////////////
/////Graph for SMP/////
///////////////////////

// Multigprah Steiner graph
class Graph : public BaseGraph {
   public:
    Graph(){};

    // Generate graph
    void add_weight(NODE i, double ci);
    void set_index_max(INDEX_MAX P_);
    void add_partition_node(INDEX k_, NODE i_);
    void add_partition_terminal(NODE t_, INDEX k_);
    void extract_partitions();

    // Accessors:
    const INDEX_MAX& get_P() const { return P; }
    const NODE_SET& get_T() const { return T; }
    const map<NODE, double>& get_map_i_ci() const { return map_i_ci; }
    const map<INDEX, NODE_SET>& get_map_k_Vk() const { return map_k_Vk; }
    const map<INDEX, NODE_SET>& get_map_k_Tk() const { return map_k_Tk; }
    const map<INDEX, shared_ptr<BaseGraph>>& get_map_k_Gk() const {
        return map_k_Gk;
    }

   private:
    INDEX_MAX P;
    NODE_SET T;
    map<NODE, double> map_i_ci;
    map<INDEX, NODE_SET> map_k_Vk;
    map<INDEX, NODE_SET> map_k_Tk;
    map<INDEX, shared_ptr<BaseGraph>>
        map_k_Gk;  // induced subgrpah of V_k in G_cb
};
