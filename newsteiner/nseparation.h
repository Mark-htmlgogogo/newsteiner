#pragma once

#include <lemon/concepts/maps.h>
#include <lemon/connectivity.h>
#include <lemon/preflow.h>
#include <lemon/smart_graph.h>
#include <lemon/time_measure.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "ngraph.h"
#include "nsmp.h"
#include "ntype.h"

using namespace std;
using namespace lemon;

typedef SmartDigraph::Node LemonNode;
typedef SmartDigraph::Arc LemonArc;
typedef pair<LemonNode, LemonNode> LemonNode_Pair;

typedef ListDigraph::Node ListNode;
typedef ListDigraph::Arc ListArc;
typedef pair<ListNode, ListNode> ListNode_Pair;
