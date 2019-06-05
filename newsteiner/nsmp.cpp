#include "nsmp.h"
#include "ncallback.h"
#include "nseparation.h"
#include "ntype.h"

#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#define LOG \
    if (false) cerr
#define TOL 0.001

using namespace std;
