#include <ilcplex/ilocplex.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "ngraph.h"
#include "nreader.h"
#include "nsmp.h"

ILOSTLBEGIN

int main(int argc, char** argv) {
    string file_name =
        "D:\\GitHub\\Repo\\SMP\\test\\data\\random_graph\\plan_random\\group_"
        "1\\dataset1_1_1_1\\animal_2.txt";
    LOG << file_name << endl;

    // Read graph into G_cb:
    Reader myReader;
    shared_ptr<Graph> G = make_shared<Graph>();
    myReader.read_graph(file_name, G);

    // Build model:
    IloEnv env;
    SmpSolver smp_solver = SmpSolver(env, G);

    smp_solver.add_primal_node_var();
    smp_solver.add_partition_node_var();

    smp_solver.add_objective();

    smp_solver.add_con_2();
    smp_solver.add_con_40();
    smp_solver.add_con_41();
    smp_solver.add_con_43();

    smp_solver.add_callback();

    smp_solver.solve();

    env.end();
}