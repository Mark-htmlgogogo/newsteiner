#pragma once

#ifndef SMPNS_HEAD
#define SMPNS_HEAD

#define LOG cout

#include <ilcplex/ilocplex.h>
#include <map>
#include <iostream>

#include "ntype.h"
#include "ngraph.h"

class SmpSolver
{
public:
	SmpSolver(IloEnv env_, shared_ptr<Graph> G_);

	void add_primal_node_var();

	void add_partition_node_var();

	void set_root_nodes();
	
	void add_objective();

	void add_con_2(); 

	void add_con_40();

	void add_con_41();

	void add_con_43();

	void add_callback();

	void solve();

private:
	IloEnv env;
	IloModel model;
	IloCplex cplex;
	IloObjective objective;

	std::shared_ptr<Graph> G;

	map<NODE, IloNumVar> map_i_xi; // map stored all x_i
	map<NODE_INDEX_PAIR, IloNumVar> map_i_k_xik; // map stored all x_i^k
	map<INDEX, NODE> map_k_rk; // map stored  roots for all partitions
};

#endif // !SMPNS_HEAD
