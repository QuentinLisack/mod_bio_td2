#include "swOut.h"

//regarder comment avoir accès aux données avec gdb
// il reste essentiellement à remplir les chaines dans les if et à vérifier les conditions dans les if et while
void printBestAlis(struct matrix *mat, struct cost *cost, char *s1, char *s2) {
  double* BestScore = mallocOrDie(sizeof(double), "best score");
  *BestScore = 0;
  unsigned int* i_BS = mallocOrDie(sizeof(unsigned int), "i_BS");
  unsigned int* j_BS = mallocOrDie(sizeof(unsigned int), "j_BS");

  Calcul_BestScore(mat, BestScore, i_BS, j_BS);

  int LENGTH = strlen(s1) < strlen(s2) ? strlen(s2) : strlen(s1);
  char* s1_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s1 print");
  char* s2_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s2 print");
  
  getCorrespondingSeq(mat, &s1_print, &s2_print, i_BS, j_BS, s1, s2);

  int s1_start=(*i_BS); // à modifier quand le code sera bon
  int s2_start=(*j_BS);

  /* affichage des résultats */
  printResults(s1, s2, s1_print, s2_print, (*BestScore), s1_start, s2_start);

  free(BestScore);
  free(i_BS);
  free(j_BS);
  free(s1_print);
  free(s2_print);
}


/*void printBestAlisAff(struct matrix *D, struct matrix *V, struct matrix *H, struct cost *cost, char *s1, char *s2) {
  double* BestScore = mallocOrDie(sizeof(double), "best score");
  *BestScore = 0;
  unsigned int* i_BS = mallocOrDie(sizeof(unsigned int), "i_BS");
  unsigned int* j_BS = mallocOrDie(sizeof(unsigned int), "j_BS");

  Calcul_BestScoreAff(D, V, H, BestScore, i_BS, j_BS);

  int LENGTH = strlen(s1) < strlen(s2) ? strlen(s2) : strlen(s1);
  char* s1_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s1 print");
  char* s2_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s2 print");
  
  getCorrespondingSeqAff(D, V, H, &s1_print, &s2_print, i_BS, j_BS, s1, s2);

  int s1_start=(*i_BS); // à modifier quand le code sera bon
  int s2_start=(*j_BS);

  /* affichage des résultats */
/* printResults(s1, s2, s1_print, s2_print, (*BestScore), s1_start, s2_start);

  free(BestScore);
  free(i_BS);
  free(j_BS);
  free(s1_print);
  free(s2_print);
}*/


void printResults(char* s1, char* s2, char* s1_print, char* s2_print, int bestScore, int s1_start, int s2_start){
  printf("Best score is %.1f, the best-scoring alignments are:\n\n ", bestScore);
  printf("s1 alignment starts at coord %d\n s2 alignment starts at coord %d\n\n",s1_start,s2_start);
  printf("les sequences alignees sont : \n");
  printf("%s\n", s1_print);
  printf("%s\n", s2_print);
  
  printf("\n");
  printf("les sequences initiales sont : \n");
  printf("s1	%s\n",s1);
  printf("s2	%s\n",s2);
}

void Calcul_BestScore(struct matrix *mat, double* BS, unsigned int* iBS, unsigned int* jBS) {
  int sc;
  /* on recherche le score maximal */
  for (unsigned int i=0; i<mat->h; i++){
    for (unsigned int j=0; j<mat->w; j++) {
      sc=mat->cells[mat->w*i+j].score;
      if (sc >= (*BS)) {
  (*BS)=sc;
  (*iBS)=i;
  (*jBS)=j;
      } 
    }
  }
}

/*void Calcul_BestScoreAff(struct matrix *D, struct matrix *V, struct matrix *H, double* BS, unsigned int* iBS, unsigned int* jBS) {
  int sc;
  /* on recherche le score maximal */
