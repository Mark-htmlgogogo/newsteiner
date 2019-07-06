#pragma once

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iterator>
#include <vector>

#include "ngraph.h"

#define LOG cout
#define TOL 0.001

using namespace std;

bool NS_separate_sc(IloEnv env_, const shared_ptr<Graph> G_,
                    const map<NODE_INDEX_PAIR, IloNumVar>& map_i_k_xik_,
                    const map<NODE_INDEX_PAIR, double>& map_i_k_Vxik_,
                    // const map<NODE, IloNumVar>& map_i_xi_,
                    // const map<NODE, double>& map_i_Vxi_,
                    const map<INDEX, NODE>& map_k_rk_,
                    vector<IloExpr>& vector_cutLhs_);

map<INDEX, shared_ptr<BaseGraph>>& build_all_partitionGraphs(
    const shared_ptr<Graph> G_,
    const map<NODE_INDEX_PAIR, double>& map_i_k_Vxik_);

shared_ptr<BaseGraph> build_single_partitionGraph(
    shared_ptr<Graph> G_, const map<NODE_INDEX_PAIR, double>& map_i_k_Vxik_,
    INDEX k_);

// Retrieve all keys that have the same value in a map
template <typename K, typename V>
inline bool retrieve_by_value(set<K>& set_key, map<K, V> map_of_element,
                              V value) {
    bool ret = false;

    auto it = map_of_element.begin();

    while (it != map_of_element.end()) {
        if (it->second == value) {
            ret = true;
            set_key.insert(it->first);
        }
    }

    return ret;
}
