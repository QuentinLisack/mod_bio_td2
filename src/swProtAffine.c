#include <stdio.h>
#include <stdlib.h>

#include "swCost.h"
#include "swCalc.h"
#include "swGetSeq.h"
#include "swOut.h"

int main(void)
{
	char *s1 ;
	while((s1 = getSeq(1)) == NULL) {
		// nothing to do
	}
	char *s2 ;
	while((s2 = getSeq(1)) == NULL) {
	}
	printf("Sequences read:\ns1\t%s\ns2\t%s\n\n", s1, s2) ;

	/* BLOSUM62 prot subst cost with affine cost for short indels */
	struct cost *cost = costProt(-10,-0.5);
	struct matrix *D = swInitMat(s1,s2);
	struct matrix *V = swInitMat(s1,s2);
	struct matrix *H = swInitMat(s1,s2);
	swFillMatAff(D, V, H, cost, s1, s2);

	printBestAlisAff(D, V, H, cost, s1, s2);

	swFreeMat(D);
	swFreeMat(V);
	swFreeMat(H);
	free(cost);
	free(s1);
	free(s2);
	return(0);
}
