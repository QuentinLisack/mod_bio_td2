#include "swOut.h"

//regarder comment avoir accès aux données avec gdb
// il reste essentiellement à remplir les chaines dans les if et à vérifier les conditions dans les if et while
void printBestAlis(struct matrix *mat, struct cost *cost, char *s1, char *s2) {
  double* BestScore = mallocOrDie(sizeof(double), "best score");
  *BestScore = 0;
  unsigned int* numMax = mallocOrDie(sizeof(unsigned int), "numMax");
  *numMax = 0;
  unsigned int* i_BS = mallocOrDie(sizeof(unsigned int), "i_BS");
  unsigned int* j_BS = mallocOrDie(sizeof(unsigned int), "j_BS");

  Calcul_BestScore(mat, BestScore, i_BS, j_BS, numMax);
  
  for(int p = 0; p < (*numMax); p++){
    printCorrespondingSeq(mat, i_BS[p], j_BS[p], s1, s2, BestScore);
  }
  
  printf("Best score is %f \n ", *BestScore);
  printf("\n");
  printf("les sequences initiales sont : \n");
  printf("s1  %s\n",s1);
  printf("\n");
  printf("s2  %s\n",s2);

  free(BestScore);
  free(i_BS);
  free(j_BS);
}

void printBestAlisAff(struct matrix *D, struct matrix *V, struct matrix *H, struct cost *cost, char *s1, char *s2) {
  double* BestScore = mallocOrDie(sizeof(double), "best score");
  *BestScore = 0;
  unsigned int* numMax = mallocOrDie(sizeof(unsigned int), "numMax");
  *numMax = 0;
  unsigned int* i_BS = mallocOrDie(sizeof(unsigned int), "i_BS");
  unsigned int* j_BS = mallocOrDie(sizeof(unsigned int), "j_BS");

  Calcul_BestScore(D, BestScore, i_BS, j_BS, numMax);
  
  for(int p = 0; p < (*numMax); p++){
    printCorrespondingSeqAff(D, V, H, i_BS[p], j_BS[p], s1, s2, BestScore);
  }
  
  printf("Best score is %.1f\n ", *BestScore);
  printf("\n");
  printf("les sequences initiales sont : \n");
  printf("s1  %s\n",s1);
  printf("\n");
  printf("s2  %s\n",s2);

  free(BestScore);
  free(i_BS);
  free(j_BS);
}

void printResults(char* s1_print, char* s2_print, int s1_start, int s2_start){
  printf("s1 alignment starts at coord %d\n s2 alignment starts at coord %d\n\n", s1_start, s2_start);
  printf("les sequences alignees sont : \n");
  printf("%s\n", s1_print);
  printf("\n");
  printf("%s\n", s2_print);
}

void Calcul_BestScore(struct matrix *mat, double* BS, unsigned int* iBS, unsigned int* jBS, unsigned int *numMax) {
  double sc;
  /* on recherche le score maximal */
  for (unsigned int i=0; i<mat->h; i++){
    for (unsigned int j=0; j<mat->w; j++) {
      sc=mat->cells[mat->w*i+j].score;
      if (sc >= (*BS)) {
        (*BS)=sc;
      } 
    }
  }
  
  /*on trouve toutes les cellules avec les scores max*/
  for (unsigned int i=0; i<mat->h; i++){
    for (unsigned int j=0; j<mat->w; j++) {
      sc=mat->cells[mat->w*i+j].score;
      if (sc == (*BS)) {
        (*numMax)++;
        reallocOrDie(iBS, (*numMax) * sizeof(unsigned int), "i_BS");
        reallocOrDie(jBS, (*numMax) * sizeof(unsigned int), "j_BS");
        iBS[(*numMax)-1] = i;
        jBS[(*numMax)-1] = j;
      } 
    }
  }
}

