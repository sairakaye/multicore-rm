#include "bitmatrix.h"
#include "edlib.h"

//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "openmp-use-default-none"
//#include <unistd.h>



using namespace std;

/*
int TruePos = 0;
int TrueNeg = 0;
int FalsePos = 0;
int FalseNeg = 0;
*/

int truePos;
int trueNeg;

unsigned int alignmentNeeded = 0;
unsigned int notNeeded = 0;

void increment(int i, vector<int> &locations) {
    for (int j = i; j < locations.size(); j++) {
        locations[j]++;
    }

}

vector<int> createMap(string P, string T, int E, vector<int> &locations) {
    int m = T.size();
    int n = P.size();
    char t[m];
    char p[n];
    int count;

    vector<int> NMap;
    for (int i = 0; i < 2 * E + 1; i++) {
        NMap.push_back(2);
        locations.push_back(i);
    }

    int jE;
    for (int i = 0; i < m; i++) {
        jE = 0;
        for (int j = i - E; j < i + E + 1; j++) {
            if (jE <= 2 * E) {
                if (T[j] != NULL && P[i] == T[j]) {
                    NMap.insert(NMap.begin() + locations[jE], 0);
                    increment(jE, locations);
                    jE++;
                } else if (T[j] != NULL && P[i] != T[j]) {
                    NMap.insert(NMap.begin() + locations[jE], 1);
                    increment(jE, locations);
                    jE++;
                    count++;
                } else {
                    jE++;
                }
            }
        }
    }
    /*cout << "\n";
    for(int j = 0; j < NMap.size();j++) {
        if(NMap[j] == 2){
            cout << NMap[j];
            cout << "\n";
        }
        else{
            cout << NMap[j];
        }

    }*/

    int E1 = E;
    int E2 = 1;


    for (int i = 0; i < 2 * E + 1; i++) {
        if (i < E) {
            for (int j = 0; j < E1; j++) {
                NMap.insert(NMap.begin() + locations[i], -1);
                increment(i, locations);
            }
            E1--;
        }
            //hammingMap[i].erase(hammingMap[i].end() - E1, hammingMap[i].end());
        else if (i > E) {
            for (int j = 0; j < E2; j++) {
                NMap.insert(NMap.begin() + locations[i - 1] + 1, -1);
                increment(i, locations);
            }
            //hammingMap[i].erase(hammingMap[i].end() - E1, hammingMap[i].end());
            E2++;
        }

    }


/*
    for (int i = 0; i < E; i++){
        for (int j = 0; j < E1; j++)
        {
            *//*  if(i == 0){
                  NMap.insert(NMap.begin(), -1);
                  increment(i, locations);
              }*//*

            NMap.insert(NMap.begin() + locations[i], -1);
            increment(i, locations);
        }
        //hammingMap[i].erase(hammingMap[i].end() - E1, hammingMap[i].end());
        E1--;
    }
    for (int i = E + 1; i < E * 2 + 1; i++){
        for (int j = 0; j < E2; j++){
            NMap.insert(NMap.begin() + locations[i - 1] + 1, -1);
            increment(i, locations);
        }
        //hammingMap[i].erase(hammingMap[i].end() - E1, hammingMap[i].end());
        E2++;

    }*/

    return NMap;
}

int countZeroes(vector<int> toCount) {
    int ctr = 0;

    for (int i : toCount) {
        if (i == 0) {
            ctr++;
        }
    }
    return ctr;

}


int countOnes(vector<int> toCount, unsigned int E) {
    int ctr = 0;

    if (toCount.empty()) {
        return E + 1;
    } else {
        for (int i = 0; i < toCount.size(); i++) {
            if (toCount[i] == 1) {
                ctr++;
            }
        }
    }
    return ctr;
}

