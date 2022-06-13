#ifndef unbounded_int_c
#define unbounded_int_c
/*
   fichier contenant les definitions des fonctions données dans polynomes.h
   gcc -Wall unbounded_int.c -lm && ./a.out
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "unbounded_int.h"



static unbounded_int create_empty_unbounded_int(){
  return (unbounded_int){.signe='*', .len=0, .premier=NULL, .dernier=NULL};
}

static chiffre *create_chiffre(char c){
  chiffre *ch=malloc(sizeof(chiffre));
  if(ch==NULL){
    fprintf(stderr, "error in function create_chiffre(char,chiffre*,chiffre*)\n");
    return NULL;
  }
  ch->c=c;
  ch->suivant=NULL;
  ch->precedent=NULL;
  return ch;
}

static void free_unbounded_int(unbounded_int ui){
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

static void afficher_unbounded_int(unbounded_int ui){
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

static int matches_unbounded_int(const char* e){
  if(strlen(e)==0) {return 0;}
  if(e[0]!='+' && e[0]!='-' &&( e[0]<'0' || e[0]>'9')) {return 0;}
  for (int i = 1; i < strlen(e); i++) {
    if(e[i]<'0' || e[i]>'9'){
      return 0;
    }
  }
  return 1;
}

static unbounded_int insertLast(unbounded_int ui,char c){
  chiffre *ch=create_chiffre(c);
  if(ch==NULL){
    fprintf(stderr, "error in function insertLast(unbounded_int,char)\n");
    return create_empty_unbounded_int();
  }
  if(ui.dernier==NULL){
    ui.dernier=ch;
    ui.premier=ch;
    ch->suivant=NULL;
    ch->precedent=NULL;
  }else if(ui.premier==ui.dernier){
    ch->suivant=NULL;
    ch->precedent=ui.premier;
    (ui.premier)->precedent=NULL;
    (ui.premier)->suivant=ch;
    ui.dernier=ch;
  }else{
    ch->suivant=NULL;
    ch->precedent=ui.dernier;
    (ui.dernier)->suivant=ch;
    ui.dernier=ch;
  }
  ui.len+=1;
  return ui;
}

static unbounded_int insertFirst(unbounded_int ui,char c){
  chiffre *ch=create_chiffre(c);
  if(ch==NULL){
    fprintf(stderr, "error in function insertLast(unbounded_int,char)\n");
    return create_empty_unbounded_int();
  }
  if(ui.premier==NULL){
    ui.premier=ch;
    ui.dernier=ch;
    ch->suivant=NULL;
    ch->precedent=NULL;
  }else if(ui.premier==ui.dernier){
    ch->suivant=ui.dernier;
    ch->precedent=NULL;
    (ui.dernier)->precedent=ch;
    (ui.dernier)->suivant=NULL;
    ui.premier=ch;
  }else{
    ch->suivant=ui.premier;
    ch->precedent=NULL;
    (ui.premier)->precedent=ch;
    ui.premier=ch;
  }
  ui.len+=1;
  return ui;
}

//retourne un string representant un unbounded int inversé (absolu)
static char *unbounded_int_to_inv_str(unbounded_int ui){
  if(ui.signe=='*') return NULL;
  char * str= malloc(ui.len*sizeof(char)+2);
  if(str==NULL){
    return NULL;
  }
  int indice=0;
  chiffre *tmp=ui.dernier;
  if(ui.premier==ui.dernier){
    str[indice]=tmp->c;
    return str;
  }
  while (tmp!=NULL) {
    str[indice]=tmp->c;
    tmp=tmp->precedent;
    indice++;
  }
  return str;
}

//retourne un string representant un unbounded int representant un string inversé (absolu)
static unbounded_int str_to_inv_unbounded_int( const char *e){

  unbounded_int ui = create_empty_unbounded_int();
  if(! matches_unbounded_int(e)) return ui;
  //fixing signe
  int indice=0;
  if(e[0]=='+' || e[0]=='-'){
    ui.signe=e[0];
    indice++;
  }else{
    ui.signe='+';
  }
  //initialising chiiffres
  for (; indice < strlen(e); indice++) {
    ui=insertFirst(ui,e[indice]);
    if(ui.signe=='*') return ui;
  }

  return ui;
}

unbounded_int string2unbounded_int(const char *e){

  unbounded_int ui = create_empty_unbounded_int();
  if(! matches_unbounded_int(e)) return ui;
  //fixing signe
  int indice=0;
  if(e[0]=='+' || e[0]=='-'){
    ui.signe=e[0];
    indice++;
  }else{
    ui.signe='+';
  }
  //initialising chiiffres
  for (; indice < strlen(e); indice++) {
    ui=insertLast(ui,e[indice]);
    if(ui.signe=='*') return ui;
  }
  return ui;
}

unbounded_int ll2unbounded_int(long long i){
  unbounded_int ui = create_empty_unbounded_int();
  if(i<0){
    ui.signe='-';
    i*=-1;
  }else{
    ui.signe='+';
  }
  if(i==0){
    ui=insertFirst(ui,'0');
    return ui;
  }
  while (i) {
    char c=(i % 10)+'0';
    ui=insertFirst(ui,c);
    if(ui.signe=='*') {return ui;}
    i/=10;
  }
  return ui;
}

char *unbounded_int2string(unbounded_int ui){
  if(ui.signe=='*') return NULL;
  char * str= malloc(ui.len*sizeof(char)+2);
  if(str==NULL){
    return NULL;
  }
  if(ui.signe=='-'){
    str[0]='-';
  }else{
    str[0]='+';
  }
  int indice=1;
  chiffre *tmp=ui.premier;
  if(ui.premier==ui.dernier){
    str[indice]=tmp->c;
    return str;
  }
  while (tmp!=NULL) {
    str[indice]=tmp->c;
    tmp=tmp->suivant;
    indice++;
  }
  return str;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b){
  if(a.signe=='*'){
    if(b.signe=='*') return 0;
    else return -1;
  }
  if(b.signe=='*'){
    if(a.signe=='*') return 0;
    else return 1;
  }

  int coeff=1;
  if(a.signe!=b.signe){
      if(a.signe=='+') return 1;
      else return -1;
  }else{
      if(a.signe=='-') coeff=-1;
  }

  if(a.len>b.len){
    return 1*coeff;
  }else if(a.len<b.len){
    return -1*coeff;
  }else{
    chiffre* tmp1=a.premier;
    chiffre* tmp2=b.premier;
    for (int i = 0; i < a.len; i++) {
        if((tmp1->c)>(tmp2->c)){
            return 1*coeff;
        }else if((tmp1->c)<(tmp2->c)){
            return -1*coeff;
        }else{
            tmp1=tmp1->suivant;
            tmp2=tmp2->suivant;
        }
    }
  }
  return 0;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b){
  if(a.signe=='*'){
    return -1;
  }
  unbounded_int a2= ll2unbounded_int(b);
  return unbounded_int_cmp_unbounded_int(a,a2);
}



//--------op---------//



static unbounded_int unbounded_int_somme_aux(const unbounded_int a, const unbounded_int b, char signe){

    unbounded_int resultat=create_empty_unbounded_int();
    //Initialise le signe
    resultat.signe=signe;

    chiffre *tmp_1=a.dernier;
    chiffre *tmp_2=b.dernier;
    int rest=0;

    while(tmp_1!=NULL && tmp_2!=NULL){
        int somme= tmp_1->c-'0' + tmp_2->c-'0'+rest;
        rest=somme/10;
        char new= somme % 10 +'0';

        resultat=insertFirst(resultat,new);
        if(resultat.signe=='*') return resultat;

        tmp_1=tmp_1->precedent;
        tmp_2=tmp_2->precedent;
    }

    while(tmp_1!=NULL){
        int somme= tmp_1->c-'0'+rest;
        rest=somme/10;
        char new= somme % 10 +'0';

        resultat=insertFirst(resultat,new);
        if(resultat.signe=='*') return resultat;

        tmp_1=tmp_1->precedent;
    }

    while(tmp_2!=NULL){
        int somme= tmp_2->c-'0'+rest;
        rest=somme/10;
        char new= somme % 10 +'0';

        resultat=insertFirst(resultat,new);
        if(resultat.signe=='*') return resultat;

        tmp_2=tmp_2->precedent;
    }

    if(rest!=0){
      resultat=insertFirst(resultat,rest+'0');
    }


    return resultat;
}


static unbounded_int unbounded_int_difference_aux(unbounded_int a, unbounded_int b, char signe){

  unbounded_int resultat=create_empty_unbounded_int();
  //Initialise le signe
  resultat.signe=signe;

  chiffre *tmp_1=a.dernier;
  chiffre *tmp_2=b.dernier;
  int rest=0;

  while(tmp_1!=NULL && tmp_2!=NULL){
      int sous= tmp_1->c-'0' - (tmp_2->c-'0'+rest);
      if(sous<0){
        rest=1;
        sous+=10;
      }else{
        rest=0;
      }

      char new= sous +'0';
      resultat=insertFirst(resultat,new);
      if(resultat.signe=='*') return resultat;

      tmp_1=tmp_1->precedent;
      tmp_2=tmp_2->precedent;
  }

  while(tmp_1!=NULL){
      int sous= (tmp_1->c-'0')-rest;
      if(sous<0){
        rest=1;
        sous+=10;
      }else{
        rest=0;
      }

      char new= sous +'0';

      resultat=insertFirst(resultat,new);
      if(resultat.signe=='*') return resultat;

      tmp_1=tmp_1->precedent;
  }

  return resultat;
}


unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b){
    //utilise les fonction auxilières add et sous selon les signes

    if(a.signe=='+' && b.signe=='+'){//a + b

        return unbounded_int_somme_aux(a,b,'+');

    }else if(a.signe=='-' && b.signe=='-'){//−(|a| + |b|)

        return unbounded_int_somme_aux(a,b,'-');

    }else if(a.signe=='+' && b.signe=='-'){//a − |b|

        b.signe='+';
        if(unbounded_int_cmp_unbounded_int(a, b)>=0){//|a|>|b|

          b.signe='-';
          return unbounded_int_difference_aux(a,b,'+');

        }else{

          b.signe='-';
          return unbounded_int_difference_aux(b,a,'-');//|a|<|b|

        }

    }else{//b − |a|
        //|a|>|b|
        a.signe='+';
        if(unbounded_int_cmp_unbounded_int(b, a)>=0){
          a.signe='-';
          return unbounded_int_difference_aux(b,a,'+');
        //|a|<|b|
        }else{
          a.signe='-';
          return unbounded_int_difference_aux(a,b,'-');
        }

    }
}


unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b){
    //utilise les fonction auxilières add et sous selon les signes
    //a − b
    if(a.signe=='+' && b.signe=='+'){

        if(unbounded_int_cmp_unbounded_int(a,b)>=0){//|a|>|b|

            return unbounded_int_difference_aux(a,b,'+');

        }else{//|a|<|b|

            return unbounded_int_difference_aux(b,a,'-');

        }

    }else if(a.signe=='-' && b.signe=='-'){//|b| − |a|

        if(unbounded_int_cmp_unbounded_int(a,b)>=0){//|a|>|b|

            return unbounded_int_difference_aux(b,a,'+');

        }else{//|a|<|b|

            return unbounded_int_difference_aux(a,b,'-');

        }

    }else if(a.signe=='+' && b.signe=='-'){//a + |b|

        return unbounded_int_somme_aux(a,b,'+');

    }else{//−(b + |a|)

        return unbounded_int_somme_aux(a,b,'-');

    }
}




static char* string_produit(char *a, char *b){
  //
  int a_length=strlen(a);
  int b_length=strlen(b);
  //Initialisation des chiffres du resultat à '0'
  char *c=malloc(a_length+2);//+1(taille max du calcul)+1(caractere final)
  for(int i=0; i<a_length+1; i++){
      c[i]='0';
  }
  c[a_length+1]='\0';

  int i=0;
  int j=0;
  int r=0;
  for( j = 0; i < b_length; i++){
    r = 0;
    if( b[j] == '0' )
      continue;
    for( i=0; i < a_length; i++ ){
      int v = (c[i+j] -'0') + (a[i]-'0')*(b[j]-'0') + r;
      c[i+j] = (char)((v % 10)+'0');
      r = v / 10;
    }
  c[j+a_length] = (char)(r+'0');
  }

  return c;//La chaine de caractere retounee
}







unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b){
  //variable
  unbounded_int res=string2unbounded_int("0");

  char* a_to_inv_str=unbounded_int_to_inv_str(a);
  char* b_to_inv_str=unbounded_int_to_inv_str(b);

  //multiplication de a par chaque chiffre de b
  for(int i=0; i<strlen(b_to_inv_str); i++){
    char *tmp=string_produit(a_to_inv_str,b_to_inv_str+i);
    //décalage en fonction de la position du chiffre de b(unité, dizaine...)
    char *decalage=malloc((sizeof(char)*i)+1);
    for(int j=0; j<i; j++){
      decalage[j]='0';
    }
    decalage[i+1]='\0';

    //concaténation du décalage et de tmp
    char *res_tmp=malloc(strlen(tmp)+strlen(decalage)+1);
    res_tmp[0]='\0';
    strcpy(res_tmp,decalage);
    strcat(res_tmp,tmp);
    //somme de res et du res_tmp
    unbounded_int res_tmp_inv =str_to_inv_unbounded_int(res_tmp);
    res=unbounded_int_somme(res_tmp_inv,res);

    //liberation de la memoire
    free(res_tmp);
    free(decalage);
  }

  //initialisation corecte du signe de res
  if(a.signe!=b.signe){
    res.signe='-';
  }

  return res;
}

/*---------------------------------------------MAIN---------------------------------------------*/


#endif
