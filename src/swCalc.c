#include "swCalc.h"


void swInitMat(char* s1, char *s2, struct matrix *m){
    m = mallocOrDie(sizeof(struct matrix), "alloc matrice");
    m->h = strlen(s1)+1;
    m->w = strlen(s2)+1;
    m->cells = mallocOrDie(m->w * m->h * sizeof(struct cell), "alloc cellules");
    for(unsigned int i = 0; i < m->h; i++){
        for(unsigned int j = 0; j< m->w; j++){
            m->cells[m->w*i + j].score = 0;
            m->cells[m->w*i + j].prevs = 0;
        }
    } 
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
  for (unsigned int i=0; i<mat->h; i++){
    for (unsigned int j=0; j<mat->w; j++) {
      printf("%.1f  ",mat->cells[mat->w*i+j].score);
    } 
    printf("\n");
  }
  printf("\n");
}

void swFillMat(struct matrix *mat, struct cost *cost, char *s1, char *s2){
    for(unsigned int i = 1; i < mat->h; i++){
        for(unsigned int j = 1; j< mat->w; j++){
	  // quand on vient à la verticale
	  int tempScore = mat->cells[mat->w*(i-1)+j].score + cost->indelOpen;
	  mat->cells[mat->w*i+j].score = tempScore;
	  mat->cells[mat->w*i+j].prevs |= 4;
	  // horizontale
	  tempScore = mat->cells[mat->w*i+(j-1)].score + cost->indelOpen;
	  if(tempScore > mat->cells[mat->w*i+j].score){
	    mat->cells[mat->w*i+j].score = tempScore;
	    mat->cells[mat->w*i+j].prevs |= 2;
	    mat->cells[mat->w*i+j].prevs &= ~4;
	  } else if (tempScore == mat->cells[mat->w*i+j].score){
	    mat->cells[mat->w*i+j].prevs |= 2;
	  }
	  // diagonale
	  tempScore = mat->cells[mat->w*(i-1)+(j-1)].score + cost->subst(s1[i-1], s2[j-1]);
            if(tempScore > mat->cells[mat->w*i+j].score){
                mat->cells[mat->w*i+j].score = tempScore;
                mat->cells[mat->w*i+j].prevs |= 1;
                mat->cells[mat->w*i+j].prevs &= ~2;
                mat->cells[mat->w*i+j].prevs &= ~4;
            } else if(tempScore == mat->cells[mat->w*i+j].score){
                mat->cells[mat->w*i+j].prevs |= 1;
            }
            if(mat->cells[mat->w*i+j].score < 0){
                mat->cells[mat->w*i+j].score = 0;
                mat->cells[mat->w*i+j].prevs = 0;
            }
            //print de debug
            printf("ligne : %d ; colonne : %d\n", j, i);
            printf("score : %f\n", mat->cells[mat->w*i+j].score);
            if(mat->cells[mat->w*i+j].prevs & 1){
            	printf("diag avec les char %c %c\n", s1[i-1], s2[j-1]);
            }
            if(mat->cells[mat->w*i+j].prevs & 2){
            	printf("left avec les char %c %c\n", s1[i-1], s2[j-1]);
            }
            if(mat->cells[mat->w*i+j].prevs & 4){
            	printf("top avec les char %c %c\n", s1[i-1], s2[j-1]);
            }
        }
    }
}

void swFillMatAff(struct matrix *D, struct matrix *V, struct matrix *H, struct cost *cost, char *s1, char *s2){
  int* dir = mallocOrDie(sizeof(int), "dir");
    for(unsigned int i = 1; i < D->h; i++){
        for(unsigned int j = 1; j< D->w; j++){
		  int tempScore;
		  // quand on vient à la verticale
		  int tempScoreD = D->cells[D->w*(i-1)+j].score;
		  int tempScoreV = V->cells[V->w*(i-1)+j].score;
		  int tempScoreH = H->cells[H->w*(i-1)+j].score;
		  tempScore = max(tempScoreD, tempScoreV, tempScoreH, dir);
		  if (*dir == 0) {
		  	tempScore += cost->indelOpen;
		  } else if (*dir == 1) {
		  	tempScore += cost->indelExtend;
		  } else {
		  	tempScore += cost->indelOpen;
		  }
		  V->cells[V->w*i+j].score = tempScore;
		  V->cells[V->w*i+j].prevs |= 4;
		  // horizontale
		  tempScoreD = D->cells[D->w*i+(j-1)].score;
		  tempScoreV = V->cells[V->w*i+(j-1)].score;
		  tempScoreH = H->cells[H->w*i+(j-1)].score;
		  tempScore = max(tempScoreD, tempScoreV, tempScoreH, dir);
		  if (*dir == 0) {
		  	tempScore += cost->indelOpen;
		  } else if (*dir == 1) {
		  	tempScore += cost->indelOpen;
		  } else {
		  	tempScore += cost->indelExtend;
		  }
		  if(tempScore > H->cells[H->w*i+j].score){
		    H->cells[H->w*i+j].score = tempScore;
		    H->cells[H->w*i+j].prevs |= 2;
		    H->cells[H->w*i+j].prevs &= ~4;
		  } else if(tempScore == H->cells[H->w*i+j].score){
		    H->cells[H->w*i+j].prevs |= 2;
		  }
		  // diagonale
		  tempScoreD = D->cells[D->w*(i-1)+(j-1)].score;
		  tempScoreV = V->cells[V->w*(i-1)+(j-1)].score;
		  tempScoreH = H->cells[H->w*(i-1)+(j-1)].score;
		  tempScore = max(tempScoreD, tempScoreV, tempScoreH, dir) + cost->subst(s1[i-1], s2[j-1]);
	      if(tempScore > D->cells[D->w*i+j].score){
	        D->cells[D->w*i+j].score = tempScore;
	        D->cells[D->w*i+j].prevs |= 1;
	        D->cells[D->w*i+j].prevs &= ~2;
	        D->cells[D->w*i+j].prevs &= ~4;
	      } else if(tempScore == D->cells[D->w*i+j].score){
	        D->cells[D->w*i+j].prevs |= 1;
	      }

	      if(D->cells[D->w*i+j].score < 0){
	        D->cells[D->w*i+j].score = 0;
	        D->cells[D->w*i+j].prevs = 0;
	       }
	    }
	    free(dir);
	}
}

int max(int a, int b, int c, int* dir) {
  int maxi = a;
  *dir = 0;
  if (b > maxi) {
    maxi = b;
    *dir = 1;
  }
  if (c > maxi) {
    maxi = c;
    *dir = 2;
  }
  if (maxi < 0) {
  	maxi = 0;
  }
  return maxi;
}
