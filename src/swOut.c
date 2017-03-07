#include "swOut.h"

//regarder comment avoir accès aux données avec gdb
// il reste essentiellement à remplir les chaines dans les if et à vérifier les conditions dans les if et while
void printBestAlis(struct matrix *mat, struct cost *cost, char *s1, char *s2) {
  double BestScore=0;
  unsigned int i_BS;
  unsigned int j_BS;
  int sc;
  uint8_t prev;
  struct cell cell;
  /* on recherche le score maximal */
  for (unsigned int i=0; i<mat->h; i++){
    for (unsigned int j=0; j<mat->w; j++) {
      sc=mat->cells[mat->w*i+j].score;
      if (sc>BestScore) {
	BestScore=sc;
	i_BS=i;
	j_BS=j;
      } 
    }
  }
  
  /* on alloue les 2 chaines : pour les remplir, on les remplit de la fin (\0) vers le début sinon faudrait les inverser à la fin vu qu'on effectue le parcours depuis la fin de la séquence choisie) en choisissant le meilleur chemin calculé précédemment dans prevs*/
  // mat->cells[i].prevs
  char *s1_align = NULL;
  char *s2_align = NULL;
  s1_align = malloc(strlen(s1));
  s2_align = malloc(strlen(s2));
  cell=mat->cells[mat->w*i_BS+j_BS];
  prev=cell.prevs;
  int *s1_align_int = NULL;
  int *s2_align_int = NULL;
  s1_align_int = malloc(strlen(s1)*sizeof(int));
  s2_align_int = malloc(strlen(s2)*sizeof(int));
  
  //initialisation des chaines. 0 signifie que le caractère n'es pas considéré dans la chaine : on n'affichera rien. 
  // 1 signifie qu'il fait partie de la chaine : on affichera le caractère
  // -1 signifie qu'il a été supprimé lors d'une indel : on affichera -
  for(int p = 0; p < strlen(s1); p++){
  	s1_align_int[p] = 0;
  }
  
  for(int p = 0; p < strlen(s2); p++){
  	s2_align_int[p] = 0;
  }
  
  while(cell.score!=0) { // pas du tout sûr!!
    //i_BS=mat-
    if (prev&1) {
      i_BS-=1; 
      j_BS-=1;
      s1_align_int[i_BS] = 1;
      s2_align_int[j_BS] = 1;
      printf("diag\n");
    }
    else if (prev&2) { // là on doit remplir la chaine
      j_BS-=1;
      s2_align_int[j_BS] = -1;
      printf("top\n");
    }
      else if (prev&4){
		i_BS-=1; //cmt on sait ?
		s1_align_int[i_BS] = -1;
		printf("left\n");
    }
    cell=mat->cells[mat->w*i_BS+j_BS];
    prev=cell.prevs;
  }


  int s1_start=i_BS; // à modifier quand le code sera bon
  int s2_start=j_BS;

  /* affichage des résulatats */
  printf("Best score is %.1f, the best-scoring alignments are:\n\n ",BestScore);
  printf("s1 alignment starts at coord %d\n s2 alignment starts at coord %d\n\n",s1_start,s2_start);
  printf("les sequences alignees sont : \n");
  for(int p = 0; p < strlen(s1); p++){
  	if(s1_align_int[p] == 1){
  	  printf("%c", s1[p]);
  	}
  	if(s1_align_int[p] == -1){
  	  printf("-");
  	}
  }
  printf("\n");
  for(int p = 0; p < strlen(s2); p++){
  	if(s2_align_int[p] == 1){
  	  printf("%c", s2[p]);
  	}
  	if(s2_align_int[p] == -1){
  	  printf("-");
  	}
  }
  printf("\n");
  printf("les sequences initiales sont : \n");
  printf("s1	%s\n",s1);
  printf("s2	%s\n",s2);
}