vector<int> checkDiagonals(int m, vector<int> NMap, int E, int w, int windowSize, vector<int> &locations, int ctr) {
    vector<int> bestDiagonal = {-1, -1, -1, -1};
    //vector<int> currDiagonal

    int bestZeroCount = 0;
    int currZeroCount = 0;

    int index = 0;
    //#pragma omp parallel for
    for (int i = 0; i < E * 2 + 1; i++) {

        for (int j = 0; j < windowSize; j++) {
            if (i == 0) {
                if (NMap[0 + j + ctr] == 0) {
                    currZeroCount++;
                }
            } else {
                if (NMap[locations[i - 1] + 1 + j + ctr] == 0) {
                    currZeroCount++;
                }
            }
        }

        if (currZeroCount >= bestZeroCount) {
            bestZeroCount = currZeroCount;
            if (i > 0) {
                //bestDiagonal.push_back(NMap[0 + j + ctr]);
                index = i;
            }

        }
        currZeroCount = 0;
    }


    for (int j = 0; j < windowSize; j++) {
        if (index == 0) {
            bestDiagonal[j] = NMap[0 + j + ctr];
        } else {
            bestDiagonal[j] = NMap[locations[index - 1] + 1 + j + ctr];
        }
    }

    //bestDiagonalsArray[w] = bestDiagonal;
    return bestDiagonal;
}

vector<int> slidingWindow(int E, vector<int> &locations, string P, string T) {
    int windowSize = 4;
    vector<int> finalVector(m, 1);
    int m = T.size();
    int n = P.size();
    char t[m];
    char p[n];
    int count;

    //vector<int> mainDiagonal;
    //vector<thread> threadArray;
/*
    for(int i = 0; i < m; i++){
        if(E == 0){
            finalVector.push_back(NMap[i]);
        }
        else{
            finalVector.push_back(NMap[locations[E-1] + 1 + i]);
        }

    }*/

    /*  if(NMap.empty()){
          return vector<int>();
      }*/


    vector<int> NMap;
    for (int i = 0; i < 2 * E + 1; i++) {
        NMap.push_back(2);
        locations.push_back(i);
    }
    int jE;
    for (int i = 0; i < m; i++) {
        jE = 0;
        for (int j = i - E; j < i + E + 1; j++) {
            if (jE <= 2 * E) {
                if (T[j] != NULL && P[i] == T[j]) {
                    NMap.insert(NMap.begin() + locations[jE], 0);
                    increment(jE, locations);
                    jE++;
                } else if (T[j] != NULL && P[i] != T[j]) {
                    NMap.insert(NMap.begin() + locations[jE], 1);
                    increment(jE, locations);
                    jE++;
                    count++;
                } else {
                    jE++;
                }
            }
        }
    }
    /*cout << "\n";
    for(int j = 0; j < NMap.size();j++) {
        if(NMap[j] == 2){
            cout << NMap[j];
            cout << "\n";
        }
        else{
            cout << NMap[j];
        }

    }*/

    int E1 = E;
    int E2 = 1;

    for (int i = 0; i < 2 * E + 1; i++) {
        if (i < E) {
            for (int j = 0; j < E1; j++) {
                NMap.insert(NMap.begin() + locations[i], -1);
                increment(i, locations);
            }
            E1--;
        }
            //hammingMap[i].erase(hammingMap[i].end() - E1, hammingMap[i].end());
        else if (i > E) {
            for (int j = 0; j < E2; j++) {
                NMap.insert(NMap.begin() + locations[i - 1] + 1, -1);
                increment(i, locations);
            }
            //hammingMap[i].erase(hammingMap[i].end() - E1, hammingMap[i].end());
            E2++;
        }

    }

    int ctr = 0;
    for (int w = 0; w < m; w++) {
        if (w + windowSize > m) {
            windowSize = m - w;
        }


        //vector<int> bestDiagonalinVector = checkDiagonals(m, NMap, E, w, windowSize, locations, ctr);

        vector<int> bestDiagonal = {-1, -1, -1, -1};
        //vector<int> currDiagonal

        int bestZeroCount = 0;
        int currZeroCount = 0;

        int index = 0;
        //#pragma omp parallel for
        for (int i = 0; i < E * 2 + 1; i++) {

            for (int j = 0; j < windowSize; j++) {
                if (i == 0) {
                    if (NMap[0 + j + ctr] == 0) {
                        currZeroCount++;
                    }
                } else {
                    if (NMap[locations[i - 1] + 1 + j + ctr] == 0) {
                        currZeroCount++;
                    }
                }
            }

            if (currZeroCount >= bestZeroCount) {
                bestZeroCount = currZeroCount;
                if (i > 0) {
                    //bestDiagonal.push_back(NMap[0 + j + ctr]);
                    index = i;
                }

            }
            currZeroCount = 0;
        }

        ctr++;

        int l = 0;
        for (int i = w; i < windowSize + w; i++) {
            if (i < m) {
                if (index == 0) {
                    finalVector[i] = NMap[0 + l + ctr];
                    l++;
                } else {
                    finalVector[i] = NMap[locations[index - 1] + 1 + l + ctr];
                }
            }
        }
    }


    return finalVector;


}

