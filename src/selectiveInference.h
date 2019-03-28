#ifndef SELECTIVEINFERENCE_H
#define SELECTIVEINFERENCE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/QR"
#include <iomanip>
#include <random>
#include "database.h"
#ifdef _OPENMP
#include <omp.h>
#endif


using namespace std;
using namespace Eigen;
using uint = unsigned int;

class SelectiveInference
{
private:
	MatrixXd mX;
	VectorXd mY;
	VectorXd mBeta;
	double mSigma;
	double mLambda;
	double mBias;
	uint mN;
	uint mM;
	// importance sampling
	uint mImportanceSampling;
	uint mSampling;

	VectorXd sign(const VectorXd &aV);
	double nuMinus(const MatrixXd &aA, const VectorXd &aB, const VectorXd &aC, const VectorXd &aZ, const double &aX);
	double nuPlus(const MatrixXd &aA, const VectorXd &aB, const VectorXd &aC, const VectorXd &aZ, const double &aX);
	double cdf_tn(const double &aA, const double &aB, const double &aMu, const double &aSquareSigma, const double &aX);
	double cdf_tn_importance_sampling(const double aT,const double aLower,const double aUpper,const double aVariance);

public:
	SelectiveInference(const MatrixXd &aXm, const VectorXd &aY, const VectorXd &aBeta, const double &aSigma, const double &aLambda, const double &aBias, const uint &aImportanceSampling,const uint &aSampling)
	{
		mX = aXm;
		mY = aY;
		mBeta = aBeta;
		mSigma = aSigma;
		mLambda = aLambda;
		mBias = aBias;
		mN = mY.size();
		mM = mBeta.size();
		mImportanceSampling = aImportanceSampling;
		mSampling = aSampling;
	}
	vector<double> infer();
	void printInference(const string &aFilename);
};

#endif
