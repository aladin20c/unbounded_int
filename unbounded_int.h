#ifndef unbounded_int_h
#define unbounded_int_h
/*Les entiers seront représentés par des listes doublements chaînées
Les champs precedent et suivant pointent respectivement vers le bloc précédent et suivant.*/

typedef struct chiffre{
  struct chiffre  *suivant;
  char c;
  struct chiffre *precedent;
} chiffre;



/*
On utilisera la structure les pointeurs vers le premier et le dernier bloc de la liste de chiffre,
le signe stocké comme un caractère, soit ’+’ soit ’-’,
len - le nombre de chiffres sur la liste.
*/

typedef struct{
  char signe; /* soit ’+’ soit ’-’ */
  size_t len; /* longueur de la liste */
  chiffre *premier; /* pointeur vers le premier élément de la liste*/
  chiffre *dernier; /*pointeur vers le dernier élément de la liste*/
} unbounded_int;





/*qui prend en argument une chaîne de caractères et retourne la structure unbounded_int*/
unbounded_int string2unbounded_int(const char *e);



/*qui prend en argument un nombre long long et retourne la structure unbounded_int*/
unbounded_int ll2unbounded_int(long long i);



/*prend en argument une structure unbounded_int représentant un entier et retourne cet entier sous forme d’une chaîne de caractères.*/
char *unbounded_int2string(unbounded_int i);



/*qui re- tourne une des valeurs −1, 0, 1*/
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);



/*qui re- tourne une des valeurs −1, 0, 1*/
int unbounded_int_cmp_ll(unbounded_int a, long long b);



/*renvoyant la représentation de la somme de deux entiers représentés par a et b,*/
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);



/* ren- voyant la représentation de leur différence,*/
unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b);



/*renvoyant la représentation de leur produit,*/
unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b);



#endif
