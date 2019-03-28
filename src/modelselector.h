#ifndef MODELSELECTOR_H
#define MODELSELECTOR_H

#include "prefixspan.h"
#include "learnerspp.h"
#include "database.h"

#include <vector>

using namespace std;
using uint = unsigned int;

class ModelSelector{
public:
	virtual ~ModelSelector() {}
	virtual uint select(const vector<double> &aSolutionPath,const Database &aDatabase,const vector<uint> &aIntOptions,const vector<double> &aDoubleOptions) = 0;
};

#endif
