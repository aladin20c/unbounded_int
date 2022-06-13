/*
   fichier contenant les tests de unbounded-int.c
*/

// gcc -Wall test_unbounded.c && ./a.out

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include "unbounded_int.c"

static void free_unbounded_int__(unbounded_int ui){
    chiffre *tmp=ui.premier;
    if(ui.premier==NULL){
      return;
    }
    if(ui.premier==ui.dernier){
        free(ui.premier);
        return;
    }
    while (tmp!=NULL) {
      chiffre *tmp2=tmp;
      tmp=tmp->suivant;
      free(tmp2);
    }
}

static void afficher_unbounded_int__(unbounded_int ui){
  if(ui.premier==NULL){
    printf("\n");return;
  }
  printf("%c",ui.signe);
  if(ui.premier==ui.dernier){
    printf("%c\n",(ui.premier)->c);
    return;
  }
  chiffre *tmp=ui.premier;
  while (tmp!=NULL) {
    printf("%c",tmp->c);
    tmp=tmp->suivant;
  }
  printf("\n");
}


int main(int argc, char const *argv[]) {

  printf("%s\n","__________________________test__________________________");

  printf("%s\n","string2unbounded_int(\"999\");");
  unbounded_int ex1=string2unbounded_int("999");
  printf("unbounded_int : ex1 :");
  afficher_unbounded_int__(ex1);


  printf("%s\n","ll2unbounded_int((long long)+93807)");
  unbounded_int ex2=ll2unbounded_int((long long)+93807);
  printf("unbounded_int : ex2 :");
  afficher_unbounded_int__(ex2);

  printf("\ncomparaison\n");
  printf("%s\n","compare ex1 to -938238");
  printf("%d\n", unbounded_int_cmp_ll(ex1,-93823876688));
  printf("%s\n","compare ex1 to ex2");
  printf("%d\n", unbounded_int_cmp_unbounded_int(ex1,ex2));
  printf("%s\n","compare ex1 to 888");
  printf("%d\n", unbounded_int_cmp_ll(ex1,888));


  printf("\n ex1 to string\n");
  printf("%s\n",unbounded_int2string(ex1));



  printf("Test de unbounded_int_somme :\n");
  unbounded_int ex3=ll2unbounded_int((long long)250);
  unbounded_int ex4=ll2unbounded_int((long long)750);
  printf("ex3 : ");afficher_unbounded_int__(ex3);
  printf("ex4 : ");afficher_unbounded_int__(ex4);
  unbounded_int ex5=unbounded_int_somme(ex3,ex4);
  printf("ex3 + ex4 : ");
  afficher_unbounded_int__(ex5);
  //free


  printf("unbounded_int_difference :\n");
  printf("ex1 - ex2 : ");
  unbounded_int ex6=unbounded_int_difference(ex1,ex2);
  afficher_unbounded_int__(ex6);


  printf("unbounded_int_produit :\n");
  unbounded_int ex7=ll2unbounded_int((long long)+25678909);
  unbounded_int ex8=ll2unbounded_int((long long)+556);
  printf("ex7 : ");afficher_unbounded_int__(ex7);
  printf("ex8 : ");afficher_unbounded_int__(ex8);
  unbounded_int ex9=unbounded_int_produit(ex7,ex8);
  printf("ex7 * ex8 : ");afficher_unbounded_int__(ex9);


  free_unbounded_int__(ex1);
  free_unbounded_int__(ex2);
  free_unbounded_int__(ex3);
  free_unbounded_int__(ex4);
  free_unbounded_int__(ex5);
  free_unbounded_int__(ex6);
  free_unbounded_int__(ex7);
  free_unbounded_int__(ex8);
  free_unbounded_int__(ex9);
  return 0;
}
