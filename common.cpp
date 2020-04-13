//
// Created by saimanalili on 25/02/2020.
//

#include "common.h"

string readGenomeFile(string filename) {
    string genome;

    ifstream fileGenome (filename);
    string line;

    if (fileGenome.is_open()) {
        while (getline (fileGenome,line)) {
            if (line.rfind(">", 0) == 0)
                continue;

            genome.append(line);
        }

        fileGenome.close();
    } else {
        cout << "File does not exist." << endl;
        cout << "File name: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    return genome;
}

vector<string> readReadsFile(string filename) {
    vector<string> readList;

    ifstream fileRead(filename);
    string line;
    string readName;

    if (fileRead.is_open()) {
        while (getline (fileRead,line)) {
            if (line.rfind(">", 0) == 0) {
                readName = line.substr(1, line.find(' '));
                continue;
            }

            if (!line.empty() && (line[line.length()-1] == '\n' || line[line.length()-1] == '\r')) {
                line.erase(line.length()-1);
            }

            readList.push_back(line);
            readsLabelMap[readName].push_back(line);
            readsLabelMap[readName].push_back(reverseComplement(line));
        }

        fileRead.close();
    } else {
        cout << "File does not exist." << endl;
        cout << "File name: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    return readList;
}

string reverseComplement(string read) {
    string reverseRead = read;
    reverse(reverseRead.begin(), reverseRead.end());

    for (int i = 0; i < reverseRead.length(); i++) {
        switch (reverseRead[i]) {
            case 'A':
                reverseRead[i] = 'T';
                break;
            case 'C':
                reverseRead[i] = 'G';
                break;
            case 'G':
                reverseRead[i] = 'C';
                break;
            case 'T':
                reverseRead[i] = 'A';
                break;
        }
    }

    return reverseRead;
}

void getDirectAddressing(string filename, vector<unsigned long long int>& dirTable, vector<unsigned long long int>& posTable) {
    ifstream directAddrFile(filename);
    string line;

    bool isGettingDir = false;
    bool isGettingPos = false;

    if (directAddrFile.is_open()) {
        while (getline (directAddrFile,line)) {
            if (!line.empty() && (line[line.length()-1] == '\n' || line[line.length()-1] == '\r')) {
                line.erase(line.length()-1);
            }

            stringstream ss(line);
            string tok;

            if (line.compare("dir") == 0) {
                isGettingDir = true;
                isGettingPos = false;
                continue;
            } else if (line.compare("pos") == 0) {
                isGettingDir = false;
                isGettingPos = true;
                continue;
            } else {
                if (isGettingDir) {
                    while(getline(ss, tok, ' ')) {
                        if (tok.length() > 0) {
                            dirTable.push_back(strtoull(tok.c_str(), nullptr, 10));
                        }
                    }
                } else if (isGettingPos) {
                    while(getline(ss, tok, ' ')) {
                        if (tok.length() > 0) {
                            posTable.push_back(strtoull(tok.c_str(), nullptr, 10));
                        }
                    }
                }
            }
        }

        directAddrFile.close();
    } else {
        cout << "File does not exist." << endl;
        cout << "File name: " << filename << endl;
        exit(EXIT_FAILURE);
    }
}

void getOpenAddressing(string filename, map<long long, unsigned long long int>& codeTable, vector<unsigned long long int>& dirTable, vector<unsigned long long int>& posTable) {
    ifstream openAddrFile(filename);
    string line;

    bool isGettingCode = false;
    bool isGettingDir = false;
    bool isGettingPos = false;

    if (openAddrFile.is_open()) {
        while (getline (openAddrFile,line)) {
            if (!line.empty() && (line[line.length() - 1] == '\n' || line[line.length() - 1] == '\r')) {
                line.erase(line.length() - 1);
            }

            stringstream ss(line);
            string tok;
            if (line.compare("code") == 0) {
                isGettingCode = true;
                isGettingDir = false;
                isGettingPos = false;
                continue;
            } else if (line.compare("dir") == 0) {
                isGettingCode = false;
                isGettingDir = true;
                isGettingPos = false;
                continue;
            } else if (line.compare("pos") == 0) {
                isGettingCode = false;
                isGettingDir = false;
                isGettingPos = true;
                continue;
            } else {
                if (isGettingCode) {
                    long long getThis;
                    unsigned long long int num;
                    int i = 0;

                    while(getline(ss, tok, ' ')) {
                        if (tok.length() > 0 && tok.compare("") != 0) {
                            if (i == 0) {
                                getThis = strtoll(tok.c_str(), nullptr, 10);

                                if (getThis == -1) {
                                    break;
                                }
                            } else if (i == 1) {
                                num = strtoull(tok.c_str(), nullptr, 10);
                            }
                        }

                        i++;
                    }

                    if (getThis != -1) {
                        codeTable.insert(pair<long long, unsigned long long int>(getThis, num));
                    }
                } else if (isGettingDir) {
                    while(getline(ss, tok, ' ')) {
                        if (tok.length() > 0) {
                            dirTable.push_back(strtoull(tok.c_str(), nullptr, 10));
                        }
                    }
                } else if (isGettingPos) {
                    while(getline(ss, tok, ' ')) {
                        if (tok.length() > 0) {
                            posTable.push_back(strtoull(tok.c_str(), nullptr, 10));
                        }
                    }
                }
            }
        }

        openAddrFile.close();
    } else {
        cout << "File does not exist." << endl;
        cout << "File name: " << filename << endl;
        exit(EXIT_FAILURE);
    }
}

vector<unsigned long long int> splitToInt(string str, char delimiter) {
    vector<unsigned long long int> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while(getline(ss, tok, delimiter)) {
        if (tok.length() > 0) {
            internal.push_back(strtoull(tok.c_str(), nullptr, 10));
        }
    }

    return internal;
}

map<unsigned long long int, vector<unsigned long long int>> getMinimizers(string filename) {
    ifstream fileMinimizer(filename);
    string line;

    map<unsigned long long int, vector<unsigned long long int>> minimizers;

    if (fileMinimizer.is_open()) {
        while (getline (fileMinimizer, line)) {
            if (line.rfind(">", 0) == 0)
                continue;

            unsigned long long int minimizerHashRank = strtoull(line.substr(0, line.find(':')).c_str(), nullptr, 10);
            string locations = line.substr(line.find(':') + 1, line.length() - line.find(":"));

            minimizers[minimizerHashRank] = splitToInt(locations, ' ');
        }

        fileMinimizer.close();
    } else {
        cout << "File does not exist." << endl;
        cout << "File name: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    return minimizers;
}

void results(map<string, vector<unsigned long long int>>& forwardReadsMap, map<string, vector<unsigned long long int>>& reverseReadsMap) {
    cout << "Number of seeds: " << to_string(counter->numSeeds) << endl;
    cout << "Number of reads: " << to_string(counter->numReads) << endl << endl;
    infoFile << "Number of seeds: " << to_string(counter->numSeeds) << endl;
    infoFile << "Number of reads: " << to_string(counter->numReads) << endl << endl;

    cout << "Number of accepted seeds: " << to_string(counter->numAcceptedSeeds) << endl;
    cout << "Number of accepted reads: " << to_string(counter->numAcceptedReads) << endl << endl;
    infoFile << "Number of accepted seeds: " << to_string(counter->numAcceptedSeeds) << endl;
    infoFile << "Number of accepted reads: " << to_string(counter->numAcceptedReads) << endl << endl;

    vector<unsigned long long int> forwardFound;
    vector<unsigned long long int> reverseFound;

    cout << "Putting forward and reverse locations for counting..." << endl;
    for (pair<string, vector<unsigned long long int>> readPair : forwardReadsMap) {
        for (unsigned long long int location : readPair.second) {
            forwardFound.push_back(location);
        }
    }

    for (pair<string, vector<unsigned long long int>> readPair : reverseReadsMap) {
        for (unsigned long long int location : readPair.second) {
            reverseFound.push_back(location);
        }
    }

    cout << "Removing duplicate locations of the read/seeds mapped..." << endl;
    unordered_set<unsigned long long int> duplicateRemoverSet;

    for (unsigned long long int location : forwardFound)
        duplicateRemoverSet.insert(location);
    forwardFound.assign(duplicateRemoverSet.begin(), duplicateRemoverSet.end());
    duplicateRemoverSet.clear();

    for (unsigned long long int location : reverseFound)
        duplicateRemoverSet.insert(location);
    reverseFound.assign(duplicateRemoverSet.begin(), duplicateRemoverSet.end());
    duplicateRemoverSet.clear();

    cout << "Number of seeds found from forward (unique locations): " + to_string(forwardFound.size()) << endl;
    cout << "Number of seeds found from backward (unique locations): " + to_string(reverseFound.size()) << endl;
    infoFile << "Number of seeds found from forward (unique locations): " + to_string(forwardFound.size()) << endl;
    infoFile << "Number of seeds found from backward (unique locations): " + to_string(reverseFound.size()) << endl;

    vector<unsigned long long int> combined(forwardFound);
    combined.insert(combined.end(), reverseFound.begin(), reverseFound.end());
    sort(combined.begin(), combined.end());
    combined.erase(unique(combined.begin(), combined.end()), combined.end());

    cout << "Number of seed locations from forward and backward (including intersection): " + to_string(forwardFound.size() + reverseFound.size()) << endl;
    cout << "Number of seed locations accepted (from both): " + to_string(combined.size()) << endl;
    infoFile << "Number of seed locations from forward and backward (including intersection): " + to_string(forwardFound.size() + reverseFound.size()) << endl;
    infoFile << "Number of seed locations accepted (from both): " + to_string(combined.size()) << endl;

    if (q == 20) {
        for (unsigned long long int location : combined) {
            outputLocationsFile << to_string(location) << endl;
        }
    }
}