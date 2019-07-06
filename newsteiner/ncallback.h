#pragma once

#ifndef CALLBACK_HEAD
#define CALLBACK_HEAD

#include <ilcplex/ilocplex.h>

#include "ngraph.h"
#include "nseparation.h"
#include "nsmp.h"

using namespace std;

// Lazy constraint for NS
class NS_StrongCompLazyCallbackI : public IloCplex::LazyConstraintCallbackI {
   private:
    // Member name suffixed by _cb
    shared_ptr<Graph> G_cb;
    map<INDEX, NODE> map_k_rk_cb;
    map<NODE_INDEX_PAIR, IloNumVar> map_i_k_xik_cb;

   public:
    ILOCOMMONCALLBACKSTUFF(NS_StrongCompLazyCallback)

    // Initializer list
    NS_StrongCompLazyCallbackI(IloEnv env_, shared_ptr<Graph> G_,
                               map<INDEX, NODE> map_k_rk_,
                               map<NODE_INDEX_PAIR, IloNumVar> map_i_k_xik_)
        : IloCplex::LazyConstraintCallbackI(env_),
          G_cb(G_),
          map_k_rk_cb(map_k_rk_),
          map_i_k_xik_cb(map_i_k_xik_) {}

    void main();
};

// NS_StrongCompLazyCallback() returns a IloCplex::Callback class
IloCplex::Callback NS_StrongCompLazyCallback(
    IloEnv env_, shared_ptr<Graph> G_, map<INDEX, NODE> map_k_rk_,
    map<NODE_INDEX_PAIR, IloNumVar> map_i_k_xik_) {
    return (IloCplex::Callback(new (env_) NS_StrongCompLazyCallbackI(
        env_, G_, map_k_rk_, map_i_k_xik_)));
}

void NS_StrongCompLazyCallbackI::main() {  // Implement
    LOG << "NS_StrongCompLazyCallbackI::main() ..." << endl;

    IloEnv env = getEnv();  // Inherited function from base CallbackI

    vector<IloExpr> vector_cutLhs;

    map<NODE_INDEX_PAIR, double> map_i_k_Vxik;
    // Assign values to map_i_k_Vxik
    NODE_INDEX_PAIR pair_i_k;
    for (int k = 1; k <= G_cb->get_P(); k++) {  // for each k
        pair_i_k.second = k;
        for (auto i : G_cb->get_map_k_Vk().at(k)) {  // for each node i in V_k
            pair_i_k.first = i;
            map_i_k_Vxik.at(pair_i_k) = getValue(map_i_k_xik_cb.at(pair_i_k));
        }
    }

    NS_separate_sc(env, G_cb, map_i_k_xik_cb, map_i_k_Vxik, map_k_rk_cb,
                   vector_cutLhs);

    for (IloExpr cut_lhs : vector_cutLhs) {
        try {
            LOG << (cut_lhs >= 1) << endl;
            add(cut_lhs >= 1);
        } catch (IloException e) {
            cout << "Cannot add cut" << endl;
            cout << e << endl;
        }
    }
}

#endif  // !CALLBACK_HEAD