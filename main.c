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
  int quantiteCaractere;
  struct suiteParagraphe * ps;			//paragrapheSuivant
  struct suiteParagraphe * pp;			//paragraphePrecedent
  TSuiteCaractere * pc;					//premierCaractere
} TSuiteParagraphe;

typedef TSuiteParagraphe * PParagraphe;

typedef TSuiteCaractere * PCaractere;

/////////////////////////////////////////////////////////////////////////////////////////////////

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
	PCaractere px=pdebut;
	int i=0;
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
	px->ps->quantiteCaractere=0;
	px->ps->pc=pdebutC;
	pdebutC->cs=NULL;
	return px->ps;
}

//Insertion d un nouveau caractere dans la liste caractere apres la case px, retourne le pointeur de la case cree
PCaractere insertionCaractere(PCaractere px) {		//Fonctionne
	PCaractere py=(PCaractere)malloc(sizeof(TSuiteCaractere));
	py->cs=px->cs;
	px->cs=py;
	px->cs->cs=NULL;
	return px->cs;
}

//Retourne la quantite de caractere dans le paragraphe px
int quantiteCaractere(PParagraphe px) {
	int i=0;
	PCaractere py=px->pc;
	while(py->cs!=NULL) {
		i++;
		py=py->cs;
	}
	return i;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

short tailleFenetre(HANDLE hConsole, short taille){
	positionChar(taille, 0);
	printf("                         ");
	positionChar(taille, 0);
	CONSOLE_SCREEN_BUFFER_INFO info;
	short rows;
	short columns;
	hConsole = CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (GetConsoleScreenBufferInfo(hConsole, &info) == 0)
		return 1;
	CloseHandle(hConsole);
	columns = info.srWindow.Right - info.srWindow.Left + 1;
	rows = info.srWindow.Bottom - info.srWindow.Top;
//	wprintf(L"%d columns by %d rows\n", columns, rows);
	return rows;
}

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

void position(int posX,int posY, HANDLE hConsole, short taille){		//Fonctionne
	positionChar(tailleFenetre(hConsole, taille),0);
	printf("posX : %3d  posY : %3d", posX,posY);
	positionChar(posX,posY);
}

//Sauvegarde les donnes des listes dans le fichier teste.txt dans le repertoire d execution de l application
void enregistrer(HANDLE hConsole, PParagraphe pdebut, PParagraphe pfin) 	//Fonctionne
{
    PParagraphe px;
    PCaractere py;
    int test = -1;
    FILE *f = NULL;
    f = fopen("texte.txt", "r");
    fscanf(f, "%c", &test);
    if(test!=-1)                                                                                                     //Test s'il y a déja un fichier rempli
    {
        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_RED);
        positionChar(28,0);
        printf("\nLe fichier existe d%cj%c, voulez-vous l'%ccraser ? (O/N)", 130, 133, 130);
        test = lireCaract();
        if(test==79 || test==111)
        {
            fclose(f);
            f = fopen("texte.txt", "w+t");

            //Ecriture des donnees
            px=pdebut->ps;
            while(px!=pfin)
            {
                py=px->pc->cs;
                while(py!=NULL)
                {
                    fprintf(f, "%c;", py->info.c);
                    py=py->cs;
                }
                px=px->ps;
                if(px!=pfin) {
                	fprintf(f, ":");
				}
            }
			fprintf(f, "*");
            MessageBeep(MB_OK);
            MessageBox(NULL, TEXT("Fichier enregistre sous : texte.txt"), TEXT("Enregistrer"),MB_OK);
            fclose(f);
        }
        else
        {
            fclose(f);
        }
        printf("\x0d");                 //Curseur au début de la ligne et effacage du message
        SetConsoleTextAttribute(hConsole, 0);
        printf("                                                        ");
        printf("\x0d");

    }
    else
    {
        fclose(f);
        f = fopen("texte.txt", "w+t");

        //Ecriture des donnees
        px=pdebut->ps;
        while(px!=pfin)
            {
                py=px->pc->cs;
                while(py!=NULL)
                {
                    fprintf(f, "%c;", py->info.c);
                    py=py->cs;
                }
                px=px->ps;
                if(px!=pfin) {
                	fprintf(f, ":");
				}
            }
			fprintf(f, "*");

    	MessageBeep(MB_OK);
        MessageBox(NULL, TEXT("Fichier enregistre sous : texte.txt"), TEXT("Enregistrer"),MB_OK);
        fclose(f);
    }
}



void selectionner() 		//A coder
{

}

