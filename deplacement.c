#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

int lireCaract(){
	
	char c=10;
	int fnt=0;	
	
	c=getch();
	fnt=c;
	if (c==-32){c=getch();fnt = 400+c;}
	if (c==0)  {c=getch();fnt = 500+c;}
	return fnt;	
}

void gotoPosition( int lig, int col ){
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
	gotoPosition(29,0);
	printf("posX : %3d  posY : %3d", posX,posY);
}

int main()
{
  int i;
  int posX,posY;
//  int fond,couleur;
  
  HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

//	fond=0;couleur=15;
	i=0;posX=0;posY=0;
	
	boucle: while (i!=3){
	  	
	  	i=lireCaract();
	
		// pas de vrif de bornes sur X et Y
	
	  	if      (i== 13){posX=posX+1; posY=0;      gotoPosition(posX,posY);} // entree
		else if (i==472){posX=posX-1;              gotoPosition(posX,posY);} // fleche haut
		else if (i==475){posX=posX  ; posY=posY-1; gotoPosition(posX,posY);} // fleche gauche
		else if (i==477){posX=posX  ; posY=posY+1; gotoPosition(posX,posY);} // fleche droite
		else if (i==480){posX=posX+1; posY       ; gotoPosition(posX,posY);} // fleche bas
//		else if (i==560){fond=0;couleur=15;} // touche F2
//		else if (i==561){fond=10;couleur=15;} // touche F3
//		else if (i==562){fond=10;couleur=1;} // touche F3
		else if (i== 3){} 					// touche ESC sortir du programme
		else {         // affiche le caractere courant                     
		  	position(posX,posY);			
			gotoPosition(posX,posY);
			
			
			
		  	//affiche(hConsole, i, fond, couleur);
		  	//posY=posY+1;
	   }
	  	
  }

	gotoPosition(29,0);
	


}
