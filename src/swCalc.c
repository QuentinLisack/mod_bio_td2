#include "swCalc.h"


struct matrix *swInitMat(char* s1, char *s2){
    struct matrix *m;
    m = mallocOrDie(sizeof(struct matrix), "alloc matrice");
    m->h = strlen(s1);
    m->w = strlen(s2);
    m->cells = mallocOrDie(m->w * m->h * sizeof(struct cell), "alloc cellules");
    for(unsigned int i = 0; i < m->h; i++){
        for(unsigned int j = 0; j< m->h; j++){
            m->cells[m->w*i + j].score = 0;
            m->cells[m->w*i + j].prevs = 0;
        }
    } 
    return m;
  return NULL;
}

void swFreeMat(struct matrix *mat){
  /*if(mat != NULL){
        if(mat->cells != NULL){
            for(int i = 0; i < m->h; i++){
                for(int j = 0; j< m->h; j++){
                    cells[m->w*i + j] 
                }
            } 
	    }*/
}

void swPrintMat(struct matrix *mat) {
  printf("\n");
  for (unsigned int i=0; i<mat->w; i++){
    for (unsigned int j=0; j<mat->h; j++) {
      printf("%f  ",mat->cells[mat->w*i+j].score);
    } 
    printf("\n");
  }
}

void swFillMat(struct matrix *mat, struct cost *cost, char *s1, char *s2){

}
