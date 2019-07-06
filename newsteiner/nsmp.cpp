#include "ncallback.h"
#include "ngraph.h"
#include "nsmp.h"

// Initialize env, model, cplex, objective and G_cb
SmpSolver::SmpSolver(IloEnv env_, shared_ptr<Graph> G_) {
    LOG << "SmpSolver() ..." << endl;
    // Initialize cplex structures:
    env = env_;
    model = IloModel(env);
    cplex = IloCplex(model);
    objective = IloObjective();

    // Graph related
    G = G_;
}

// Add primal node var x_i and stored in map_i_xi
// x_i \in {0,1} \forall i \in V
void SmpSolver::add_primal_node_var() {
    LOG << "add_primal_node_var() ..." << endl;

    char var_name[255];

    IloNumVar temp_var;

    for (auto i : G->get_V()) {  // for each i i V
        snprintf(var_name, 255, "x_%d", i);
        temp_var = IloNumVar(env, 0, 1, IloNumVar::Bool, var_name);

        map_i_xi.insert({i, temp_var});
    }
}

// Add partition node var x_i^k and stored in map_i_k_xik_cb
// x_i^k \forall k \in P, \forall i \in V_k
void SmpSolver::add_partition_node_var() {
    LOG << "add_partition_node_var()" << endl;

    char var_name[255];

    NODE_INDEX_PAIR pair_i_k;

    IloNumVar temp_var;

    for (int k = 1; k <= G->get_P(); k++) {  // for each k
        pair_i_k.second = k;
        for (auto i : G->get_map_k_Vk().at(k)) {  // for each V_k
            pair_i_k.first = i;

            snprintf(var_name, 255, "x_%d^%d", i, k);
            temp_var = IloNumVar(env, 0, 1, IloNumVar::Bool, var_name);

            map_i_k_xik.insert({pair_i_k, temp_var});
        }
    }
}

// Choose the beginning element in each T_k as r_k
void SmpSolver::set_root_nodes() {
    LOG << "set_root_nodes() ..." << endl;

    for (int k = 1; k <= G->get_P(); k++) {  // for each k
        auto begin_iterate = G->get_map_k_Tk().at(k).begin();
        map_k_rk[k] = *begin_iterate;
    }
}

// Add objective function: min \sum c_i*x_i
void SmpSolver::add_objective() {
    LOG << "add_objective()" << endl;

    IloExpr total_cost(env);

    IloNumVar temp_var;

    double obj_coeff = 0.0;

    for (auto i : G->get_V()) {  // for each i in V
        temp_var = map_i_xi.at(i);
        obj_coeff = G->get_map_i_ci().at(i);
        total_cost += temp_var * obj_coeff;
    }

    objective = IloObjective(env, total_cost, IloObjective::Minimize);
    model.add(objective);
}

// Add constraint (2): x_t = 1 \forall t \in T
void SmpSolver::add_con_2() {
    LOG << "add_con_2()" << endl;
    for (auto t : G->get_T()) {
        model.add(map_i_xi.at(t) >= 1);
        model.add(map_i_xi.at(t) <= 1);
    }
}

// Add constraint (40):
// x_i^k = 1 \forall k \in P, \forall i \in T_k
void SmpSolver::add_con_40() {
    LOG << "add_con_40()" << endl;

    NODE_INDEX_PAIR pair_i_k;

    for (int k = 1; k <= G->get_P(); k++) {  // for each k in P
        pair_i_k.second = k;
        for (auto i : G->get_map_k_Tk().at(k)) {  // for each i in T_k
            pair_i_k.first = i;
            model.add(map_i_k_xik.at(pair_i_k) >= 1);
            model.add(map_i_k_xik.at(pair_i_k) <= 1);
        }
    }
}

// Add constraint (41):
// x_i >= x_i^k \forall k \in P, \forall i \in V_k
void SmpSolver::add_con_41() {
    LOG << "add_con_41()" << endl;

    NODE_INDEX_PAIR pair_i_k;

    for (int k = 1; k <= G->get_P(); k++) {  // for each k in P
        pair_i_k.second = k;
        for (auto i : G->get_map_k_Vk().at(k)) {  // for each  i in V_k
            pair_i_k.first = i;
            model.add(map_i_xi.at(i) >= map_i_k_xik.at(pair_i_k));
        }
    }
}

// Add constraint (43):
// \sum N(i)^k >= 1,       \forall k \in P, \forall i \in T_k
// \sum N(i)^k >= 2*x_i^k, \forall k \in P, \forall i \in V_k\T_k
// N(i)^k is the nodes neighboring i in G_cb[V_k]
void SmpSolver::add_con_43() {
    LOG << "add_con_43()" << endl;

    for (int k = 1; k <= G->get_P(); k++) {  // for each k in P

        NODE_SET T_k = G->get_map_k_Tk().at(k);
        NODE_SET V_k = G->get_map_k_Vk().at(k);

        NODE_INDEX_PAIR pair_i_k;
        pair_i_k.second = k;
        NODE_INDEX_PAIR pair_j_k;
        pair_j_k.second = k;

        IloExpr sum_neighbors_i(env);

        for (auto i : V_k) {  // for each i in V_k

            // construct IloExpr \sum N(i)^k
            for (auto j : G->get_map_i_Ni().at(
                     i)) {  // for each neighbor node j of i in G_cb
                if (V_k.find(j) != V_k.end()) {  // if j in V_k
                    pair_j_k.first = j;
                    sum_neighbors_i += map_i_k_xik.at(pair_j_k);
                }
            }

            pair_i_k.first = i;
            // i in T_k or not
            if (T_k.find(i) != T_k.end()) {  // if i in T_k
                model.add(sum_neighbors_i >= 1);
            } else {
                model.add(sum_neighbors_i >= 2 * map_i_k_xik.at(pair_i_k));
            }
        }
    }
}

// Add LazyConstriant or UserCut into system
void SmpSolver::add_callback() {
    LOG << "add_callback()" << endl;

    cplex.use(NS_StrongCompLazyCallback(env, G, map_k_rk, map_i_k_xik));
}

// Solve model and print info
void SmpSolver::solve() {
    try {
        cplex.solve();
    } catch (IloException e) {
        cout << e << endl;
    }
}
