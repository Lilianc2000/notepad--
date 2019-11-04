//Main.c
#include <stdio.h>
#include <stdlib.h>

typedef struct caractere {
  char c;
  int fond;
  int couleur;
}TCaractere; 
 
typedef struct suiteCaractere{
  TCaractere info;
  struct suiteCaractere * caractereSuivant;
}TSuiteCaractere; 
 
typedef struct suiteParagraphe{
  int numeroParagraphe ;
  struct suiteParagraphe * paragrapheSuivant;
  struct suiteParagraphe * paragraphePrecedent;
  TSuiteCaractere * premierCaractere;
} TSuiteParagraphe; 
 
typedef TSuiteParagraphe * PParagraphe;

typedef TSuiteCaractere * PCaractere;

void main(void) {
  
  
  
  
  
  
  
  
  
}