vector<int> BitMatrixAlgorithm(int E, string read, string reference) {

    vector<int> locations(2 * E + 1);
    int windowSize = 4;
    vector<int> finalVector(m);
    int m = reference.size();
    int n = read.size();
/*    char t[m];
    char p[n];*/
    int count = 0;
    int shiftLeftCount = 0;
    int shiftRightCount = 0;
    int diagLoc = 0;

    vector<int> NMap((2 * E + 1) * m + (2 * E + 1));
    vector<int> bestDiagonal = {-1, -1, -1, -1};
    vector<int> movingLocation;



    //NEIGHBORHOOD MAP
    int p = 0;
    for (int i = 0; i < 2 * E + 1; i++) {
        locations[i] = p;
        NMap[p] = 2;
        p += m + 1;
    }
    movingLocation = locations;

    int jE;
    for (int i = 0; i < m; i++) {
        jE = 0;
        for (int j = i - E; j < i + E + 1; j++) {
            if (jE <= 2 * E) {
                if ((reference[j] == 'A' || reference[j] == 'C' || reference[j] == 'G' || reference[j] == 'T') &&
                    read[i] == reference[j]) {
                    //NMap.insert(NMap.begin() + locations[jE], 0);
                    NMap[movingLocation[jE] + 1] = 0;

                    if (jE == E + 1) {
                        finalVector[diagLoc] = 0;
                        diagLoc++;
                    }
                    movingLocation[jE] += 1;
                    /*for (int j = jE; j < locations.size(); j++) {
                        locations[j]++;
                    }*/
                    jE++;
                } else if ((reference[j] == 'A' || reference[j] == 'C' || reference[j] == 'G' || reference[j] == 'T') &&
                           read[i] != reference[j]) {
                    //NMap.insert(NMap.begin() + locations[jE], 1);
                    NMap[movingLocation[jE] + 1] = 1;

                    if (jE == E + 1) {
                        finalVector[diagLoc] = 1;
                        diagLoc++;
                        count++;
                    }

                    movingLocation[jE] += 1;
                    /*for (int j = jE; j < locations.size(); j++) {
                        locations[j]++;
                    }*/
                    jE++;
                } else {
                    movingLocation[jE] += 1;
                    jE++;
                }
            }
        }
    }
    /*  int E1 = E;
      int E2 = 1;
      for (int i = 0; i < 2 * E + 1; i++) {
          if (i < E) {
              for (int j = 0; j < E1; j++) {
                  //NMap.insert(NMap.begin() + locations[i], -1);
                  NMap[movingLocation[i]] = -1;
                  movingLocation[i]++;
                  //increment(i, locations);
                *//*  for (int j = i; j < locations.size(); j++) {
                    locations[j]++;
                }*//*
            }
            E1--;
        }
            //hammingMap[i].erase(hammingMap[i].end() - E1, hammingMap[i].end());
        else if (i > E) {
            for (int j = 0; j < E2; j++) {
                //NMap.insert(NMap.begin() + locations[i - 1] + 1, -1);
                NMap[movingLocation[i]] = -1;
                movingLocation[i]++;
                //increment(i, locations);
                *//*for (int j = i; j < locations.size(); j++) {
                    locations[j]++;
                }*//*
            }
            //hammingMap[i].erase(hammingMap[i].end() - E1, hammingMap[i].end());
            E2++;
        }

    }
*/
/*    cout << "\n";
      for(int j = 0; j < NMap.size();j++) {
          if(NMap[j+1] == 2){
              cout << NMap[j];
              cout << "\n";
          }
          else{
              cout << NMap[j];
          }

      }
    cout << "\n";*/

    if (E == 0 || count <= E) {
        finalVector = NMap;
    } else {
        // END OF NEIGHBORHOOD MAP
        // SLIDING WINDOW
        int ctr = 0;
        for (int w = 0; w < m; w++) {
            if (w + windowSize > m) {
                windowSize = m - w;
            }

            //BEST DIAGONAL
            int bestZeroCount = 0;
            int currZeroCount = 0;
            int index = 0;

            //#pragma omp parallel for
            for (int i = 0; i < E * 2 + 1; i++) {

                for (int j = 0; j < windowSize; j++) {
                    /* if (i == 0) {
                         if (NMap[0 + j + ctr] == 0) {
                             currZeroCount++;
                         }
                     } else {
                         if (NMap[locations[i - 1] + 1 + j + ctr] == 0) {
                             currZeroCount++;
                         }
                     }*/
                    if (NMap[locations[i] + 1 + j + ctr] == 0) {
                        currZeroCount++;
                    }
                }
                if (currZeroCount >= bestZeroCount) {
                    bestZeroCount = currZeroCount;
                    //bestDiagonal.push_back(NMap[0 + j + ctr]);
                    index = i;
                }
                currZeroCount = 0;
            }
            //END OF BEST DIAGONAL
            ctr++;
            int l = 0;
            for (int i = w; i < windowSize + w; i++) {
                if (i < m) {
                    /* if (index == 0) {
                         finalVector[i] = NMap[0 + l + ctr];
                         l++;
                     } else {
                         finalVector[i] = NMap[locations[index - 1] + 1 + l + ctr];
                         l++;
                     }*/

                    finalVector[i] = NMap[locations[index] + 1 + l + ctr];
                    l++;

                }
            }
        }
    }
    //END OF SLIDING WINDOW

    return finalVector;

}


