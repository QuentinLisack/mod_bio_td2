#include <stdio.h>
#include <stdlib.h>

#include "swCost.h"
#include "swCalc.h"
#include "swGetSeq.h"
#include "swOut.h"

int main(void)
{
	char *s1 ;
	while((s1 = getSeq(0)) == NULL) {
		// nothing to do
	}
	char *s2 ;
	while((s2 = getSeq(0)) == NULL) {
	}
	printf("Sequences read:\ns1\t%s\ns2\t%s\n\n", s1, s2) ;

	/* affine cost for long gaps eg spliced RNA on genome */

	struct cost *cost = costDna(-100,-0.05);
	struct matrix *D = swInitMat(s1,s2);
	struct matrix *V = swInitMat(s1,s2);
	struct matrix *H = swInitMat(s1,s2);
	swFillMatAff(D, V, H, cost, s1, s2);

    //uncomment for debug : 
    
    printf("\n");
    printf("matrice D : \n");
	swPrintMat(D);
	printf("\n");
	printf("matrice V : \n");
	swPrintMat(V);
	printf("\n");
	printf("matrice H : \n");
	swPrintMat(H);
	printf("\n");
	/*
	printf("\n");
    printf("matrice D : \n");
	swPrintMatPrev(D);
	printf("\n");
	printf("matrice V : \n");
	swPrintMatPrev(V);
	printf("\n");
	printf("matrice H : \n");
	swPrintMatPrev(H);
	printf("\n");
	*/
	
	printBestAlisAff(D, V, H, cost, s1, s2);

	swFreeMat(D);
	swFreeMat(V);
	swFreeMat(H);
	free(cost);
	free(s1);
	free(s2);
	return(0);
}
