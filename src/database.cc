#include "database.h"

void Database::read(const char *aFilename){
	ifstream tFile(aFilename);
	if(!tFile){
		cerr << "Error: cannot open file" << endl;
		exit(1);
	}

	uint tItemSize = 0;
	double tLabel;
	string tLine;
	vector<Event> tSequence;

	while(getline(tFile, tLine)){
		tSequence.clear();
		stringstream ss1(tLine);
		ss1 >> tLabel;
		mY.push_back(tLabel);
		string eventstring;

		while(ss1 >> eventstring){
			stringstream ss2(eventstring);
			Itemset tItemset;
			vector<uint> tItem;
			string itemstring;
			int tmp;
			while(getline(ss2, itemstring, ':')){
				if(contain(itemstring, '(')){
					string tString;
					stringstream ss3(itemstring);
					if(contain(itemstring, '_')){
						while(getline(ss3, tString, '_')){
							if(contain(tString, '(')){
								if(contain(tString, ')')) break;

								tString.erase(tString.begin());
							}else if(contain(tString, ')')){
								tString.pop_back();
							}
							tItemset.push_back(stoi(tString));
						}
					}else {
						itemstring.erase(itemstring.begin());
						itemstring.pop_back();

						tItemset.push_back(stoi(itemstring));
					}

				}else{
					tmp = stoi(itemstring);
					uint tVal = (tmp < 0) ? 0xffffffff : tmp; // wild card
					tItem.push_back(tVal);
				}
			}

			if(!tItemSize){
				tItemSize = tItem.size();
			}else{
				if(tItemSize != tItem.size()){
					cerr << "Format Error: different Event Size at line: " << mTransaction.size() << ", event: " << tSequence.size() << endl;
					exit(-1);
				}
			}

			Event tEvent = make_pair(tItemset, tItem);
			tSequence.push_back(tEvent);
		}
		mTransaction.push_back(tSequence);
	}
}

vector<vector<Event>> Database::get_transaction()const{
	return mTransaction;
}

vector<vector<Event>> Database::get_transaction(vector<uint> tIndices)const{
	vector<vector<Event>> tTrainsaction(tIndices.size());
	for (uint i = 0; i < tIndices.size(); ++i)
	{
		tTrainsaction[i] = mTransaction[tIndices[i]];
	}
	return tTrainsaction;
}

vector<double> Database::get_y()const{
	return mY;
}

vector<double> Database::get_y(vector<uint> tIndices)const{
	vector<double> tY(tIndices.size());
	for (uint i = 0; i < tIndices.size(); ++i)
	{
		tY[i] = mY[tIndices[i]];
	}
	return tY;
}

uint Database::get_size()const{
	return mY.size();
}
