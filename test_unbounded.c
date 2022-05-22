/*
   fichier contenant les tests de unbounded-int.c
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


#include "unbounded_int.h"


void test_unbounded_int2string(){
    int number;
    printf("Test de la fonction unbounded_int2string :\n");
    srand(time(NULL));
    number=(rand()%9)+1;
    unbounded_int test_1=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_1=insertLast(test_1, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_1.signe='+';
    }else{
        test_1.signe='-';
    }
    
    
    printf("Premier entier : ");
    afficher_unbounded_int(test_1);
    char *test =unbounded_int2string(test_1);
    printf("Resultat : %s\n", test);
    
}

void test_unbounded_int_cmp_unbounded_int(){
    int number;
    printf("Test de la fonction unbounded_int_cmp_unbounded_int :\n");
    srand(time(NULL));
    number=(rand()%9)+1;
    unbounded_int test_1=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_1=insertLast(test_1, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_1.signe='+';
    }else{
        test_1.signe='-';
    }
    
    number=rand()%10;
    unbounded_int test_2=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_2=insertLast(test_2, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_2.signe='+';
    }else{
        test_2.signe='-';
    }
    
    
    printf("Premier entier : ");
    afficher_unbounded_int(test_1);
    printf("Second entier : ");
    afficher_unbounded_int(test_2);
    int test =unbounded_int_cmp_unbounded_int(test_1,test_2);
    printf("Resultat : %d\n",test);
    afficher_unbounded_int(test);
}

void test_unbounded_int_somme(){
    int number;
    printf("Test de la fonction unbounded_int_somme :\n");
    srand(time(NULL));
    number=(rand()%9)+1;
    unbounded_int test_1=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_1=insertLast(test_1, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_1.signe='+';
    }else{
        test_1.signe='-';
    }
    
    number=rand()%10;
    unbounded_int test_2=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_2=insertLast(test_2, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_2.signe='+';
    }else{
        test_2.signe='-';
    }
    
    
    printf("Premier entier : ");
    afficher_unbounded_int(test_1);
    printf("Second entier : ");
    afficher_unbounded_int(test_2);
    unbounded_int test =unbounded_int_somme(test_1,test_2);
    printf("Resultat : ");
    afficher_unbounded_int(test);
}

void test_unbounded_int_difference(){
    int number;
    printf("Test de la fonction unbounded_int_difference :\n");
    srand(time(NULL));
    number=(rand()%9)+1;
    unbounded_int test_1=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_1=insertLast(test_1, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_1.signe='+';
    }else{
        test_1.signe='-';
    }
    
    number=rand()%10;
    unbounded_int test_2=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_2=insertLast(test_2, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_2.signe='+';
    }else{
        test_2.signe='-';
    }
    
    
    printf("Premier entier : ");
    afficher_unbounded_int(test_1);
    printf("Second entier : ");
    afficher_unbounded_int(test_2);
    unbounded_int test =unbounded_int_difference(test_1,test_2);
    printf("Resultat : ");
    afficher_unbounded_int(test);
}

void test_unbounded_int_produit(){
    int number;
    printf("Test de la fonction unbounded_int_produit :\n");
    srand(time(NULL));
    number=(rand()%9)+1;
    unbounded_int test_1=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_1=insertLast(test_1, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_1.signe='+';
    }else{
        test_1.signe='-';
    }
    
    number=rand()%10;
    unbounded_int test_2=create_empty_unbounded_int();
    for(int i=0;i<number; i++){
        number=rand()%10;
        char tmp=(char)number+'0';
        test_2=insertLast(test_2, tmp);
    }
    number=rand()%2;
    if(number==0){
        test_2.signe='+';
    }else{
        test_2.signe='-';
    }
    
    
    printf("Premier entier : ");
    afficher_unbounded_int(test_1);
    printf("Second entier : ");
    afficher_unbounded_int(test_2);
    unbounded_int test =unbounded_int_produit(test_1,test_2);
    printf("Resultat : ");
    afficher_unbounded_int(test);
}

int main(){
    

    return 0;
}