void printCorrespondingSeqAff(struct matrix *D, struct matrix *V, struct matrix *H, unsigned int i_BS, unsigned int j_BS, char* s1, char* s2, int* BestScore){
  int *s1_align_int = mallocOrDie(strlen(s1)*sizeof(int), "alloc error for s1 align");
  int *s2_align_int = mallocOrDie(strlen(s2)*sizeof(int), "alloc error for s2 align");
  struct cell cell;
  
  //initialisation des tableaux pour les alignements
  for(unsigned int p = 0; p < strlen(s1); p++){
    s1_align_int[p] = 0;
  }
  for(unsigned int p = 0; p < strlen(s2); p++){
    s2_align_int[p] = 0;
  }
  
  cell=D->cells[D->w*i_BS+j_BS];
  uint8_t prev = 1;

  //recherche de l'alignement
  while(cell.score > 0) {
    if (prev & 1) {
      //printf("coucou");
      s1_align_int[i_BS-1] = 1;
      s2_align_int[j_BS-1] = 1;
      i_BS-=1; 
      j_BS-=1;
    } else if (prev & 4) {
      s1_align_int[i_BS-1] = -1;
      i_BS-=1;
    } else if (prev & 2){
      s2_align_int[j_BS-1] = -1;
      j_BS-=1;
    }
    prev = cell.prevs;
    if(prev & 1){
      cell=D->cells[D->w*i_BS+j_BS];
    } else if(prev & 4){
      cell=V->cells[V->w*i_BS+j_BS];
    } else if(prev & 2){
      cell=H->cells[H->w*i_BS+j_BS];
    } else {
      break;
    }
  }

  //on remplit les séquences à afficher avec une bonne gestion des indel
  int LENGTH = strlen(s1) < strlen(s2) ? strlen(s2) : strlen(s1);
  char* s1_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s1 print");
  char* s2_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s2 print");
  
  int p1 = 0, p2 = 0;
  int reslength = 0;
  int seqBegin = 0;
  for(int p = 0; p < LENGTH; p++){
    if(s1_align_int[p1] == -1){
      seqBegin = 1;
      s1_print[reslength] = s1[p1];
      s2_print[reslength] = '-';
      reslength++;
      p1++;
    }else if(s2_align_int[p2] == -1){
      seqBegin = 1;
      s1_print[reslength] = '-';
      s2_print[reslength] = s2[p2];
      reslength++;
      p2++;
    }else if(s1_align_int[p1] == 1 && s2_align_int[p2] == 1){
      seqBegin = 1;
      s1_print[reslength] = s1[p1];
      s2_print[reslength] = s2[p2];
      reslength++;
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
  
  //printf("s1_print %s\n", s1_print);
  
  int s1_start=i_BS + 1; // à modifier quand le code sera bon
  int s2_start=j_BS + 1;

  /* affichage des résultats */
  printResults(s1_print, s2_print, s1_start, s2_start);
  
  free(s1_align_int);
  free(s2_align_int);
  free(s1_print);
  free(s2_print);
}

void printCorrespondingSeq(struct matrix *mat, unsigned int i_BS, unsigned int j_BS, char* s1, char* s2, int* BestScore){
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
  
  cell=mat->cells[mat->w*i_BS+j_BS];
  (*prev)=cell.prevs;
  
  //recherche de l'alignement
  while(cell.prevs!=0) {
    if ((*prev)&1) {
      i_BS-=1; 
      j_BS-=1;
      s1_align_int[i_BS] = 1;
      s2_align_int[j_BS] = 1;
      //printf("diag\n");
    }
    else if ((*prev)&2) {
      j_BS-=1;
      s2_align_int[j_BS] = -1;
      //printf("top\n");
    }
      else if ((*prev)&4){
    i_BS-=1;
    s1_align_int[i_BS] = -1;
    //printf("left\n");
    }
    cell=mat->cells[mat->w*i_BS+j_BS];
    (*prev)=cell.prevs;
  }

  //on remplit les séquences à afficher avec une bonne gestion des indel
  int LENGTH = strlen(s1) < strlen(s2) ? strlen(s2) : strlen(s1);
  char* s1_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s1 print");
  char* s2_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s2 print");

  /*
  for(int i = 0; i < strlen(s1); i++){
    printf("%d  ", s1_align_int[i]);
  }
  printf("\n");
  for(int i = 0; i < strlen(s2); i++){
    printf("%d  ", s2_align_int[i]);
  }
  printf("\n");
  */

  int p1 = 0, p2 = 0;
  int reslength = 0;
  int seqBegin = 0;
  for(int p = 0; p < LENGTH; p++){
    if(s1_align_int[p1] == -1){
      seqBegin = 1;
      s1_print[reslength] = s1[p1];
      s2_print[reslength] = '-';
      reslength++;
      p1++;
    }else if(s2_align_int[p2] == -1){
      seqBegin = 1;
      s1_print[reslength] = '-';
      s2_print[reslength] = s2[p2];
      reslength++;
      p2++;
    }else if(s1_align_int[p1] == 1 && s2_align_int[p2] == 1){
      seqBegin = 1;
      s1_print[reslength] = s1[p1];
      s2_print[reslength] = s2[p2];
      reslength++;
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
    }
  }
  
  int s1_start=i_BS + 1; 
  int s2_start=j_BS + 1;

  /* affichage des résultats */
  printResults(s1_print, s2_print, s1_start, s2_start);
  
  free(s1_align_int);
  free(s2_align_int);
  free(prev);
  free(s1_print);
  free(s2_print);
}
