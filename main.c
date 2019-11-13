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
  struct suiteCaractere * cs;   			//caractereSuivant
}TSuiteCaractere; 
 
typedef struct suiteParagraphe{
  int numeroParagraphe;
  struct suiteParagraphe * ps;			//paragrapheSuivant
  struct suiteParagraphe * pp;			//paragraphePrecedent
  TSuiteCaractere * pc;					//premierCaractere
} TSuiteParagraphe; 
 
typedef TSuiteParagraphe * PParagraphe;

typedef TSuiteCaractere * PCaractere;

/////////////////////////////////////////////////////////////////////////////////////////////////

int lireCaract(){		//Fonctionne
	
	char c=10;
	int fnt=0;	
	
	c=getch();
	fnt=c;
	if (c==-32){c=getch();fnt = 400+c;}
	if (c==0)  {c=getch();fnt = 500+c;}
	return fnt;	
}

void positionChar( int lig, int col ){		//Fonctionne
	// ressources
	COORD mycoord;
	
	mycoord.X = col;
	mycoord.Y = lig;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

void affiche(HANDLE hConsole, char c,int fond, int couleur){
	int i;
	
	i=16*fond+couleur;
	SetConsoleTextAttribute(hConsole, i);
     printf("%c", c);
}

void position(int posX,int posY){		//Fonctionne
	positionChar(29,0);
	printf("posX : %3d  posY : %3d", posX,posY);
}

//Sauvegarde les donnes des listes dans le fichier teste.txt dans le repertoire d execution de l application
void enregistrer(HANDLE hConsole, PParagraphe pdebut, PParagraphe pfin){	//Fonctionne
    PParagraphe px;
    PCaractere py;
    int test = -1;
    FILE *f;
    f = fopen("texte.txt", "r");
    fscanf(f, "%c", &test);
    if(test!=-1){
        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_RED);
        printf("Le fichier existe d%cj%c, voulez-vous l'%ccraser ? (O/N)", 130, 133, 130);
        test = lireCaract();
        if(test==79 || test==111){
            fclose(f);
            f = fopen("texte.txt", "w+t");
            
            //Ecriture des donnees
            px=pdebut->ps;
            while(px!=pfin){
            	py=px->pc->cs;
                while(py!=NULL){
                    fprintf(f, "%c;", py->info.c);
                    py=py->cs;
                }
                fprintf(f, ":");
                px=px->ps;
            }
            
        	MessageBeep(MB_OK);
        	MessageBox(NULL, TEXT("Fichier enregistré sous : texte.txt"), TEXT("Enregistrer"),MB_OK);
        	fclose(f);
        }
        else {
            fclose(f);
        }
        printf("\x0d");                 //Curseur au début de la ligne et effacage du message
        SetConsoleTextAttribute(hConsole, 0);
        printf("                                                        ");
        printf("\x0d");
    }
    else {
        fclose(f);
        f = fopen("texte.txt", "w+t");
        
        //Ecriture des donnees
        px=pdebut->ps;
		while(px!=pfin){
			py=px->pc->cs;
            while(py!=NULL){
                fprintf(f, "%c;", py->info.c);
                py=py->cs;
            }
            fprintf(f, ":");
            px=px->ps;
        }
        
         MessageBeep(MB_OK);
         MessageBox(NULL, TEXT("Fichier enregistré sous : texte.txt"), TEXT("Enregistrer"),MB_OK);
         fclose(f);
    }
}



void selectionner(){		//A coder
	
}

void ouvrir(){		//A coder
	
}

//Retourne le pointeur px de la case correspondante a la position donnee dans la liste paragraphe
PParagraphe pointeurPositionParagraphe(int position, PParagraphe pdebut) {		//Fonctionne
	PParagraphe px=pdebut->ps;
	int i=1;
	while(i<position) {
		px=px->ps;
		i++;
	}
	return px;
}

//Retourne le pointeur px de la case correspondante a la position donne d un caractere dans un paragraphe
PCaractere pointeurPositionCaractere(int position, PCaractere pdebut) {			//Fonctionne
	PCaractere px=pdebut->cs;
	int i=1;
	while(i<position) {
		px=px->cs;
		i++;
	}
	return px;
}

//Insertion d un nouveau paragraphe dans la liste paragraphe apres la case px, retourne le pointeur de la case creee
PParagraphe insertionParagraphe(PParagraphe px) {		//Fonctionne
	px->ps->pp=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
	px->ps->pp->ps=px->ps;
	px->ps->pp->pp=px;
	px->ps=px->ps->pp;
	//Creation d un bidon pdebutC
	PCaractere pdebutC=(PCaractere)malloc(sizeof(TSuiteCaractere));
	px->ps->pc=pdebutC;
	pdebutC->cs=NULL;
	return px->ps;
}

//Insertion d un nouveau caractere dans la liste caractere apres la case px, retourne le pointeur de la case cree
PCaractere insertionCaractere(PCaractere px) {		//Fonctionne
	PCaractere py=(PCaractere)malloc(sizeof(TSuiteCaractere));
	py->cs=px->cs;
	px->cs=py;
	return px->cs;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	int i;
	int posX,posY;
	PParagraphe pdebut, pfin, px;						//px est le pointeur du paragraphe courant
	PCaractere py;										//py est le pointeur du caractere courant
	int fond,couleur;
  	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

	fond=0;couleur=15;
	i=0;posX=0;posY=0;
	
	//Creation de la liste paragraphe
	pdebut=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
	pfin=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
	pdebut->ps=pfin;
	pfin->pp=pdebut;
	
	//Creation du 1ier paragraphe
	px=insertionParagraphe(pdebut);
	px->numeroParagraphe=posX+1;
	py=px->pc;
	
	while (i!=3) {				//CTRL + C
	  	i=lireCaract();
		
		//Pas de verification de bornes sur X et Y
		
	  	if (i==13) {				//Entree
			posX=posX+1;
			posY=0;
			positionChar(posX,posY);
			//Insertion d un nouveau paragraphe apres px
			px=insertionParagraphe(px);
			py=px->pc;
		}
		else if (i==472) {				//Fleche haut
			posX=posX-1;
     	    positionChar(posX,posY);
		}
		else if (i==475) {				//Fleche gauche
			posX=posX;
			posY=posY-1;
			positionChar(posX,posY);
		}
		else if (i==477) {				//Fleche droite
			posX=posX;
			posY=posY+1;
			positionChar(posX,posY);
		} 
		else if (i==480) {				//Fleche bas
			posX=posX+1;
			posY;
			positionChar(posX,posY);
		}
    	else if (i==19) {				//CTRL + S
			enregistrer(hConsole, pdebut, pfin);
		}
    	else if (i==15) {				//CTRL + O
			ouvrir();
		}
    	else if (i==4) {				//CTRL + D
			selectionner();
		}
    
//		else if (i==560){fond=0;couleur=15;} 												// touche F2
//		else if (i==561){fond=10;couleur=15;} 												// touche F3
//		else if (i==562){fond=10;couleur=1;} 												// touche F3
             
		else if (i==3) {}				// touche CTRL + C sortir du programme
		else {				//Affiche le caractere courant                     
		  	position(posX,posY);			
			positionChar(posX,posY);
			
			//Enregistrement du caractere tape dans le paragraphe
			py=insertionCaractere(py);
			py->info.c=i;

		  	affiche(hConsole, i, fond, couleur);
		  	posY=posY+1;
	   }
	  	
  }

	positionChar(29,0);
	


}
