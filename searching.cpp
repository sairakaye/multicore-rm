//
// Created by saimanalili on 25/02/2020.
//

#include "searching.h"
#include "pigeonhole.h"

void searchingUsingMinimizers(string seed, int adjustmentValue, int k, vector<unsigned long long int>& foundLocations,
    vector<unsigned long long int>& location) {
    int i;
    for (i = 0; i < location.size(); i++) {
        if (seed.compare(refGenome.genomeData.substr(location[i], q)) == 0) {
            if ((location[i] - adjustmentValue) >= 0 && (location[i] - adjustmentValue) < refGenome.genomeData.size()) {
                    foundLocations.push_back(location[i] - adjustmentValue);
            }
        }
    }
}

void approximateSearchingUsingMinimizers(string seed, int adjustmentValue, int k, vector<unsigned long long int>& foundLocations,
    vector<unsigned long long int>& location) {
    int i;
    for (i = 0; i < location.size(); i++) {
        EdlibAlignResult result = edlibAlign(refGenome.genomeData.substr(location[i], q).c_str(), q,
                seed.c_str(), seed.length(), edlibDefaultAlignConfig());

        if (result.editDistance <= allowableE) {
            if ((location[i] - adjustmentValue) >= 0 && (location[i] - adjustmentValue) < refGenome.genomeData.size()) {
                foundLocations.push_back(location[i] - adjustmentValue);
            }
        }

        edlibFreeAlignResult(result);
    }
}

void searchingUsingDirectOrOpen(string seed, unsigned long long int index, string mode, int adjustmentValue, int k, vector<unsigned long long int>& foundLocations,
                                vector<unsigned long long int>& location) {
    while (seed.compare(refGenome.genomeData.substr(posTable[index], q)) == 0) {
        if ((posTable[index] - adjustmentValue) >= 0 && (posTable[index] - adjustmentValue) < refGenome.genomeData.size()) {
            foundLocations.push_back(posTable[index] - adjustmentValue);
        }

        index++;
    }
}

void approximateSearchingUsingDirectOrOpen(string seed, unsigned long long int index, string mode, int adjustmentValue, int k, vector<unsigned long long int>& foundLocations,
    vector<unsigned long long int>& location) {
    bool continueCompare = true;

    while (continueCompare) {
        if (index < posTable.size()) {
            EdlibAlignResult result = edlibAlign(refGenome.genomeData.substr(posTable[index], q).c_str(),
                                                 q, seed.c_str(), seed.length(), edlibDefaultAlignConfig());

            if (result.editDistance <= allowableE) {
                if ((posTable[index] - adjustmentValue) >= 0 && (posTable[index] - adjustmentValue) < refGenome.genomeData.size()) {
                    foundLocations.push_back(posTable[index] - adjustmentValue);
                }

                index++;
            } else {
                continueCompare = false;
            }

            edlibFreeAlignResult(result);
        } else {
            continueCompare = false;
        }
    }
}

void exactSearchingPosition(string seed, string mode, int adjustmentValue, int k, vector<unsigned long long int>& foundLocations) {
    vector<unsigned long long int> location;

    unsigned long long int rank;
    unsigned long long int index;

    if (mode.compare("min") == 0) {
        rank = getMinimizerRank(seed, q, w);
        location = minimizers[rank];

        if (location.size() > 0) {
            searchingUsingMinimizers(seed.substr(0, q), adjustmentValue, k, foundLocations, location);
        }
    } else if (mode.compare("dir") == 0 || mode.compare("open") == 0) {
        if (mode.compare("dir") == 0) {
            rank = extractRanking(seed);

            if (rank >= 0) {
                index = dirTable[rank];

                if (index < posTable.size()) {
                    searchingUsingDirectOrOpen(seed, index, mode, adjustmentValue, k, foundLocations, location);
                }
            }
        } else {
            rank = extractRanking(seed);

            try {
                index = dirTable[codeTable[rank]];

                searchingUsingDirectOrOpen(seed, index, mode, adjustmentValue, k, foundLocations, location);
            } catch (exception& e) { }
        }
    }
}

void approximateSearchingPosition(string seed, string mode, int adjustmentValue, int k, vector<unsigned long long int>& foundLocations) {
    vector<unsigned long long int> location;

    unsigned long long int rank;
    unsigned long long int index;

    if (mode.compare("min") == 0) {
        rank = getMinimizerRank(seed, q, w);
        location = minimizers[rank];

        if (location.size() > 0) {
            approximateSearchingUsingMinimizers(seed.substr(0, q), adjustmentValue, k, foundLocations, location);
        }
    } else if (mode.compare("dir") == 0 || mode.compare("open") == 0) {
        if (mode.compare("dir") == 0) {
            rank = extractRanking(seed);

            if (rank >= 0) {
                index = dirTable[rank];

                if (index < posTable.size()) {
                    approximateSearchingUsingDirectOrOpen(seed, index, mode, adjustmentValue, k, foundLocations, location);
                }
            }
        } else {
            rank = extractRanking(seed);

            try {
                index = dirTable[codeTable[rank]];

                approximateSearchingUsingDirectOrOpen(seed, index, mode, adjustmentValue, k,foundLocations, location);
            } catch (exception& e) { }
        }
    }
}