void bitMatrixFilterProcess() {

    // string readString, referenceString;
    //double finalTime;

    //vector<string> read;
    //vector<string> reference;

    //ifstream infile("/home/aaron/Desktop/Shouji-master/Datasets/ERR240727_1_E40_30million.txt");
    //ifstream infile("/home/aaron/Desktop/10k");

    //vector<string> fileNames;
    //vector<thread> threadArray;

    /*
    while (infile >> readString >> referenceString) {
        read.push_back(readString);
        reference.push_back(referenceString);
    }
    */

    //vector<string> reads;
    //vector<string> refs;

    //int refcount = refs.size();
    //int readcount = reads.size();

    //alignmentNeeded = 0;
    //notNeeded = 0;

    /*
    TruePos = 0;
    TrueNeg = 0;
    FalsePos = 0;
    FalseNeg = 0;
    */

    int E = e;

    //vector<int> location(2*e+1);
/*    ofstream pairReadsFile;
    string pairReadsFileName(to_string(e));
    pairReadsFile.open(pairReadsFileName.c_str(), ios::out);*/

    alignmentNeeded = 0;
    notNeeded = 0;
    //ofstream pairReadsFile;
    //string pairReadsFileName(to_string(E));
    //pairReadsFile.open(pairReadsFileName.c_str(), ios::out);

    auto start = std::chrono::high_resolution_clock::now();

    int i;
    #pragma omp parallel for reduction(+:notNeeded, alignmentNeeded)
    for (i = 0; i < reads.size(); i++) {
        string read(reads[i].readData);
        //vector<unsigned long long int> tempAcceptedLocations;

        if (reads[i].forwardLocations.size() > 0) {
            //vector<unsigned long long int> &locations = reads[i].forwardLocations;

            int j;
            for (j = 0; j < reads[i].forwardLocations.size(); j++) {
                if (refGenome.genomeData.substr(reads[i].forwardLocations[j], m).size() == m) {
                    if (countOnes(BitMatrixAlgorithm(E, read, refGenome.genomeData.substr(reads[i].forwardLocations[j], m)), E) <= E) {
                        //pairReadsFile << read << "\t" << refGenome.genomeData.substr(locations[j], m) << endl;

                        alignmentNeeded++;
                    } else {
                        notNeeded++;
                        #pragma omp critical
                        reads[i].forwardLocations.erase(reads[i].forwardLocations.begin() + j);
                        j--;
                    }
                } else {
                    notNeeded++;
                    #pragma omp critical
                    reads[i].forwardLocations.erase(reads[i].forwardLocations.begin() + j);
                    j--;
                }
            }
            /*
            #pragma omp critical
            reads[i].forwardLocations = vector<unsigned long long int>(tempAcceptedLocations);
            */
        } else if (reads[i].reverseLocations.size() > 0) {
            //vector<unsigned long long int> &locations = reads[i].reverseLocations;

            int j;
            for (j = 0; j < reads[i].reverseLocations.size(); j++) {
                if (refGenome.genomeData.substr(reads[i].reverseLocations[j], m).size() == m) {
                    if (countOnes(BitMatrixAlgorithm(E, read, refGenome.genomeData.substr(reads[i].reverseLocations[j], m)), E) <=
                        E) {
                        #pragma omp critical
                        //tempAcceptedLocations.push_back(locations[j]);
                        alignmentNeeded++;

                        //pairReadsFile << read << "\t" << refGenome.genomeData.substr(locations[j], m) << endl;

                    } else {
                        notNeeded++;
                        #pragma omp critical
                        reads[i].reverseLocations.erase(reads[i].reverseLocations.begin() + j);
                        j--;
                    }
                } else {
                    notNeeded++;
                    #pragma omp critical
                    reads[i].reverseLocations.erase(reads[i].reverseLocations.begin() + j);
                    j--;
                }
            }

            /*
            #pragma omp critical
            reads[i].reverseLocations = vector<unsigned long long int>(tempAcceptedLocations);
            */
        }
    }
    //pairReadsFile.close();

    /*
    int i;
    #pragma omp parallel for reduction(+:notNeeded, alignmentNeeded)
    for(i = 0; i < possibleReadsMap.size(); i++) {
        auto iteratorMap = possibleReadsMap.begin();
        advance(iteratorMap, i);

        string read = (*iteratorMap).first;
        vector<unsigned long long int>& locations = (*iteratorMap).second;
        vector<unsigned long long int> tempAcceptedLocations;

        int j;
        for (j = 0; j < locations.size(); j++) {
            if (countOnes(threadFunc(e, read, refGenome.genomeData.substr(locations[j], m)), e) <= e) {
                /*
                #pragma omp critical
                filteredReadsMap[(*iteratorMap).first].push_back(locations[j]);
                tempAcceptedLocations.push_back(locations[j]);
                alignmentNeeded++;
            } else {
                notNeeded++;
            }
        }

        /*
        #pragma omp critical
        filteredReadsMap[read] = vector<unsigned long long int>(tempAcceptedLocations);
    }
    */
    auto end = std::chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;
    bmRunTime = diff.count();

    numFilteredReadLocations = alignmentNeeded;
    //cout << diff.count() << "\t" << E << "\t" << alignmentNeeded << "\t" << notNeeded << endl;

    cout << "Time taken by the Bit Matrix filter process is: " << to_string(bmRunTime) << " sec" << endl;
    cout << "Number of needed locations: " << to_string(alignmentNeeded) << endl;
    cout << "Number of not needed locations: " << to_string(notNeeded) << endl << endl;
}


