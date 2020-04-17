//
// Created by saimanalili on 16/04/2020.
//

#include "directaddressing.h"

vector<string> qGrams;

void generateQGrams(string prefix, int k)
{
    char charactersDNA[] = {'A', 'C', 'G', 'T'};

    if (k == 0) {
        qGrams.push_back(prefix);
        return;
    }

    for (int i = 0; i < 4; i++) {
        string newPrefix;
        newPrefix = prefix + charactersDNA[i];
        generateQGrams(newPrefix, k - 1);
    }
}

void buildDirectAddressingTables(string stringDNA, string mainName, int m, int q, unsigned long long int dirTableSize, unsigned long long int posTableSize)
{
    auto indexing_time_start = chrono::high_resolution_clock::now();

    ofstream outfile;
    outfile.open("dir_" + mainName + "_" + to_string(q) + ".txt", ios::out);

    vector<unsigned long> dirTable(dirTableSize);
    vector<unsigned long> posTable(posTableSize);

    fill(dirTable.begin(), dirTable.end(), 0);
    fill(posTable.begin(), posTable.end(), 0);

    generateQGrams("", q);

    for (int i = 0; i < (m - (q - 1)); i++) {
        unsigned int tempIndex = extractRanking(stringDNA.substr(i, q));
        dirTable.at(tempIndex)++;
    }

    for (int i = 1; i < dirTable.size(); i++) {
        dirTable.at(i) += dirTable.at(i-1);
    }

    int index = 0;
    for (int i = m - q; i >= 0; i--) {
        unsigned long int tempIndex = extractRanking(stringDNA.substr(i, q));
        dirTable.at(tempIndex)--;

        index = dirTable.at(tempIndex);
        posTable.at(index) = i;
    }

    auto indexing_time_end = chrono::high_resolution_clock::now();
    duration<double, std::milli> indexing_duration = duration_cast<milliseconds>(indexing_time_end - indexing_time_start);
    cout << "Time taken by the indexing process is : " << (indexing_duration.count()/1000) << " sec" << endl << endl;
    cout << "Starting the writing process..." << endl << endl;

    auto writing_time_start = chrono::high_resolution_clock::now();

    outfile << "dir" << "\n";
    for (int i = 0; i < dirTable.size(); i++) {
        outfile << dirTable.at(i) << "\n";
    }
    outfile << "\n" << "pos" << "\n";
    for (int i = 0; i < posTable.size(); i++) {
        outfile << posTable.at(i) << "\n";
    }

    auto writing_time_end = chrono::high_resolution_clock::now();
    duration<double, std::milli> writing_duration = duration_cast<milliseconds>(writing_time_end - writing_time_start);
    cout << "Time taken by the index writing process is : " << (writing_duration.count()/1000) << " sec" << endl << endl;

    outfile.close();
}

void buildDirectAddressingIndexing(string& genome, string& mainName) {
    unsigned long long int dirTableSize = pow(4, q) + 1;
    int posTableSize = genome.size() - q + 1;

    cout << "Direct Addressing for q = " << q << endl << endl;
    buildDirectAddressingTables(genome, mainName, genome.length(), q, dirTableSize, posTableSize);
}