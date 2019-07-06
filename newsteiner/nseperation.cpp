#include "nseparation.h"

// Strong-component separation for integral solution in NS
bool NS_separate_sc(IloEnv env_, const shared_ptr<Graph> G_,
                    const map<NODE_INDEX_PAIR, IloNumVar>& map_i_k_xik_,
                    const map<NODE_INDEX_PAIR, double>& map_i_k_Vxik_,
                    // const map<NODE, IloNumVar>& map_i_xi_,
                    // const map<NODE, double>& map_i_Vxi_,
                    const map<INDEX, NODE>& map_k_rk_,
                    vector<IloExpr>& vector_cutLhs_) {
    LOG << "NS_separate_sc()..." << endl;

    bool ret = false;

    // Build incumbent graph
    map<INDEX, shared_ptr<BaseGraph>> map_k_incumbentGk =
        build_all_partitionGraphs(G_, map_i_k_Vxik_);

    // For each incumbent_Gk£º get C(r_k), A(C(r_k)) and construct cuts
    NODE_INDEX_PAIR pair_i_k;
    for (int k = 1; k <= G_->get_P(); k++) {  // for each k in P
        shared_ptr<BaseGraph> incumbent_Gk = map_k_incumbentGk.at(k);

        // Find all strong connected components in incumbent_Gk
        map<NODE, COMP_INDEX> map_i_Comp;
        int comp_max = incumbent_Gk->connected_components(map_i_Comp);

        // If there is only one connected component, no operation on this
        // partition
        if (comp_max == 1) continue;
        LOG << "comp_max = " << comp_max << endl;

        // Get C(r_k): the connected component containing root r
        NODE r_k = map_k_rk_.at(k);  // get r_k
        COMP_INDEX comp_root = map_i_Comp.at(r_k);
        NODE_SET C_rk;  // the connected component containing root r
        bool result = retrieve_by_value(C_rk, map_i_Comp, comp_root);

        // Get A(C(r_k)): adjacent nodes of r_k which are not in C(r_k)
        shared_ptr<BaseGraph> G_k = make_shared<BaseGraph>();  // G_cb[V_k]
        G_k = G_->get_map_k_Gk().at(k);
        NODE_SET A_C_rk;  // neighbors of C_rk
        if (result) {     // if C_rk is not empty
            G_k->neighbors_of_nodeSet(A_C_rk, C_rk);
        } else {
            cout << "ERROR: C_r" << k << " is empty!" << endl;
        }

        // Construct cuts via A(C(r_k))
        IloExpr ns_sc_cutL(env_);
        pair_i_k.second = k;
        for (auto i : A_C_rk) {  // for each node i in  A(C(r_k))
            pair_i_k.first = i;
            ns_sc_cutL += (map_i_k_xik_.at(pair_i_k));
        }

        // Add cut to vector_cutLhs_
        vector_cutLhs_.push_back(ns_sc_cutL);

        ret = true;
    }
    return ret;
}

// Build all partition graphs out from incumbent solutions of x_i^k
map<INDEX, shared_ptr<BaseGraph>>& build_all_partitionGraphs(
    const shared_ptr<Graph> G_,
    const map<NODE_INDEX_PAIR, double>& map_i_k_Vxik_) {
    LOG << "build_all_partitionGraphs()..." << endl;

    map<INDEX, shared_ptr<BaseGraph>> map_k_incumbentGk_;

    for (int k = 1; k <= G_->get_P(); k++) {  // for each k in P
        map_k_incumbentGk_[k] =
            build_single_partitionGraph(G_, map_i_k_Vxik_, k);
    }

    return map_k_incumbentGk_;
}

// Build a BaseGraph on partition k out from map_i_k_Vxik
shared_ptr<BaseGraph> build_single_partitionGraph(
    shared_ptr<Graph> G_, const map<NODE_INDEX_PAIR, double>& map_i_k_Vxik_,
    INDEX k_) {
    LOG << "build_single_partitionGraph()..." << endl;

    shared_ptr<BaseGraph> incumbent_Gk =
        make_shared<BaseGraph>();  // incumbent partition graph

    NODE_INDEX_PAIR pair_i_k;
    pair_i_k.second = k_;

    // Add nodes i unless x_i^k = 1 for each incumbent partition graph k_
    for (NODE i : G_->get_map_k_Vk().at(k_)) {  // for each i in V_k
        pair_i_k.first = i;
        if (map_i_k_Vxik_.at(pair_i_k) > (1 - TOL)) {
            incumbent_Gk->add_node(i);
        }
    }

    const NODE_SET& V_incumbent_Gk =
        incumbent_Gk->get_V();  // node set of incumbent_Gk

    shared_ptr<BaseGraph> G_k = make_shared<BaseGraph>();  // G_cb[V_k]
    G_k = G_->get_map_k_Gk().at(k_);

    // Add arc e  unless the values of both endpoints of e is selected
    for (ARC e : G_k->get_E()) {  // for each arc e in E of G_k
        if (V_incumbent_Gk.find(e.first) != V_incumbent_Gk.end() &&
            V_incumbent_Gk.find(e.second) !=
                V_incumbent_Gk.end()) {  // if the values of both endpoints of e
                                         // is selected
            incumbent_Gk->add_arc(e.first, e.second);
        }
    }

    return incumbent_Gk;
}
