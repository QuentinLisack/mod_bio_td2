#ifndef _SWOUT_H_
#define _SWOUT_H_

#include "swCalc.h"
#include "swCost.h"
#include "mem.h"

/* Find highest scoring local alignment(s) in mat, and print to stdout
   the corresponding best alignments.
   mat must have been filled with scores and prevs.
   cost is provided so mismatches with negative scores can be lowercased.
*/
void printBestAlis(struct matrix *mat, struct cost *cost, char *s1, char *s2);
void printBestAlisAff(struct matrix *D, struct matrix *V, struct matrix *H, struct cost *cost, char *s1, char *s2);
void Calcul_BestScore(struct matrix *mat, double* BS, unsigned int* iBS, unsigned int* jBS, unsigned int *numMax) ;
void printCorrespondingSeq(struct matrix *mat, unsigned int i_BS, unsigned int j_BS, char* s1, char* s2, int* BestScore);
void printResults(char* s1_print, char* s2_print, int s1_start, int s2_start);
void printCorrespondingSeqAff(struct matrix *D, struct matrix *V, struct matrix *H, unsigned int i_BS, unsigned int j_BS, char* s1, char* s2, int* BestScore);

#endif
