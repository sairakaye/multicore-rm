//
// Created by saimanalili on 17/04/2020.
//

#ifndef SALAINDNA_OUTPUT_H
#define SALAINDNA_OUTPUT_H

#include "common.h"

void outputPairReads(string& genomeFileName);
void outputRunTimeResults(string& genomeFileName, double indexTimeTaken, double ssTimeTaken, double bmTimeTaken,
                          double verificationTimeTaken, double totalTimeTaken);
void outputSeedSelectorResults(string& mainName, double timeTaken);
void outputFileSeedSelectorResults(string& mainName, double timeTaken);
void outputPrealignmentResults();
void outputSAMFile();
void outputEdlibResults();

#endif //SALAINDNA_OUTPUT_H
