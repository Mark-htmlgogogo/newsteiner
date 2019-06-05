/*
file : separation.h
*/

#include "ngraph.h"
#include "nseparation.h"
#include "nsmp.h"
#include "ntype.h"
#include "nunionfind.h"

#include <lemon/concepts/maps.h>
#include <lemon/connectivity.h>
#include <lemon/list_graph.h>
#include <lemon/preflow.h>
#include <lemon/smart_graph.h>
#include <lemon/time_measure.h>
#include <algorithm>
#include <fstream>
#include <iostream>

#define LOG \
    if (false) cerr
//#define LOG cout
#define TOL 0.001

using namespace std;
using namespace lemon;
