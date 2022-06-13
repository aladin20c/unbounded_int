1 Modalités Langage C - projet


Le projet doit être réalisé en binôme. Les soutenances auront lieu au mois de mai (après le 9 mai), la date exacte sera communiquée ultérieurement. Pendant la soutenance, les membres de binôme devront chacun montrer leur maîtrise de la totalité du code.

2 Présentation de projet

Le but du projet est d’écrire une bibliothèque de fonctions qui implémentent des opérations sur les entiers avec un nombre arbitraire de chiffres.

3 Type de données

Les entiers seront représentés par des listes doublements chaînées. La structure suivante définit un élément de la liste :
1 2 3 4 5
1 2 3 4 5 6

  typedef struct chiffre{
    struct chiffre  *suivant;
    char c;
    struct chiffre *precedent;
  } chiffre;

La structure sert à stocker un chiffre représenté par le caractère correspondant : ’0’, ’1 ’, ’2’, ’3’, ’4’, ’5’, ’6’, ’7’, ’8’, ’9’.
Les champs precedent et suivant pointent respectivement vers le bloc précédent et suivant.
On utilisera la structure

  typedef struct{
    char signe; /* soit ’+’ soit ’-’ */ size_t len; /* longueur de la liste */
    chiffre *premier; /* pointeur vers le premier élément de la liste*/
    chiffre *dernier; /*pointeur vers le dernier élément de la liste*/
  } unbounded_int;

pour stocker
– les pointeurs vers le premier et le dernier bloc de la liste de chiffre,
– le signe stocké comme un caractère, soit ’+’ soit ’-’,
– len le nombre de chiffres sur la liste.



4 Travail demandé


Le travail demandé est divisé en deux parties décrites ci-dessous. Dans la première, vous aurez à écrire et tester des fonctions permettant de construire des objets unbounded_int, ou implémentant des opérations arithmétiques sur les entiers unbounded_int.

Dans la seconde partie, vous aurez à écrire un mini-interpréteur permettant de lire un fichier dont le nom sera un paramètre de main. Ce fichier contiendra des instructions décrivant des opérations à effectuer sur des objets unbounded_int. Les résultats de ces opérations seront écrits dans un autre fichier texte dont le nom sera un autre paramètre de main.

4.1 Fonctions sur les unbounded_int

Certains de fonctions suivantes retournent la structure unbounded_int. Si les appels à malloc() échouent pour indiquer le problème la fonction doit retourner un unbounded_int avec le champ signe == ’*’.


(1) unbounded_int string2unbounded_int(const char *e)

(2) unbounded_int ll2unbounded_int(long long i)

(3) char *unbounded_int2string(unbounded_int i)

(4) int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b)

(5) int unbounded_int_cmp_ll(unbounded_int a, long long b)

(6) unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b)

(7) unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b)

(8) unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b)



4.2 Interpréteur


La deuxième partie du projet consiste à écrire un mini-interpréteur calc_unbounded_int. Ce programme sera exécuté en ligne de commande de façon suivante :

              ./calc_unbounded_int -i source.txt -o dest.txt

L’option -i du programme donne le nom de fichier texte qui contient une suite d’ins- tructions à exécuter par l’interprète. Dans l’option -o on passe le nom de fichier où calc_unbounded_int écrira les résultats.
On suppose que les deux options sont facultatives : si l’option -i est absente calc_unbounded_int
lira les instructions depuis le flot d’entrée standard stdin et si l’option -o est absente le programme écrira les résultats dans le flot stdout.
Le fichier lu par l’interpréteur contient trois types d’instructions, réparties sur des lignes distinctes :

(A) variable = entier_ou_variable op entier_ou_variable

(B) variable = entier

(C) print variable

On suppose que
– chaque ligne contient au plus une instruction,
– il est possible d’avoir des lignes vides (sans instruction),
– il n’y a pas de parenthèses,
– il y a des espaces (au moins un de chaque côté) autour de chaque opération * , +, -,
– les entiers positif peuvent être écrits soit comme 23432 soit comme +98676565 avec + au début suivi immédiatement d’un chiffre,
– les nombres négatifs s’écrivent comme -657867654 avec le signe - suivi immédiatement d’un chiffre,
– des espaces à gauche ou à droite de = sont autorisés mais pas obligatoires,
– on peut avoir zéro ou plusieurs espaces au début de chaque ligne.





les fichiers sont compilés par la commande : make
