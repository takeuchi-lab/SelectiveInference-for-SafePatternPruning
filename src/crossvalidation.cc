#include "crossvalidation.h"

uint CrossValidation::select(const vector<double> &aSolutionPath,const Database &aDatabase,const vector<uint> &aIntOptions,const vector<double> &aDoubleOptions){

	uint tK = aIntOptions[0];
	uint tAve = aIntOptions[1];
	uint tMinsup = aIntOptions[2];
	uint tMaxpat = aIntOptions[3];
	int tInterval = aIntOptions[4];
	uint tSupMode = aIntOptions[5];
	uint tCloSpan = aIntOptions[6];
	uint tMaxiter = aIntOptions[7];
	uint tFreq = aIntOptions[8];
	double tEps = aDoubleOptions[0];
	double tRatio = aDoubleOptions[1];

	uint tN = aDatabase.get_size();
	if (tN < tK)
	{
		cout << "error:CV,n<k" << '\n';
		exit(1);
	}

	omp_set_nested(1);

	vector<double> tSquareErrors(aSolutionPath.size() - 1, 0);
	#pragma omp parallel for
	for(uint i = 0; i < tAve; ++i){

		queue<vector<uint>> tDivIdx = k_shuffle_split(tK,tN);
		vector<vector<uint>> tTestIdxs;
		vector<vector<uint>> tTrainIdxs;

		for (uint j = 0; j < tK; ++j){
			queue<vector<uint>> tDivIdxCpy(tDivIdx);
			tTestIdxs.push_back(tDivIdxCpy.front());
			tDivIdxCpy.pop();
			vector<uint> tTrainIdx;
			while (!tDivIdxCpy.empty()) {
				for (uint tIdx : tDivIdxCpy.front())
				{
					tTrainIdx.push_back(tIdx);
				}
				tDivIdxCpy.pop();
			}
			tTrainIdxs.push_back(tTrainIdx);
			tDivIdx.push(tDivIdx.front());
			tDivIdx.pop();
		}

		#pragma omp parallel for
		for (uint j = 0; j < tK;++j){
			vector<vector<Event>> tTrainX = aDatabase.get_transaction(tTrainIdxs[j]);
			vector<double> tTrainY = aDatabase.get_y(tTrainIdxs[j]);

			vector<vector<Event>> tTestX = aDatabase.get_transaction(tTestIdxs[j]);
			vector<double> tTestY = aDatabase.get_y(tTestIdxs[j]);

			PrefixSpan tPrefix(tMinsup, tMaxpat, tInterval, tSupMode, tCloSpan);
			tPrefix.init(tTrainX, tTrainY);

			LASSOSPP tLearner = LASSOSPP(tMaxiter, tFreq, tEps, tRatio);
			vector<uint> tOption = {(uint)aSolutionPath.size()};
			vector<vector<double>> tYHats = tLearner.get_all_predict(tPrefix, aSolutionPath, tTestX, tOption);
			vector<double> tTmp = calc_accuracy(tYHats, tTestY, (uint)aSolutionPath.size());

			for (uint k = 0; k < tTmp.size();++k){
				#pragma omp atomic
				tSquareErrors[k] += tTmp[k];
			}

		}
	}

	vector<double>::iterator tIter;

	//Lasso
	// for(uint i = 0; i < tSquareErrors.size(); ++i){
	// 	cout << "λ[" << i + 1 << "]=" << aSolutionPath[i + 1] << " RMSE:" <<sqrt(tSquareErrors[i] / (double) (aIntOptions[1] * tN)) << '\n';
	// }
	tIter = min_element(tSquareErrors.begin(), tSquareErrors.end());
	return distance(tSquareErrors.begin(), tIter) + 1;
}

vector<double> CrossValidation::calc_accuracy(const vector<vector<double> > &aYHats,const vector<double> &aY,const int &aPathSize){
	vector<double> tSquareErrors(aPathSize - 1, 0);
	if(tSquareErrors.size()!=aYHats.size()){
		cout << "error:calc_accuracy" <<tSquareErrors.size()<<":"<<aYHats.size()<< '\n';
		exit(1);
	}

	for(uint j=0;j<aYHats.size();++j){
    	for(uint k=0;k<aY.size();++k){
			tSquareErrors[j] += (aY[k]-aYHats[j][k])*(aY[k]-aYHats[j][k]);
    	}
	}
	return tSquareErrors;
}

vector<uint> CrossValidation::create_shuffle_vector(uint aN){
	vector<uint> tVec(aN);
	iota(tVec.begin(), tVec.end(), 0);
	random_device seed_gen;
	mt19937 engine(seed_gen());
	shuffle(tVec.begin(), tVec.end(),engine);
	return tVec;
}

queue<vector<uint>> CrossValidation::k_shuffle_split(uint aK, uint aN){
	vector<uint> tShuffleIdices = create_shuffle_vector(aN);
	queue<vector<uint>> tDivIdx;
	for (uint i = 0; i < aK; ++i)
	{
		int tNum = (aN + i) / aK;
		vector<uint> tVec;
		for (int j = 0; j < tNum;++j){
			tVec.push_back(tShuffleIdices.back());
			tShuffleIdices.pop_back();
		}
		tDivIdx.push(tVec);
	}
	return tDivIdx;
}

