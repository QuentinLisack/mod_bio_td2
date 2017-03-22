#include "swOut.h"

//regarder comment avoir accès aux données avec gdb
// il reste essentiellement à remplir les chaines dans les if et à vérifier les conditions dans les if et while
void printBestAlis(struct matrix *mat, struct cost *cost, char *s1, char *s2) {
  double* BestScore = mallocOrDie(sizeof(double), "best score");
  *BestScore = 0;
  unsigned int* i_BS = mallocOrDie(sizeof(unsigned int), "i_BS");
  unsigned int* j_BS = mallocOrDie(sizeof(unsigned int), "j_BS");
  uint8_t* prev = mallocOrDie(sizeof(uint8_t), "prev");
  struct cell cell;

  Calcul_BestScore(mat, BestScore, i_BS, j_BS);
  
  /* on alloue les 2 chaines : pour les remplir, 
  on les remplit de la fin (\0) vers le début sinon faudrait les inverser à la 
  fin vu qu'on effectue le parcours depuis la fin de la séquence choisie) 
  en choisissant le meilleur chemin calculé précédemment dans prevs*/
  // mat->cells[i].prevs
  cell=mat->cells[mat->w*(*i_BS)+(*j_BS)];
  (*prev)=cell.prevs;
  int *s1_align_int = NULL;
  int *s2_align_int = NULL;
  s1_align_int = mallocOrDie(strlen(s1)*sizeof(int), "alloc error for s1 align");
  s2_align_int = mallocOrDie(strlen(s2)*sizeof(int), "alloc error for s2 align");
  
  //initialisation des chaines. 0 signifie que le caractère n'es pas considéré dans la chaine : on n'affichera rien. 
  // 1 signifie qu'il fait partie de la chaine : on affichera le caractère
  // -1 signifie qu'il a été supprimé lors d'une indel : on affichera -
  for(unsigned int p = 0; p < strlen(s1); p++){
  	s1_align_int[p] = 0;
  }
  
  for(unsigned int p = 0; p < strlen(s2); p++){
  	s2_align_int[p] = 0;
  }
  
  while(cell.prevs!=0) { // pas du tout sûr!!
    //i_BS=mat-
    if ((*prev)&1) {
      (*i_BS)-=1; 
      (*j_BS)-=1;
      s1_align_int[(*i_BS)] = 1;
      s2_align_int[(*j_BS)] = 1;
      printf("diag\n");
    }
    else if ((*prev)&2) { // là on doit remplir la chaine
      (*j_BS)-=1;
      s2_align_int[(*j_BS)] = -1;
      printf("top\n");
    }
      else if ((*prev)&4){
		(*i_BS)-=1; //cmt on sait ?
		s1_align_int[(*i_BS)] = -1;
		printf("left\n");
    }
    cell=mat->cells[mat->w*(*i_BS)+(*j_BS)];
    (*prev)=cell.prevs;
  }


  int s1_start=(*i_BS); // à modifier quand le code sera bon
  int s2_start=(*j_BS);

  //on remplit les séquences à afficher avec une bonne gestion des indel
  int LENGTH = strlen(s1) < strlen(s2) ? strlen(s2) : strlen(s1);
  char* s1_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s1 print");
  char* s2_print = mallocOrDie(LENGTH*sizeof(char), "alloc error for s2 print");

  int p1 = 0, p2 = 0;
  for(int p = 0; p < LENGTH; p++){
    if(s1_align_int[p1] == -1){
      s1_print[p] = s1[p1];
      s2_print[p] = '-';
      p1++;
    }else if(s2_align_int[p2] == -1){
      s1_print[p] = '-';
      s2_print[p] = s2[p2];
      p2++;
    }else if(s1_align_int[p1] == 1 && s2_align_int[p2] == 1){
      s1_print[p] = s1[p1];
      s2_print[p] = s2[p2];
      p1++;
      p2++;
    }else if(s1_align_int[p1] == 0 && s2_align_int[p2] == 0){
      break;
    }else{
      printf("PROBLEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEME\n");
    }
  }

  /* affichage des résultats */
  printf("Best score is %.1f, the best-scoring alignments are:\n\n ",(*BestScore));
  printf("s1 alignment starts at coord %d\n s2 alignment starts at coord %d\n\n",s1_start,s2_start);
  printf("les sequences alignees sont : \n");
  printf("%s\n", s1_print);
  printf("%s\n", s2_print);
  
  printf("\n");
  printf("les sequences initiales sont : \n");
  printf("s1	%s\n",s1);
  printf("s2	%s\n",s2);

  free(s1_align_int);
  free(s2_align_int);
  free(BestScore);
  free(i_BS);
  free(j_BS);
  free(prev);
  free(s1_print);
  free(s2_print);
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