void ouvrir(PParagraphe pdebut, PParagraphe pfin) {		//A tester
    int test = -1, i=0;
    PCaractere py;
    PParagraphe px;
    FILE *f = NULL;
    f = fopen("texte.txt", "r");
    fscanf(f, "%c", &test);
    fclose(f);
    f = fopen("texte.txt", "r");
    if(test==-1)
    {
        MessageBeep(MB_OK);
        MessageBox(NULL, TEXT("Le fichier 'texte.txt' est introuvable.\nAnnulation."), TEXT("Alerte"),MB_OK);
    }
    else
    {
        // -------------------------------------------- VIDER LA LISTE EXISTANTE
        pdebut->ps=pfin;
        pfin->pp=pdebut;
        pdebut->numeroParagraphe=-1;
		px=pdebut;

        do {
        	px=insertionParagraphe(px);
        	px->numeroParagraphe=px->pp->numeroParagraphe+1;
        	py=px->pc;
            do {
                fscanf(f,"%c", &i);
                if(i!=42 && i!=58 && i!=59) {						//i=42 => *
                	py=insertionCaractere(py);
                	py->info.c=i;
                	px->quantiteCaractere++;
                	printf("%c", i);
				}

            }while(i!=58 && i!=42);						//i=58 => :
            if(i!=42) {
            	printf("\n");
			}
        }while(i!=42);


    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
	SetConsoleTitle("Notepad--");
	int i, a=0;
	int posX,posY;
	PParagraphe pdebut, pfin, px;						//px est le pointeur du paragraphe courant
	PCaractere py;										//py est le pointeur du caractere courant
	int fond,couleur;
  	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    short taille = tailleFenetre(hConsole,0);
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

	  	if (i==13) {					//Entree
			posX=posX+1;
			posY=0;
			//Insertion d un nouveau paragraphe apres px
			px=insertionParagraphe(px);
			py=px->pc;

			position(posX,posY,hConsole,taille);
     	    positionChar(posX,posY);
		}
		else if (i==472) {				//Fleche haut posX--
			if(posX>=1) {
				posX--;
				px=px->pp;
			}

			position(posX,posY,hConsole,taille); positionChar(posX,posY);
		}
		else if (i==475) {				//Fleche gauche posY--
			if(posY>=1) {
				posY--;
				if(posY==0) {
					py=px->pc;
				} else {
					py=pointeurPositionCaractere(posY, px->pc->cs);
				}
			}

			position(posX,posY,hConsole,taille); positionChar(posX,posY);
		}
		else if (i==477) {				//Fleche droite posY++
			if(posY<quantiteCaractere(px)) {
				posY++;
				py=py->cs;
			}

			position(posX,posY,hConsole,taille); positionChar(posX,posY);
		}
		else if (i==480) {				//Fleche bas posX++
			if(px->ps!=pfin) {
				posX++;
				px=px->ps;
			}

			position(posX,posY,hConsole,taille); positionChar(posX,posY);
		}
		else if (i==19) {				//CTRL + S
    		//Sauvegarder dans un fichier
			enregistrer(hConsole, pdebut, pfin);
			position(posX,posY,hConsole,taille);
		}
    	else if (i==15) {				//CTRL + O
    		//Ouvrir depuis un fichier
			ouvrir(pdebut, pfin);
			px=pfin->pp;	//Px sur dernier para
			py=px->pc;		//Py sur dernier caract
			posY=0;
			while(py->cs!=NULL) {
				py=py->cs;
				posY=posY++;
			}
			posX=px->numeroParagraphe-1;
			positionChar(posX,posY);
			position(posX, posY,hConsole,taille);
		}
    	else if (i==4) {				//CTRL + D
			selectionner();
		}

		else if (i==8){                 //Backspace
			if (a==1){                  //Si un backspace à été entrée précédemment
               			 py=pointeurPositionCaractere(posY-1,px); //On se place sur la case d'avant
		   	 }
		  	 a=1;
            		 py->info.c=NULL;
           		 posY=posY-1;
           		 position(posX,posY,hConsole,taille);
          		 affiche(hConsole, 0, fond, couleur);
          		 px->quantiteCaractere=px->quantiteCaractere-1;
		}
//		else if (i==560){fond=0;couleur=15;} 												// touche F2
//		else if (i==561){fond=10;couleur=15;} 												// touche F3
//		else if (i==562){fond=10;couleur=1;} 												// touche F3

		else if (i==3) {}				// touche CTRL + C sortir du programme
		else {				//Affiche le caractere courant
		  	position(posX,posY,hConsole,taille);
			positionChar(posX,posY);

			//Enregistrement du caractere tape dans le paragraphe
			if(a!=1){py=insertionCaractere(py);}
			else{a=0;}
			py->info.c=i;
			px->quantiteCaractere=px->quantiteCaractere+1;
		  	affiche(hConsole, i, fond, couleur);
		  	posY=posY+1;
	   }
	}

	positionChar(29,0);

	return 0;

}
