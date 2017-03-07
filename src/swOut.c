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
  for (unsigned int i=0; i<mat->w; i++){
    for (unsigned int j=0; j<mat->h; j++) {
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

  while(cell.score!=0) { // pas du tout sûr!!
    //i_BS=mat-
    if (cell.prevs==1) {
      i_BS-=1; 
      j_BS-=1;
    }
    else if (cell.prevs==-1) { // là on doit remplir la chaine
      i_BS-=1; 
      j_BS-=1;
    }
      else if (cell.prevs==-2){
	i_BS-=1; //cmt on sait ?
    }
    cell=mat->cells[mat->w*i_BS+j_BS];
    prev=cell.prevs;
  }


  int s1_start=1; // à modifier quand le code sera bon
  int s2_start=2;

  /* affichage des résulatats */
  printf("Best score is %.1f, the best-scoring alignments are:\n\n ",BestScore);
  printf("s1 alignment starts at coord %d\n s2 alignment starts at coord %d\n\n",s1_start,s2_start);
  printf("s1	%s\n",s1);
  printf("s2	%s\n",s2);


}
