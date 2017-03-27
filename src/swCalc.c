#include "swCalc.h"


struct matrix* swInitMat(char* s1, char *s2){
    struct matrix* m = mallocOrDie(sizeof(struct matrix), "alloc matrice");
    m->h = strlen(s1)+1;
    m->w = strlen(s2)+1;
    m->cells = mallocOrDie(m->w * m->h * sizeof(struct cell), "alloc cellules");
    for(unsigned int i = 0; i < m->h; i++){
        for(unsigned int j = 0; j< m->w; j++){
            m->cells[m->w*i + j].score = 0;
            m->cells[m->w*i + j].prevs = 0;
        }
    } 
    return m;
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
            /*
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
            */
        }
    }
}

void swFillMatAff(struct matrix *D, struct matrix *V, struct matrix *H, struct cost *cost, char *s1, char *s2){
  int dir=0;
  for(unsigned int i = 1; i < D->h; i++){
    for(unsigned int j = 1; j< D->w; j++){
		  double tempScore;
      double tempScoreD;
      double tempScoreV;
      double tempScoreH;

		  // quand on vient à la verticale
		  tempScoreD = D->cells[D->w*(i-1)+j].score + cost->indelOpen;
      if (i != 1) {
		    tempScoreV = V->cells[V->w*(i-1)+j].score + cost->indelExtend;
		  } else {
        tempScoreV = V->cells[V->w*(i-1)+j].score + cost->indelOpen;
      }
      tempScoreH = H->cells[H->w*(i-1)+j].score + cost->indelOpen;
		  tempScore = max(tempScoreD, tempScoreV, tempScoreH, dir);
      V->cells[V->w*i+j].score = tempScore;
		  if (dir == 0) {
        V->cells[V->w*i+j].prevs |= 1;
		  } else if (dir == 1) {
        V->cells[V->w*i+j].prevs |= 4;
		  } else {
        V->cells[V->w*i+j].prevs |= 2;
		  }
		  
		  // horizontale
		  tempScoreD = D->cells[D->w*i+(j-1)].score + cost->indelOpen;
		  tempScoreV = V->cells[V->w*i+(j-1)].score + cost->indelOpen;
      if (j != 1) {
        tempScoreH = H->cells[H->w*i+(j-1)].score + cost->indelExtend;
      } else {
        tempScoreH = H->cells[H->w*i+(j-1)].score + cost->indelOpen;
      }
		  tempScore = max(tempScoreD, tempScoreV, tempScoreH, dir);
      H->cells[H->w*i+j].score = tempScore;
		  if (dir == 0) {
        H->cells[V->w*i+j].prevs |= 1;
      } else if (dir == 1) {
        H->cells[V->w*i+j].prevs |= 4;
      } else {
        H->cells[V->w*i+j].prevs |= 2;
      }

		  // diagonale
		  tempScoreD = D->cells[D->w*(i-1)+(j-1)].score + cost->subst(s1[i-1], s2[j-1]);
		  tempScoreV = V->cells[V->w*(i-1)+(j-1)].score + cost->subst(s1[i-1], s2[j-1]);
		  tempScoreH = H->cells[H->w*(i-1)+(j-1)].score + cost->subst(s1[i-1], s2[j-1]);
		  tempScore = max(tempScoreD, tempScoreV, tempScoreH, dir);
      D->cells[D->w*i+j].score = tempScore;
      if (dir == 0) {
        D->cells[V->w*i+j].prevs |= 1;
      } else if (dir == 1) {
        D->cells[V->w*i+j].prevs |= 4;
      } else {
        D->cells[V->w*i+j].prevs |= 2;
	    }
    }
  }
}

double max(double d, double v, double h, int dir) {
  double maxi = d;
  dir = 0;
  if (v > maxi) {
    maxi = v;
    dir = 1;
  }
  if (h > maxi) {
    maxi = h;
    dir = 2;
  }
  if (maxi < 0) {
    maxi = 0;
  }
  return maxi;
}
