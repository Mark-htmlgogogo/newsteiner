#pragma once
#include <set>

using namespace std;

typedef			int					NODE;
typedef			set<NODE>			NODE_SET;

typedef			pair<int, int>		ARC;
typedef			set<ARC>			ARC_SET;

typedef			int					INDEX; // as partition index
typedef			int					INDEX_MAX;

typedef			pair<NODE, INDEX>	NODE_INDEX_PAIR;

typedef			int					COMP_INDEX; // as component index in a partition