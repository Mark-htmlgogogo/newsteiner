#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include "ngraph.h"
#include "nseparation.h"
#include "nsmp.h"
#include "ntype.h"
#include "nunionfind.h"
#define LOG \
    if (false) cerr

using namespace std;
using namespace lemon;
