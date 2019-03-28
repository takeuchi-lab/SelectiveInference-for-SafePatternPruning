#ifndef LASSOSPP_H
#define LASSOSPP_H

#include "prefixspan.h"
#include "database.h"
#include "learnerspp.h"
#include <vector>

using namespace std;

using uint = unsigned int;

//Lasso
class LASSOSPP : public LearnerSPP{
private:

	uint mN;

	double mBias;
	vector<double> mR;
	uint mT;
	uint mMaxIter;
	uint mFreq;
	double mEps;
	double mRatio;
	double clac_sup(const vector<Event> &aSequence,const vector<Event> &aPattern,const uint aSupMode);

public:
	LASSOSPP(uint aMaxIter, uint aFreq, double aEps,double aRatio){
	mMaxIter = aMaxIter;
	mFreq = aFreq;
	mEps = aEps;
	mRatio = aRatio;
	};
	virtual void learn(PrefixSpan &aPrefix,const vector<double> &aLambdas,const vector<uint> &aOptions);
	virtual double get_lambda_max(PrefixSpan &aPrefix);
	virtual vector<double> predict(const PrefixSpan &aPrefix,const vector<vector<Event> > &aTransaction) const;
	virtual vector<vector<double> > get_all_predict(PrefixSpan &aPrefix,const vector<double> &aLambdas,const vector<vector<Event> > &aTransaction,const vector<uint> &aOptions);
	virtual double get_bias();
};

#endif
