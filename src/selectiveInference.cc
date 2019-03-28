#include "selectiveInference.h"

vector<double> SelectiveInference::infer()
{

	vector<double> tPValues;
	vector<double> tPValues_old;
	VectorXd tS = sign(mBeta);

	MatrixXd XtX = mX.transpose() * mX + MatrixXd::Identity(mM, mM)*1e-10;
	MatrixXd tQr1 = (XtX).householderQr().solve(mX.transpose());
	MatrixXd tA1 = -1 * ((tS).asDiagonal()) * tQr1;
	MatrixXd temp1 = -1 * tS.asDiagonal();
	MatrixXd tQr2 = (XtX).householderQr().solve(tS);
	VectorXd tB1 = -1 * mLambda * (tS.asDiagonal()) * tQr2;
	MatrixXd tEta = (tQr1).transpose();
	VectorXd tSgnEtaTy = sign(tEta.transpose() * mY);
	MatrixXd tIn = MatrixXd::Identity(mN, mN);

	for (size_t i = 0; i < mM; i++)
	{
		VectorXd tC = mSigma * mSigma * tSgnEtaTy(i) * tEta.col(i) * ((tSgnEtaTy(i) * tEta.col(i)).transpose() * (tSgnEtaTy(i) * tEta.col(i))).inverse();
		VectorXd tZ = (tIn - tC * (tSgnEtaTy(i) * tEta.col(i)).transpose()) * mY;
		double t1 = 0;
		double t2 = mSigma * mSigma * (tEta.col(i).norm() * tEta.col(i).norm());
		double t3 = (tSgnEtaTy(i) * tEta.col(i)).transpose() * mY;
		double tNuMinus = nuMinus(tA1, tB1, tC, tZ, t3);
		double tNuPlus = nuPlus(tA1, tB1, tC, tZ, t3);

		if (t3 < 0 && tNuPlus > 0)
		{
			tNuPlus = 0;
		}
		else if (t3 > 0 && tNuMinus < 0)
		{
			tNuMinus = 0;
		}

		if(tNuMinus > t3 && tNuPlus < t3){
			cout << "Error" << endl;
		}else{
			if(mImportanceSampling == 1){
				double tPValue = cdf_tn_importance_sampling(t3,tNuMinus, tNuPlus, t2);
				if(tPValue > 0.5){
					tPValue = 1 - tPValue;
				}
				tPValues.push_back(tPValue);
			}else{
				double tPValue =cdf_tn(tNuMinus,tNuPlus,t1,t2,t3);
				if(tPValue > 0.5){
					tPValue = 1 - tPValue;
				}
				tPValues.push_back(tPValue);
			}
		}
	}

	return tPValues;
}

void SelectiveInference::printInference(const string &aFilename){
}

double SelectiveInference::cdf_tn_importance_sampling(const double  aT,const double aLower,const double aUpper,const double aVariance){
	double tNumerator = 0;
	double tDenominator = 0;
	random_device rd;
	mt19937 gen{rd()};
	normal_distribution<double> d{aT,sqrt(aVariance)};
	double tX;

	for (uint i = 0; i < mSampling; ++i){
		tX = d(gen);
		if (aT <= tX && tX <= aUpper)
		{
			tNumerator += exp((-aT * tX + aT * aT) / aVariance);
		}
		tX = d(gen);
		if (aLower <= tX && tX <= aUpper)
		{
			tDenominator += exp((-aT * tX + aT * aT) / aVariance);
		}
	}
	return tNumerator / tDenominator;
}

VectorXd SelectiveInference::sign(const VectorXd &aV)
{
	VectorXd tSign = VectorXd::Zero(mM);
	for (uint i = 0; i < mM; ++i)
	{
		tSign(i) = (aV(i) > 0) - (aV(i) < 0);
	}
	return tSign;
}

double SelectiveInference::nuMinus(const MatrixXd &aA, const VectorXd &aB, const VectorXd &aC, const VectorXd &aZ, const double &aX)
{
	VectorXd tAc = aA * aC;
	VectorXd tAz = aA * aZ;

    double tV = -10;
	for (size_t i = 0; i < mM; i++)
	{
		if (tAc(i) < 0)
		{
		    double tmp = (aB(i) - tAz(i)) / tAc(i);
			if (aX < tmp)
			{
			}
			else
			{
				if (tV < tmp)
				{
					tV = tmp;
				}
			}
		}
	}
	return tV;
}

double SelectiveInference::nuPlus(const MatrixXd &aA, const VectorXd &aB, const VectorXd &aC, const VectorXd &aZ, const double &aX)
{
	VectorXd tAc = aA * aC;
	VectorXd tAz = aA * aZ;

	double tV = 10000000000000;
	for (size_t i = 0; i < mM; i++)
	{
		if (tAc(i) > 0)
		{
			double tmp = (aB(i) - tAz(i)) / tAc(i);
			if (tmp >= aX && tV > tmp)
			{
				tV = tmp;
			}
		}
	}
	return tV;
}

double normalCDF(double x)
{
    return std::erfc(-x/std::sqrt(2))/2;
}

double SelectiveInference::cdf_tn(const double &aA, const double &aB, const double &aMu, const double &aSquareSigma, const double &aX)
{
	return (normalCDF((aX - aMu) / sqrt(aSquareSigma)) - normalCDF((aA - aMu) / sqrt(aSquareSigma))) / (normalCDF((aB - aMu) / sqrt(aSquareSigma)) - normalCDF((aA - aMu) / sqrt(aSquareSigma)));
}