void verifyWithEdlib() {
    //cout << "Edlib:\nTime\tE\tAccepted\tRejected" << endl;

    auto start = std::chrono::high_resolution_clock::now();
    int i;
    for (i = 0; i < reads.size(); i++) {
        string read(reads[i].readData);
        //vector<unsigned long long int> tempAcceptedLocations;

        if (reads[i].forwardLocations.size() > 0) {
            //vector<unsigned long long int> &locations = reads[i].forwardLocations;

            int j;
            for (j = 0; j < reads[i].forwardLocations.size(); j++) {
                if (refGenome.genomeData.substr(reads[i].forwardLocations[j], m).size() == m) {
                    EdlibAlignResult resultEdlib;
                    string c = refGenome.genomeData.substr(reads[i].forwardLocations[j], m);
                    const char* const pRef = c.c_str();
                    const char* const pRead = read.c_str();
                    resultEdlib = edlibAlign(pRef, m, pRead, m,
                                             edlibNewAlignConfig(e, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));
                    edlibFreeAlignResult(resultEdlib);
                    if (resultEdlib.editDistance != -1) {
                        //tempAcceptedLocations.push_back(locations[j]);
                        truePos++;
//                        EdlibAccept = true;
                    } else {
//                        EdlibAccept = false;
                        trueNeg++;
                        reads[i].forwardLocations.erase(reads[i].forwardLocations.begin() + j);
                        j--;

                    }
                } else {
                    trueNeg++;
                    reads[i].forwardLocations.erase(reads[i].forwardLocations.begin() + j);
                    j--;
                }
//                auto end = std::chrono::high_resolution_clock::now();
            }
            //reads[i].forwardLocations = vector<unsigned long long int>(tempAcceptedLocations);
        } else if (reads[i].reverseLocations.size() > 0) {
            //vector<unsigned long long int> &locations = reads[i].reverseLocations;

            int j;
            for (j = 0; j < reads[i].reverseLocations.size(); j++) {
                if (refGenome.genomeData.substr(reads[i].reverseLocations[j], m).size() == m) {
                    EdlibAlignResult resultEdlib;
                    const char* const pRef = refGenome.genomeData.substr(reads[i].reverseLocations[j], m).c_str();
                    const char* const pRead = read.c_str();
                    resultEdlib = edlibAlign(pRef, m, pRead, m,
                                             edlibNewAlignConfig(e, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));
                    edlibFreeAlignResult(resultEdlib);
                    if (resultEdlib.editDistance!= -1) {
                        //tempAcceptedLocations.push_back(locations[j]);
                        truePos++;
//                        EdlibAccept = true;
                    } else {
//                        EdlibAccept = false;
                        trueNeg++;
                        reads[i].reverseLocations.erase(reads[i].reverseLocations.begin() + j);
                        j--;
                    }
                } else {
                    trueNeg++;
                    reads[i].reverseLocations.erase(reads[i].reverseLocations.begin() + j);
                    j--;
                }
//                auto end = std::chrono::high_resolution_clock::now();
            }
            //reads[i].reverseLocations = vector<unsigned long long int>(tempAcceptedLocations);
        }
    }
    numVerifiedReadLocations = truePos;

    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    //cout << diff.count() << "\t" << e << "\t" << truePos << "\t" << trueNeg << endl << endl;

    cout << "Time taken by Edlib is: " << to_string(diff.count()) << " sec" << endl;
    cout << "Number of accepted locations: " << to_string(truePos) << endl;
    cout << "Number of rejected locations: " << to_string(notNeeded) << endl << endl;
}