/* for (unsigned int i=0; i<D->h; i++){
    for (unsigned int j=0; j<D->w; j++) {
      sc=D->cells[D->w*i+j].score;
      if (sc >= (*BS)) {
	(*BS)=sc;
	(*iBS)=i;
	(*jBS)=j;
      }
      sc=V->cells[V->w*i+j].score;
      if (sc >= (*BS)) {
	(*BS)=sc;
	(*iBS)=i;
	(*jBS)=j;
      } 
      sc=H->cells[H->w*i+j].score;
      if (sc >= (*BS)) {
	(*BS)=sc;
	(*iBS)=i;
	(*jBS)=j;
      } 
    }
  }
}*/

void getCorrespondingSeq(struct matrix *mat, char** s1_print, char** s2_print, unsigned int* i_BS, unsigned int* j_BS, char* s1, char* s2){
  int *s1_align_int = mallocOrDie(strlen(s1)*sizeof(int), "alloc error for s1 align");
  int *s2_align_int = mallocOrDie(strlen(s2)*sizeof(int), "alloc error for s2 align");
  uint8_t* prev = mallocOrDie(sizeof(uint8_t), "prev");
  struct cell cell;
  
  //initialisation des tableaux pour les alignements
  for(unsigned int p = 0; p < strlen(s1); p++){
  	s1_align_int[p] = 0;
  }
  for(unsigned int p = 0; p < strlen(s2); p++){
  	s2_align_int[p] = 0;
  }
  
  cell=mat->cells[mat->w*(*i_BS)+(*j_BS)];
  (*prev)=cell.prevs;
  
  //recherche de l'alignement
  while(cell.prevs!=0) {
    if ((*prev)&1) {
      (*i_BS)-=1; 
      (*j_BS)-=1;
      s1_align_int[(*i_BS)] = 1;
      s2_align_int[(*j_BS)] = 1;
      printf("diag\n");
    }
    else if ((*prev)&2) {
      (*j_BS)-=1;
      s2_align_int[(*j_BS)] = -1;
      printf("top\n");
    }
      else if ((*prev)&4){
		(*i_BS)-=1;
		s1_align_int[(*i_BS)] = -1;
		printf("left\n");
    }
    cell=mat->cells[mat->w*(*i_BS)+(*j_BS)];
    (*prev)=cell.prevs;
  }

  //on remplit les séquences à afficher avec une bonne gestion des indel
  int LENGTH = strlen(s1) < strlen(s2) ? strlen(s2) : strlen(s1);

  for(int i = 0; i < strlen(s1); i++){
    printf("%d  ", s1_align_int[i]);
  }
  printf("\n");
  for(int i = 0; i < strlen(s2); i++){
    printf("%d  ", s2_align_int[i]);
  }
  printf("\n");

  int p1 = 0, p2 = 0;
  int seqBegin = 0;
  for(int p = 0; p < LENGTH; p++){
    if(s1_align_int[p1] == -1){
      seqBegin = 1;
      (*s1_print)[p] = s1[p1];
      (*s2_print)[p] = '-';
      p1++;
    }else if(s2_align_int[p2] == -1){
      seqBegin = 1;
      (*s1_print)[p] = '-';
      (*s2_print)[p] = s2[p2];
      p2++;
    }else if(s1_align_int[p1] == 1 && s2_align_int[p2] == 1){
      seqBegin = 1;
      (*s1_print)[p] = s1[p1];
      (*s2_print)[p] = s2[p2];
      p1++;
      p2++;
    }else if(s1_align_int[p1] == 0 && s2_align_int[p2] == 0){
      if(seqBegin){
        break;
      }else{
        p1++;
        p2++;
      }
    }else if(s1_align_int[p1] == 0){
      p1++;
    }else if(s2_align_int[p2] == 0){
      p2++;
    }else{
      printf("PROBLEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEME\n");
      printf("p1 = %d, p2 = %d, s1 = %d, s2 = %d\n", p1, p2, s1_align_int[p1], s2_align_int[p2]);
    }
  }
  
  free(s1_align_int);
  free(s2_align_int);
  free(prev);
}
