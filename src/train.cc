
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <memory>
#include <algorithm>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "database.h"
#include "prefixspan.h"

//learnerspp
#include "lassospp.h"

//modelselector
#include "crossvalidation.h"
#include "selectiveInference.h"

using namespace std;
using namespace Eigen;
using uint = unsigned int;

inline void exit_with_help(){
	cout << "-- Safe Pattern Pruning for sequential pattern mining --\n"
			"Usage: train [-options] input_file\n"
			"options:\n"
			"    -t : learning regularization parameter index (besides CV) (default:min lambda)\n"
			"    -m : min support (default 1)\n"
			"    -L : max pattern length  (default 10)\n"
			"    -T : number of regularization path (default 100)\n"
			"    -r : min regularization path ratio (default 2.0)\n"
			"    -i : max iteration (default 1000000)\n"
			"    -f : frequency of convergence check (default 50)\n"
			"    -e : tolerance (default 1e-6)\n"
			"    -F : output filename (default result.csv)\n"
			"    -p : max event interval (default 0)\n"
			"    -c : is cross validation (default 0:false|1:true)\n"
        	"        -k : k-fold (default:10)\n"
        	"        -a : average (default:10)\n"
			"    -C : is CloSpan (default 0:false|1:true)\n"
			"    -I : importance sampling(default 1:true|0:false)\n"
			"    -R : sampling(importance sampling , default 1e+8)\n"
			"    -s : is support frequency (default 0:false|1:true)\n"
	<< endl;
	exit(1);
}


void print_info(const uint &aLearnerType,const uint &aMaxpat){
	cout << "Learner:LASSO" << '\n';
	cout << "Max pattern length:" << aMaxpat<<'\n';
}

vector<double> create_regularization_path(uint tN,double tRatio,double tLambdaMax){
	vector<double> tVec(tN);
	for (uint t = 0; t < tN; ++t)
	{
		tVec[t] = tLambdaMax * pow(10, -tRatio * t / (tN - 1));
	}
	return tVec;
}

int main(int argc, char **argv){
	uint tMinsup = 1;
	uint tMaxpat = 10;
	uint tT = 100;
	uint tMaxiter = 1000000;
	uint tFreq = 50;
	double tEps = 1e-6;
	double tRatio = 2.0;
	uint tInterval = 0;
	uint tSupMode = 0;
	uint tCV = 0;
	uint tK = 10;
	uint tAve = 10;
	uint tTargetLambda = tT-1;
	uint tCloSpan = 0;
	uint tIS = 1;
	uint tR = 1e+8;
	string tFilename = "./result.csv";

	int tI;
	for(tI = 1; tI < argc; tI++){
		if(argv[tI][0] != '-'){
			break;
		}
		if(++tI >= argc){
			exit_with_help();
		}
		switch(argv[tI - 1][1]){
			case 't':
				tTargetLambda = atoi(argv[tI]);
				break;
			case 'm':
				tMinsup = atoi(argv[tI]);
				break;
			case 'L':
				tMaxpat = atoi(argv[tI]);
				break;
			case 'T':
				tT = atoi(argv[tI]);
				break;
			case 'r':
				tRatio = atof(argv[tI]);
				break;
			case 'i':
				tMaxiter = atoi(argv[tI]);
				break;
			case 'f':
				tFreq = atoi(argv[tI]);
				break;
			case 'e':
				tEps = atof(argv[tI]);
				break;
			case 'F':
				tFilename = argv[tI];
				break;
			case 'p':
				tInterval = atoi(argv[tI]);
				break;
			case 's':
				tSupMode = atoi(argv[tI]);
				break;
			case 'c':
				tCV = atoi(argv[tI]);
				break;
			case 'k':
				tK = atoi(argv[tI]);
				break;
			case 'a':
				tAve = atoi(argv[tI]);
				break;
			case 'C':
				tCloSpan = atoi(argv[tI]);
				break;
			case 'I':
				tIS = atoi(argv[tI]);
				break;
			case 'R':
				tR = atoi(argv[tI]);
				break;
			default:
				cerr << "Error unknown option: -" << argv[tI - 1][1] << endl;
				exit_with_help();
				break;
		}
	}

	if(tI >= argc){
		cerr << "Error please input filename" << endl;
		exit_with_help();
	}

	Database tDatabase;
	tDatabase.read(argv[tI]);
	vector<vector<Event> > tTransaction = tDatabase.get_transaction();
	vector<double> tY = tDatabase.get_y();
	uint tN = tY.size();

	//make prefixspan
	PrefixSpan tPrefix(tMinsup, tMaxpat, tInterval, tSupMode, tCloSpan);
	tPrefix.init(tTransaction, tY);

	LearnerSPP* tLearner = nullptr;
	tLearner = new LASSOSPP(tMaxiter, tFreq, tEps, tRatio);

	//calculate lambda max
	double tLamMax = tLearner->get_lambda_max(tPrefix);

	//calculate regularization path
	vector<double> tLambdas = create_regularization_path(tT,tRatio,tLamMax);

	//cross validation(choose regularization parameter)
	if(tCV == 1){
		cout << "[start]Cross Validation" << endl;
		cout << tK << "-fold : " << tAve << "times" << endl;
		CrossValidation tCrossValidation;
		vector<uint> tIntOptions = {tK, tAve, tMinsup, tMaxpat, tInterval, tSupMode, tCloSpan, tMaxiter, tFreq};
		vector<double> tDoubleOptions = {tEps,tRatio};
		tTargetLambda = tCrossValidation.select(tLambdas, tDatabase, tIntOptions, tDoubleOptions);
		cout << "[end]Cross Validation" << '\n' << endl;
	}

	cout << "[start]Learning" << endl;
	vector<uint> tLearningOption = { tTargetLambda };
	tLearner->learn(tPrefix, tLambdas, tLearningOption);
	cout << "[end]Learning" << '\n'<< endl;

	// non zero feature vector
	MatrixXd tXm = tPrefix.get_NonZeroWeightX();
	VectorXd tVectorXdY = VectorXd::Zero(tN);
	for(uint i = 0; i < tN; i++){
		tVectorXdY(i)=tY[i];
	}
	// weight vector
	VectorXd tBeta = tPrefix.get_weight();
	double tSigma = 1;
	double tLambda = tTargetLambda;
	double tBias = tLearner->get_bias();
	delete tLearner;
	MatrixXd tBiasVec = MatrixXd::Ones(tN, 1);
	VectorXd tYd = tVectorXdY - tBiasVec * tBias;

	cout << "[start]Selective Inference" << endl;
	if(tIS == 1){
		cout << "Importance Sampling : " << tR << endl;
	}else{
		cout << "Importance Sampling : off"<< endl;
	}
	//SelectiveInference
	SelectiveInference tSI(tXm, tYd, tBeta, tSigma, tLambda, tBias, tIS, tR);
	vector<double> tPvalues = tSI.infer();
	cout << "[end] Selective Inference" << '\n' << endl;

	// output result
	cout << "[start]Result writting " << endl;
	cout << tFilename << endl;
	tPrefix.printTree(tFilename, tPvalues);
	cout << "[end]Result writting " << endl;

	return 0;
}