void preCheckWithEdlib() {
    cout << "Pre-Edlib:\nTime\tE\tAccepted\tRejected" << endl;

    int pos = 0;
    int neg = 0;

    auto start = std::chrono::high_resolution_clock::now();
    int i;
    for (i = 0; i < reads.size(); i++) {
        string read(reads[i].readData);
        //vector<unsigned long long int> tempAcceptedLocations;

        if (reads[i].forwardLocations.size() > 0) {
            //vector<unsigned long long int> &locations = reads[i].forwardLocations;

            int j;
            for (j = 0; j < reads[i].forwardLocations.size(); j++) {
                if (refGenome.genomeData.substr(reads[i].forwardLocations[j], m).size() == m) {
                    EdlibAlignResult resultEdlib;
                    string c = refGenome.genomeData.substr(reads[i].forwardLocations[j], m);
                    const char* const pRef = c.c_str();
                    const char* const pRead = read.c_str();
                    resultEdlib = edlibAlign(pRef, m, pRead, m,
                                             edlibNewAlignConfig(e, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));
                    edlibFreeAlignResult(resultEdlib);
                    if (resultEdlib.editDistance != -1) {
                        //tempAcceptedLocations.push_back(locations[j]);
                        pos++;
//                        EdlibAccept = true;
                    } else {
//                        EdlibAccept = false;
                        neg++;
//                        reads[i].forwardLocations.erase(reads[i].forwardLocations.begin() + j);
//                        j--;

                    }
                } else {
                    neg++;
//                    reads[i].forwardLocations.erase(reads[i].forwardLocations.begin() + j);
//                    j--;
                }
//                auto end = std::chrono::high_resolution_clock::now();
            }
            //reads[i].forwardLocations = vector<unsigned long long int>(tempAcceptedLocations);
        } else if (reads[i].reverseLocations.size() > 0) {
            //vector<unsigned long long int> &locations = reads[i].reverseLocations;

            int j;
            for (j = 0; j < reads[i].reverseLocations.size(); j++) {
                if (refGenome.genomeData.substr(reads[i].reverseLocations[j], m).size() == m) {
                    EdlibAlignResult resultEdlib;
                    string c = refGenome.genomeData.substr(reads[i].reverseLocations[j], m);
                    const char* const pRef = c.c_str();
                    const char* const pRead = read.c_str();
                    resultEdlib = edlibAlign(pRef, m, pRead, m,
                                             edlibNewAlignConfig(e, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));
                    edlibFreeAlignResult(resultEdlib);
                    if (resultEdlib.editDistance!= -1) {
                        //tempAcceptedLocations.push_back(locations[j]);
                        pos++;
//                        EdlibAccept = true;
                    } else {
//                        EdlibAccept = false;
                        neg++;
//                        reads[i].reverseLocations.erase(reads[i].reverseLocations.begin() + j);
//                        j--;
                    }
                } else {
                    neg++;
//                    reads[i].reverseLocations.erase(reads[i].reverseLocations.begin() + j);
//                    j--;
                }
//                auto end = std::chrono::high_resolution_clock::now();
            }
            //reads[i].reverseLocations = vector<unsigned long long int>(tempAcceptedLocations);
        }
    }
//    numVerifiedReadLocations = truePos;

    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cout << diff.count() << "\t" << e << "\t" << pos << "\t" << neg << endl << endl;
}

