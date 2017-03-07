#include "swCalc.h"


struct matrix *swInitMat(char* s1, char *s2){
    struct matrix *m;
    m = mallocOrDie(sizeof(struct matrix), "alloc matrice");
    m->h = strlen(s1);
    m->w = strlen(s2);
    m->cells = mallocOrDie(m->w * m->h * sizeof(struct cell), "alloc cellules");
    for(unsigned int i = 0; i < m->h; i++){
        for(unsigned int j = 0; j< m->w; j++){
            m->cells[m->w*i + j].score = 0;
            m->cells[m->w*i + j].prevs = 0;
        }
    } 
    return m;
  return NULL;
}

void swFreeMat(struct matrix *mat){
    if(mat != NULL){
        if(mat->cells != NULL){
            free(mat->cells);
        }
        free(mat);
    }
}

void swPrintMat(struct matrix *mat) {
  printf("\n");
  for (unsigned int i=0; i<mat->w; i++){
    for (unsigned int j=0; j<mat->h; j++) {
      printf("%f  ",mat->cells[mat->w*i+j].score);
    } 
    printf("\n");
  }
  printf("\n");
}

void swFillMat(struct matrix *mat, struct cost *cost, char *s1, char *s2){
    for(unsigned int i = 1; i < mat->h; i++){
        for(unsigned int j = 1; j< mat->w; j++){
            int tempScore = mat->cells[mat->w*(i-1)+j].score + cost->indelOpen;
            mat->cells[mat->w*i+j].score = tempScore;
            mat->cells[mat->w*i+j].prevs |= 4;
            tempScore = mat->cells[mat->w*i+(j-1)].score + cost->indelOpen;
            if(tempScore > mat->cells[mat->w*i+j].score){
                mat->cells[mat->w*i+j].score = tempScore;
                mat->cells[mat->w*i+j].prevs |= 2;
                mat->cells[mat->w*i+j].prevs &= ~4;
            }else if(tempScore = mat->cells[mat->w*i+j].score){
                mat->cells[mat->w*i+j].prevs |= 2;
            }
            tempScore = mat->cells[mat->w*(i-1)+(j-1)].score + cost->subst(s1[i], s2[j]);
            if(tempScore > mat->cells[mat->w*i+j].score){
                mat->cells[mat->w*i+j].score = tempScore;
                mat->cells[mat->w*i+j].prevs |= 1;
                mat->cells[mat->w*i+j].prevs &= ~2;
                mat->cells[mat->w*i+j].prevs &= ~4;
            }else if(tempScore = mat->cells[mat->w*i+j].score){
                mat->cells[mat->w*i+j].prevs |= 1;
            }
            if(mat->cells[mat->w*i+j].score < 0){
                mat->cells[mat->w*i+j].score = 0;
                mat->cells[mat->w*i+j].prevs = 0;
            }
        }
    }
}
