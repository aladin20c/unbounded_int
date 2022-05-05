/*
   fichier contenant les definitions des fonctions données dans polynomes.h
   gcc unbounded_int.c -lm && ./a.out
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#include "unbounded_int.h"


static unbounded_int create_empty_unbounded_int();

static chiffre *create_chiffre(char c);

static void free_unbounded_int(unbounded_int ui);

static void afficher_unbounded_int(unbounded_int ui);

static int matchesnumber(const char* str);

static unbounded_int insertLast(unbounded_int ui,char c);

static unbounded_int insertFirst(unbounded_int ui,char c);

static unbounded_int unbounded_int_absolute_value(unbounded_int a);

static chiffre* chiffre_somme(chiffre a, chiffre b, chiffre r);

static chiffre* chiffre_difference(chiffre a, chiffre b, chiffre r);

static unbounded_int unbounded_int_somme_aux(unbounded_int a, unbounded_int b, char signe);

static unbounded_int unbounded_int_difference_aux(unbounded_int a, unbounded_int b, char signe);



/*---------------------------------------------MAIN---------------------------------------------*/



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
    ch->suivant=ch;
    ch->precedent=ch;
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
    ch->suivant=ch;
    ch->precedent=ch;
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


static unbounded_int unbounded_int_absolute_value(unbounded_int a){
    unbounded_int abs_value=create_empty_unbounded_int();
    //Initialise le signe à + (positif)
    abs_value.signe='+';

    if(a.premier==NULL){
        fprintf(stderr, "error in function unbounded_int_absolute_value(unbounded_int)\n");
        return create_empty_unbounded_int();
    }else{
        chiffre *tmp=a.premier;

        do{
            abs_value=insertLast(abs_value, tmp->c);
            tmp=tmp->suivant;
        }while(tmp!=NULL);
    }

    return abs_value;
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

static chiffre* chiffre_somme(chiffre a, chiffre b, chiffre r){ 
    chiffre *chiffre_zero=malloc(sizeof(chiffre));
    chiffre_zero->c='0';
    //somme des entiers courants et de la retenue le tout modulo 10
    chiffre *tmp=malloc(sizeof(chiffre));
    tmp->c=(char) ((a.c-'0' + b.c-'0' + r.c-'0')%10 +'0');
    //Calcul de la retenu pour le prochain calcul
    chiffre *r_tmp=malloc(sizeof(chiffre));
    r_tmp->c=(char) ((a.c-'0' + b.c-'0' + r.c-'0')/10 +'0');

    //si a.precedent et b.precedent sont null
    if(a.precedent==NULL && b.precedent==NULL){
        //Si retenu!=0
        if(r_tmp->c!='0'){
            //calcul de chiffre+1 avec la retenu
            tmp->precedent=chiffre_somme(*chiffre_zero, *chiffre_zero, *r_tmp);     
            //initialise le suivant de tmp.precedent
            tmp->precedent->suivant=tmp;
        }
    
    //si a.precedent est null 
    }else if(a.precedent==NULL){
        //retourn somme de 0 et b.precedent
        tmp->precedent=chiffre_somme(*chiffre_zero, *b.suivant, *r_tmp);    
        //initialise le suivant de tmp.precedent
        tmp->precedent->suivant=tmp;

    //si b.precedent est null 
    }else if(b.precedent==NULL){
        //retourn somme de 0 et a.precedent
        tmp->precedent=chiffre_somme(*a.precedent, *chiffre_zero, *r_tmp);
        //initialise le suivant de tmp.precedent
        tmp->precedent->suivant=tmp;

    //sinon (a.precedent et b.precedent ne sont pas null)
    }else{
        //somme de a.precedent et b.precedent
        tmp->precedent=malloc(sizeof(chiffre));
        tmp->precedent=chiffre_somme(*a.precedent, *b.precedent,*r_tmp);
        //initialise le suivant de tmp.precedent
        tmp->precedent->suivant=tmp;
    }
    //libère la retenu du prochain calcul et le chiffre zero
    free(r_tmp);
    free(chiffre_zero);
    //retourne le chiffre courant
    return tmp;
}


static chiffre* chiffre_difference(chiffre a, chiffre b, chiffre r){ 
    //Vérifier que a>b>0


    chiffre *chiffre_zero=malloc(sizeof(chiffre));
    chiffre_zero->c='0';
    chiffre *chiffre_dix=malloc(sizeof(chiffre));
    chiffre_dix->c=(('9'-'0')+('1'-'0'));
    //Différence des entiers courants plus la retenu
    chiffre *tmp=malloc(sizeof(chiffre));
    tmp->c=(char) (((a.c-'0') - (b.c-'0') - (r.c-'0')) +'0');
    //Calcul de la retenu pour le prochain calcul
    chiffre *r_tmp=malloc(sizeof(chiffre));
    r_tmp->c=(char) (0 +'0');
    //Si le resultat du calcul courant<0
    if(tmp->c<'0'){
        tmp->c+=(('9'-'0')+('1'-'0'));
        r_tmp->c=(char) (1 +'0');
    }
    //si a.precedent et b.precedent sont null
    if(a.precedent!=NULL && b.precedent!=NULL){
        //Différence de a.precedent et b.precedent
        tmp->precedent=malloc(sizeof(chiffre));
        tmp->precedent=chiffre_difference(*a.precedent, *b.precedent,*r_tmp);
        //initialise le suivant de tmp.precedent
        tmp->precedent->suivant=tmp;
    //si b.precedent est null
    }else if(a.precedent!=NULL){
        //Différence de a.precedent et b.precedent
        tmp->precedent=malloc(sizeof(chiffre));
        tmp->precedent=chiffre_difference(*a.precedent, *chiffre_zero,*r_tmp);
        //initialise le suivant de tmp.precedent
        tmp->precedent->suivant=tmp;
    }

    //libère la retenu du prochain calcul, le chiffre zero et le chiffre dix
    free(r_tmp);
    free(chiffre_zero);
    free(chiffre_dix);
    //retourne le chiffre courant
    return tmp;
}


static unbounded_int unbounded_int_somme_aux(unbounded_int a, unbounded_int b, char signe){
    unbounded_int resultat=create_empty_unbounded_int();
    //Initialise le dernier chiffre 
    resultat.dernier=chiffre_somme(*a.dernier,*b.dernier,*(create_chiffre('0')));
    //Initialise len et le premier chiffre
    chiffre tmp=*(resultat.dernier);
    resultat.len++;
    while(tmp.precedent!=NULL){
        resultat.len++;
        tmp=*(tmp.precedent);
    }
    resultat.premier=&(tmp);
    //Initialise le signe 
    resultat.signe=signe;

    return resultat;
}


static unbounded_int unbounded_int_difference_aux(unbounded_int a, unbounded_int b, char signe){
    unbounded_int resultat=create_empty_unbounded_int();
    //Initialise le dernier chiffre 
    resultat.dernier=chiffre_difference(*a.dernier,*b.dernier,*create_chiffre('0'));
    //Initialise len et le premier chiffre
    chiffre tmp=*(resultat.dernier);
    resultat.len++;
    while(tmp.precedent!=NULL){
        resultat.len++;
        tmp=*(tmp.precedent);
    }
    resultat.premier=&(tmp);
    //Initialise le signe 
    resultat.signe=signe;
    
    return resultat;
}

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b){
    //utilise les fonction auxilières add et sous selon les signes
    //a + b
    if(a.signe=='+' && b.signe=='+'){
        return unbounded_int_somme_aux(a,b,'+');
    //−(|a| + |b|)
    }else if(a.signe=='-' && b.signe=='-'){
        return unbounded_int_somme_aux(a,b,'-');

    //a − |b| 
    }else if(a.signe=='+' && b.signe=='-'){
        //|a|>|b|

        if(unbounded_int_cmp_unbounded_int(a, unbounded_int_absolute_value(b))>=0){
            return unbounded_int_difference_aux(a,b,'+');
        //|a|<|b| 
        }else{
            return unbounded_int_difference_aux(b,a,'-');
        }

    //b − |a| 
    }else{
        //|a|>|b|
        if(unbounded_int_cmp_unbounded_int(a, unbounded_int_absolute_value(b))>=0){
            return unbounded_int_difference_aux(b,a,'+');
        //|a|<|b| 
        }else{
            return unbounded_int_difference_aux(a,b,'-');            
        }

    }
}

unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b){
    //utilise les fonction auxilières add et sous selon les signes
    //a − b
    if(a.signe=='+' && b.signe=='+'){
        //|a|>|b|
        if(unbounded_int_cmp_unbounded_int(a,b)>=0){
            return unbounded_int_difference_aux(a,b,'+');
        //|a|<|b|
        }else{
            return unbounded_int_difference_aux(b,a,'-');
        } 
    //|b| − |a|
    }else if(a.signe=='-' && b.signe=='-'){
        //|a|>|b|
        if(unbounded_int_cmp_unbounded_int(unbounded_int_absolute_value(a),unbounded_int_absolute_value(b))>=0){
            return unbounded_int_difference_aux(a,b,'-');
        //|a|<|b| 
        }else{
            return unbounded_int_difference_aux(b,a,'+');
        }
    //a + |b|
    }else if(a.signe=='+' && b.signe=='-'){
        return unbounded_int_somme_aux(a,b,'+');
    //−(b + |a|)
    }else{
        return unbounded_int_somme_aux(a,b,'-');
    }
}


int main(int argc, char const *argv[]) {
  unbounded_int ex1=string2unbounded_int("-93823876688");
  unbounded_int ex2=ll2unbounded_int((long long)+93807);
  afficher_unbounded_int(ex1);
  afficher_unbounded_int(ex2);
  printf("%d\n", unbounded_int_cmp_unbounded_int(ex1,ex2));
  printf("%d\n", unbounded_int_cmp_ll(ex1,+9382388)); 

  printf("Test de unbounded_int_somme :\n");
  unbounded_int ex3=create_empty_unbounded_int();
  ex3=insertLast(ex3,'1');
  ex3=insertLast(ex3,'0');
  ex3.signe='+';
  unbounded_int ex4=create_empty_unbounded_int();
  ex4=insertLast(ex4,'1');
  ex4=insertLast(ex4,'0');
  ex4.signe='+';
  afficher_unbounded_int(ex3);
  afficher_unbounded_int(ex4);
  unbounded_int res=unbounded_int_somme(ex3,ex4);
  printf("Resultat :");
  afficher_unbounded_int(res);

  //free
  free_unbounded_int(ex1);
  free_unbounded_int(ex2);
  free_unbounded_int(ex3);
  free_unbounded_int(ex4);
}