/*
void checkResultswithEdlib(){


    string readString, referenceString;
    vector<string> read;
    vector<string> reference;



    int ctr = 0;

    //ifstream infile("/home/aaron/Desktop/ERR240727_1_E2_30million.txt");
    ifstream infile("/home/aaron/Desktop/Shouji-master/Datasets/ERR240727_1_E2_30million.txt");
    //ifstream infile("/home/aaron/Desktop/Shouji-master/Datasets/Test");
    //ifstream infile("F0.txt");
    while (infile >> readString >> referenceString) {
        read.push_back(readString);
        reference.push_back(referenceString);
    }

    cout << "Time\tE\tTruePos\tTrueNeg\tFalsePos\tFalseNeg";

    for (int E = 0; E < 11; E++) {
        double finalTime;
        clock_t start, end;
        TruePos = 0;
        TrueNeg = 0;
        FalsePos = 0;
        FalseNeg = 0;

        bool EdlibAccept;
        bool ShoujiAccept;

        int m = reference[0].length();

        start = clock();
        for (int i = 0; i < reference.size(); i++) {

            ctr++;
            vector<int> shouji(m);


            start = clock();

            //if (E == 4) {

            shouji = slidingWindow(createMap(read[i], reference[i], E), m, E);

            //PRINTTTT
            /* cout << "\n" << "Final bit vector : ";
             for (int i = 0; i < shouji.size(); i++) {
                 cout << shouji[i];
             }
             cout << "\n" << "NUMBER OF ONES: ";
             cout << countOnes(shouji);
             cout << "\n" << "Size: ";
             cout << shouji.size();*/

/*
if (countOnes(shouji, E) <= E) {
    ShoujiAccept = true;
} else {
    ShoujiAccept = false;
}




const char* const edlibRead = read[i].c_str();


EdlibAlignResult resultEdlib = edlibAlign(reference[i].c_str(), m, edlibRead, m, edlibNewAlignConfig(E, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));
edlibFreeAlignResult(resultEdlib);
if (resultEdlib.editDistance!= -1)
    EdlibAccept = true;
else
    EdlibAccept =false;


if(ShoujiAccept && EdlibAccept){
    TruePos++;
}
else if(!ShoujiAccept && !EdlibAccept){
    TrueNeg++;
}
else if(ShoujiAccept && !EdlibAccept){
    FalsePos++;
}
else if(!ShoujiAccept && EdlibAccept){
    FalseNeg++;
}

//}

//}
end = clock();

finalTime += double(end - start) / double(CLOCKS_PER_SEC);
}
cout << "\n";
cout << finalTime << "\t" <<E<<"\t"<<TruePos<<"\t"<<TrueNeg<<"\t"<<FalsePos<<"\t"<<FalseNeg;
}
}


int main(void){
//fixedInputMain();
//multipleInputMain();

bitMatrixFilterProcess();

//checkResultswithEdlib();

return 0;
}
*/

//#pragma clang diagnostic pop