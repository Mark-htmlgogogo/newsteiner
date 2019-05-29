#pragma once

#include <lemon/concepts/maps.h>
#include <lemon/connectivity.h>
#include <lemon/preflow.h>
#include <lemon/smart_graph.h>
#include <lemon/time_measure.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "graph.h"
#include "smp.h"
#include "type.h"

using namespace std;
using namespace lemon;

typedef SmartDigraph::Node LemonNode;
typedef SmartDigraph::Arc LemonArc;
typedef pair<LemonNode, LemonNode> LemonNode_Pair;

typedef ListDigraph::Node ListNode;
typedef ListDigraph::Arc ListArc;
typedef pair<ListNode, ListNode> ListNode_Pair;

void build_support_graph_Steiner(SmartDigraph& support_graph,
                                 map<NODE, LemonNode>& v_nodes,
                                 map<LemonNode, NODE>& rev_nodes,
                                 const map<NODE_PAIR, double>& xSol,
                                 std::shared_ptr<Graph>, INDEX k);

void build_cap_graph_Steiner(SmartDigraph& cap_graph,
                             SmartDigraph::ArcMap<double>& x_capacities,
                             map<NODE, LemonNode>& v_nodes,
                             map<LemonNode, NODE>& rev_nodes,
                             const map<NODE_PAIR, double>& xSol,
                             std::shared_ptr<Graph>, INDEX k);

void build_cap_graph_ns(ListDigraph& cap_graph,
                        ListDigraph::ArcMap<double>& x_capacities,
                        map<NODE, pair<ListNode, ListNode>>& v_nodes,
                        map<ListNode, NODE>& rev_nodes,
                        const map<pair<NODE, INDEX>, double>& xSol,
                        std::shared_ptr<Graph>, INDEX k,
                        const map<INDEX, NODE>& ns_root);

bool separate_sc_Steiner(
    IloEnv masterEnv, const map<pair<NODE_PAIR, INDEX>, double>& xSol,
    std::shared_ptr<Graph>,
    const map<pair<NODE_PAIR, INDEX>, IloNumVar>& edge_vars,
    vector<IloExpr>& cutLhs, vector<IloExpr>& cutRhs,
    vector<double>& violation);

bool separate_sc_ns(
    IloEnv masterEnv, const map<pair<NODE, INDEX>, double>& xSol,
    std::shared_ptr<Graph> G,
    const map<pair<NODE, INDEX>, IloNumVar>& partition_node_vars,
    vector<IloExpr>& cutLhs, vector<IloExpr>& cutRhs, vector<double>& violation,
    const map<INDEX, NODE>& ns_root, const map<NODE, double>& xSol_primal);

bool seperate_min_cut_Steiner(
    IloEnv masterEnv, const map<pair<NODE_PAIR, INDEX>, double>& xSol,
    std::shared_ptr<Graph>,
    const map<pair<NODE_PAIR, INDEX>, IloNumVar>& edge_vars,
    vector<IloExpr>& cutLhs, vector<IloExpr>& cutRhs, vector<double>& violation,
    const map<INDEX, NODE>& root, const map<NODE, IloNumVar>& primal_node_vars);

bool seperate_min_cut_ns(
    IloEnv masterEnv, const map<pair<NODE, INDEX>, double>& xSol,
    std::shared_ptr<Graph>,
    const map<pair<NODE, INDEX>, IloNumVar>& partition_node_vars,
    vector<IloExpr>& cutLhs, vector<IloExpr>& cutRhs, vector<double>& violation,
    const map<INDEX, NODE>& ns_root);