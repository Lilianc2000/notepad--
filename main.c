//Main.c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////

int lireCaract(){
	
	char c=10;
	int fnt=0;	
	
	c=getch();
	fnt=c;
	if (c==-32){c=getch();fnt = 400+c;}
	if (c==0)  {c=getch();fnt = 500+c;}
	return fnt;	
}

void positionChar( int lig, int col ){
	// ressources
	COORD mycoord;
	
	mycoord.X = col;
	mycoord.Y = lig;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

//void affiche(HANDLE hConsole, char c,int fond, int couleur){
//	int i;
	
//	i=16*fond+couleur;
//	SetConsoleTextAttribute(hConsole, i);
//     printf("%c", c);
//}

void position(int posX,int posY){
	positionChar(29,0);
	printf("posX : %3d  posY : %3d", posX,posY);
}

void enregistrer(){}

void selectionner(){}

void ouvrir(){}

PParagraphe pointeurPositionParagraphe(int position, PParagraphe pdebut) { //EN COURS DE MODIF
	ptMaillon px=pdebut->ps;
	int i=1;
	while(i<position) {
		px=px->ps;
		i++;
	}
	return px;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
  int i;
  int posX,posY;
//  int fond,couleur;
  
  HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

//	fond=0;couleur=15;
	i=0;posX=0;posY=0;
	
	boucle: while (i!=3){                                               //CTRL + C
	  	
	  	i=lireCaract();
	
		// pas de vrif de bornes sur X et Y
	
	  if (i== 13){posX=posX+1; posY=0;      positionChar(posX,posY);} // entree
		else if (i==472){posX=posX-1;              positionChar(posX,posY);} // fleche haut
		else if (i==475){posX=posX  ; posY=posY-1; positionChar(posX,posY);} // fleche gauche
		else if (i==477){posX=posX  ; posY=posY+1; positionChar(posX,posY);} // fleche droite
		else if (i==480){posX=posX+1; posY       ; positionChar(posX,posY);} // fleche bas
    else if (i==19){enregistrer();}                                      //CTRL + S
    else if (i==15){ouvrir();}                                           //CTRL + O
    else if (i==4){selectionner();}                                      //CTRL + D
    
//		else if (i==560){fond=0;couleur=15;} // touche F2
//		else if (i==561){fond=10;couleur=15;} // touche F3
//		else if (i==562){fond=10;couleur=1;} // touche F3
             
		else if (i== 3){} 					// touche CTRL + C sortir du programme
		else {         // affiche le caractere courant                     
		  	position(posX,posY);			
			positionChar(posX,posY);
			
			
			
		  	//affiche(hConsole, i, fond, couleur);
		  	//posY=posY+1;
	   }
	  	
  }

	positionChar(29,0);
	


}
