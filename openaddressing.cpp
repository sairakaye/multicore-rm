/**
 * @file openaddressing.cpp
 * @author A. Fajardo, S. Manalili, C. Mercado, R. Zapanta
 * @brief It contains the implementation of the Minimizers mode of Hash-based Indexer.
 */

#include "openaddressing.h"

/**
 * It builds the open addressing index.
 *
 * @param genome - The reference genome data.
 * @param genomeFileName - The filename of the reference genome.
 * @param n - The length of the reference genome.
 * @param q - The value of the q-gram.
 * @param codeTableSize - The length or size of the code table.
 * @param dirTableSize - The length or size of the directory table.
 * @param posTableSize - The length or size of the position table.
 */
void buildOpenAddressingTables(string genome, string genomeFileName, unsigned int n, unsigned int q, unsigned long long int codeTableSize, unsigned long long int dirTableSize, unsigned long long int posTableSize)
{
    ofstream outfile;
    outfile.open("open_" + genomeFileName + "_" + to_string(q) + ".txt", ios::out);

    vector<long long int> initCodeTable(codeTableSize);
    vector<unsigned long long int> initDirTable(dirTableSize);
    vector<unsigned long long int> initPosTable(posTableSize);

    dirTable = vector<unsigned long long int>(initDirTable.begin(), initDirTable.end());
    initDirTable.clear();
    posTable = vector<unsigned long long int>(initPosTable.begin(), initPosTable.end());
    initPosTable.clear();

    fill(initCodeTable.begin(), initCodeTable.end(), -1);
    fill(dirTable.begin(), dirTable.end(), 0);
    fill(posTable.begin(), posTable.end(), 0);

    vector<unsigned long long int> codeTableIndices(n - q + 1);
    int i;

    omp_lock_t codeLock;
    omp_init_lock(&codeLock);

    #pragma omp parallel for
    for (i = 0; i < n - q + 1; i++) {
        unsigned long long int rank = extractRanking(genome.substr(i, q));
        unsigned long long int hashValue = rank % (unsigned long long int)codeTableSize;

        omp_set_lock(&codeLock);
        if (initCodeTable.at(hashValue) != -1) {
            unsigned long long int j = hashValue;
            unsigned long long int k = 1;

            while (initCodeTable.at(j) != -1 && initCodeTable.at(j) != rank) {
                j = (j + (k * k)) % (unsigned long long int) codeTableSize;
                k = k + 1;
            }
            initCodeTable.at(j) = rank;
            dirTable.at(j)++;

            omp_unset_lock(&codeLock);

            codeTableIndices.at(i) = j;
        } else {
            initCodeTable.at(hashValue) = rank;
            dirTable.at(hashValue)++;

            omp_unset_lock(&codeLock);

            codeTableIndices.at(i) = hashValue;
        }
    }

    omp_destroy_lock(&codeLock);

    for (i = 1; i < dirTable.size(); i++) {
        dirTable.at(i) += dirTable.at(i-1);
    }

    #pragma omp parallel for
    for (i = n - q; i >= 0; i--) {
        unsigned long long int index = 0;

        #pragma omp critical
        {
            dirTable.at(codeTableIndices.at(i))--;
            index = dirTable.at(codeTableIndices.at(i));
        }
        posTable.at(index) = i;
    }

    outfile << "code" << endl;
    i = 0;
    auto it = initCodeTable.begin();
    while (it != initCodeTable.end()) {
        auto currIt = it++;

        if (*currIt != -1) {
            outfile << (unsigned long long int)*currIt << " " << i << endl;
            codeTable.insert(pair<long long, unsigned long long int>(*currIt, i));
        } else {
            outfile << *currIt << " " << i << endl;
        }

        i++;
    }

    outfile << endl << "dir" << endl;
    for (int i = 0; i < dirTable.size(); i++) {
        outfile << dirTable.at(i) << endl;
    }
    outfile << endl << "pos" << endl;
    for (int i = 0; i < posTable.size(); i++) {
        outfile << posTable.at(i) << endl;
    }
    outfile << endl;

    outfile.close();
}

/**
 * It initializes the table sizes and calls the building of the open addressing index.
 *
 * @param genome - The reference genome data.
 * @param genomeFileName - The filename of the reference genome.
 * @param loadFactor - The value of load factor that will be used for initializing the size of the code table.
 */
void buildOpenAddressingIndexingFile(string& genome, string& genomeFileName, double loadFactor) {
    unsigned long long int codeTableSize = floor(( pow(loadFactor, -1)) * genome.length());
    unsigned long long int dirTableSize = codeTableSize + 1;
    unsigned long long int posTableSize = genome.size() - q + 1;

    cout << "Open Addressing for q = " << q  << endl << endl;
    buildOpenAddressingTables(genome, genomeFileName, genome.length(), q, codeTableSize, dirTableSize, posTableSize);
}