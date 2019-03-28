#ifndef CROSSVALIDATION_H
#define CROSSVALIDATION_H

#include "modelselector.h"
#include <random>
#include <queue>
//learnerspp
#include "lassospp.h"

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;
using namespace Eigen;
using uint = unsigned int;

class CrossValidation: public ModelSelector{
private:
	vector<uint> mNumDiv;
	vector<uint> create_shuffle_vector(uint aN);
	queue<vector<uint>> k_shuffle_split(uint aK, uint aN);

public:

	CrossValidation(){};
	vector<double> calc_accuracy(const vector<vector<double> > &aYHats,const vector<double> &aY,const int &aPathSize);
	uint select(const vector<double> &aSolutionPath, const Database &aDatabase, const vector<uint> &aIntOptions, const vector<double> &aDoubleOptions);
};

#endif
