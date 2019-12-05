//Main.c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct caractere {
    char c;
    int fond;
    int couleur;
} TCaractere;

typedef struct suiteCaractere {
    TCaractere info;
    struct suiteCaractere * cs;   			//caractereSuivant
} TSuiteCaractere;

typedef struct suiteParagraphe {
    int numeroParagraphe;
    int quantiteCaractere;
    struct suiteParagraphe * ps;			//paragrapheSuivant
    struct suiteParagraphe * pp;			//paragraphePrecedent
    TSuiteCaractere * pc;					//premierCaractere
} TSuiteParagraphe;

typedef TSuiteParagraphe * PParagraphe;

typedef TSuiteCaractere * PCaractere;

/////////////////////////////////////////////////////////////////////////////////////////////////

//Retourne la quantite de caractere dans le paragraphe px, il faut donner px->pc
int quantiteCaractere(PCaractere pc) {		//Fonctionne
    int i=0;
    PCaractere py=pc;
    while(py->cs!=NULL)
    {
        i++;
        py=py->cs;
    }
    return i;
}

//Retourne la quantite de paragraphe dans la liste de paragraphe
int quantiteParagraphe(PParagraphe pdebut, PParagraphe pfin) {		//Fonctionne
	PParagraphe px=pdebut->ps;
	int i=0;
	while(px!=pfin) {
		px=px->ps;
		i++;
	}
	return i;
}

//Retourne le pointeur px de la case correspondante a la position donnee dans la liste paragraphe
PParagraphe pointeurPositionParagraphe(int position, PParagraphe pdebut) {		//Fonctionne 
    PParagraphe px=pdebut->ps;
    int i=0;
    while(i<position)
    {
        px=px->ps;
        i++;
    }
    return px;
}

//Retourne le pointeur px de la case correspondante a la position donne d un caractere dans un paragraphe
PCaractere pointeurPositionCaractere(int position, PCaractere pdebut) {		//Fonctionne
    PCaractere px=pdebut;
    int i=0;
    while(i<position)
    {
    	if(px->cs!=NULL) {
    		px=px->cs;
		}
        i++;
    }
    return px;
}

//Insertion d un nouveau paragraphe dans la liste paragraphe apres la case px, retourne le pointeur de la case creee
PParagraphe insertionParagraphe(PParagraphe px, PParagraphe pPoubelleParagrapheDebut, PParagraphe pPoubelleParagrapheFin, PCaractere pPoubelleCaractere) {		//Fonctionne
	if(pPoubelleParagrapheDebut->ps!=pPoubelleParagrapheFin && pPoubelleParagrapheFin->pp!=pPoubelleParagrapheDebut) {		//Si la liste poubelle de paragraphe n est pas vide
		px->ps->pp=pointeurPositionParagraphe(quantiteParagraphe(pPoubelleParagrapheDebut, pPoubelleParagrapheFin)-1, pPoubelleParagrapheDebut);
		px->ps->pp->pp->ps=pPoubelleParagrapheFin;
		pPoubelleParagrapheFin->pp=px->ps->pp->pp;
	} else {
		px->ps->pp=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
	}
//	px->ps->pp=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
    px->ps->pp->ps=px->ps;
    px->ps->pp->pp=px;
    px->ps=px->ps->pp;
    //Creation d un bidon pdebutC
    if(pPoubelleCaractere->cs!=NULL) {		//Si la liste poubelle de caractere n est pas vide
    	px->ps->pc=pointeurPositionCaractere(quantiteCaractere(pPoubelleCaractere), pPoubelleCaractere);
    	pointeurPositionCaractere((quantiteCaractere(pPoubelleCaractere)-1), pPoubelleCaractere)->cs=NULL;
	} else {
		px->ps->pc=(PCaractere)malloc(sizeof(TSuiteCaractere));
	}
    px->ps->quantiteCaractere=0;
    px->ps->pc->cs=NULL;
    return px->ps;
}

//Insertion d un nouveau caractere dans la liste caractere apres la case px, retourne le pointeur de la case cree
PCaractere insertionCaractere(PCaractere px, PCaractere pPoubelleCaractere) {		//Fonctionne
	PCaractere py;
    if(pPoubelleCaractere->cs!=NULL) {		//Si la liste poubelle de caractere n est pas vide
    	//Recuperer dans py le dernier caractere de la poubelle
    	py=pointeurPositionCaractere(quantiteCaractere(pPoubelleCaractere), pPoubelleCaractere);
    	//Decrocher ce dernier caractere de la poubelle
    	pointeurPositionCaractere(quantiteCaractere(pPoubelleCaractere)-1, pPoubelleCaractere)->cs=NULL;
	} else {		//Sinon creation d un nouveau caractere
		py=(PCaractere)malloc(sizeof(TSuiteCaractere));
	}
	if(px->cs==NULL) {		//Si il n y a pas de caractere apres px (si px est en bout de ligne)
		px->cs=py;
		py->cs=NULL;
	} else {		//Si il y a des caracteres apres px (si px n est pas en bout de ligne)
		py->cs=px->cs;
		px->cs=py;
	}
    return py;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

//Retourne la valeur decimale ASCII du caractere tape au clavier
int lireCaract() {		//Fonctionne

    char c=10;
    int fnt=0;

    c=getch();
    fnt=c;
    if (c==-32)
    {
        c=getch();
        fnt = 400+c;
    }
    if (c==0)
    {
        c=getch();
        fnt = 500+c;
    }
    return fnt;
}

//Deplace le curseur de la souris sur les coordonnes (x,y)
void positionChar(int lig, int col) {		//Fonctionne
    // ressources
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

//Affiche un caractere selon son code ASCII decimal, sa couleur de police et sa couleur de fond
void affiche(HANDLE hConsole, char c,int fond, int couleur) {		//Fonctionne
    int i;
    i=16*fond+couleur;
    SetConsoleTextAttribute(hConsole, i);
    printf("%c", c);
}

//Affiche tous les caracteres
void dynamiqueAffichageAll(HANDLE hConsole, PParagraphe pdebut, PParagraphe pfin) {
	PParagraphe px=pdebut;
	PCaractere py;
	system("cls");
	while(px->ps!=pfin) {
		px=px->ps;
		py=px->pc;
		while(py->cs!=NULL) {
			py=py->cs;
			SetConsoleTextAttribute(hConsole, 16*(py->info.fond)+py->info.couleur);
			printf("%c", py->info.c);
		}
		if(px->ps!=pfin) {
			printf("\n");
		}
	}
}

//Retourne la quantite de lignes pouvant etre affichee par la fenetre de commande
short tailleFenetre(HANDLE hConsole, short taille) {		//Fonctionne
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

//Affiche les valeurs de posY et posX en bas de fenetre
void position(int posX,int posY, HANDLE hConsole, short taille) {		//Fonctionne
    positionChar(tailleFenetre(hConsole, taille),0);
    printf("posX : %3d  posY : %3d", posX,posY);
    positionChar(posX,posY);
}

//Surligne les caracteres entre pyA et pyB (fond blanc police noir), px le paragraphe contenant pyA, pyA est AVANT pyB
void surligneur(PParagraphe px, PCaractere pyA, PCaractere pyB) {
	while(pyA!=pyB) {
		pyA->info.couleur=0;
		pyA->info.fond=15;
		if(pyA->cs!=NULL) {
			pyA=pyA->cs;
		} else {
			px=px->ps;
			pyA=px->pc->cs;
		}
	}
}

//Sauvegarde les donnes des listes dans le fichier texte.txt dans le repertoire d execution de l application
void enregistrer(HANDLE hConsole, PParagraphe pdebut, PParagraphe pfin) {		//Fonctionne
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
                if(px!=pfin)
                {
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
            if(px!=pfin)
            {
                fprintf(f, ":");
            }
        }
        fprintf(f, "*");

        MessageBeep(MB_OK);
        MessageBox(NULL, TEXT("Fichier enregistre sous : texte.txt"), TEXT("Enregistrer"),MB_OK);
        fclose(f);
    }
}

//Retourne 
PCaractere selectionner(PCaractere py, int posX,int posY, HANDLE hConsole, short taille, PParagraphe px) {		//A coder
    PCaractere pz = py;
    int i = -1, fond=15, couleur=0;
    while (i!=6){
        i = lireCaract();
        if (i==477) {
            if(posY<quantiteCaractere(px->pc)) {
                posY++;
                pz=pz->cs;
            } 
            affiche(hConsole, pz->info.c, fond, couleur);       //Réécrire le caractere avec le fond blanc et de police noir
            position(posX,posY,hConsole,taille);


        }
        if (i==475) {
            if(posY>=1) {
                posY--;
                if(posY==0) {
                    pz=px->pc;
                }
                else {
                    pz=pointeurPositionCaractere(posY, px->pc->cs);
                }
            }
            affiche(hConsole, pz->info.c, couleur, fond);
			position(posX,posY,hConsole,taille);
        }
        if (i==3) {
            pz=py;
            break;
        }
        if (i==26) {
            break;
        }
    }
    return pz;
}

//Affiche le texte sauvegarde dans le fichier texte.txt
void ouvrir(PParagraphe pdebut, PParagraphe pfin, PParagraphe pPoubelleParagrapheDebut, PParagraphe pPoubelleParagrapheFin, PCaractere pPoubelleCaractere) {		//Fonctionne
    int test = -1, i=0;
    PCaractere py;
    PParagraphe px;
    FILE *f = NULL;
    f = fopen("texte.txt", "r");
    fscanf(f, "%c", &test);
    fclose(f);
    f = fopen("texte.txt", "r");
    if(test==-1) {
        MessageBeep(MB_OK);
        MessageBox(NULL, TEXT("Le fichier 'texte.txt' est introuvable.\nAnnulation."), TEXT("Alerte"),MB_OK);
    }
    else {
        // -------------------------------------------- VIDER LA LISTE EXISTANTE
        pdebut->ps=pfin;
        pfin->pp=pdebut;
        pdebut->numeroParagraphe=-1;
        px=pdebut;
        do {
            px=insertionParagraphe(px, pPoubelleParagrapheDebut, pPoubelleParagrapheFin, pPoubelleCaractere);
            px->numeroParagraphe=px->pp->numeroParagraphe+1;
            py=px->pc;
            do {
                fscanf(f,"%c", &i);
                if(i!=42 && i!=58 && i!=59) { 						//i=42 => *
                    py=insertionCaractere(py, pPoubelleCaractere);
                    py->info.c=i;
                    px->quantiteCaractere++;
                    printf("%c", i);
                }
            }
            while(i!=58 && i!=42);						//i=58 => :
            if(i!=42)
            {
                printf("\n");
            }
        }
        while(i!=42);


    }
}

//Deplace le paragraphe px dans la liste pPoubelleParagraphe et retourne py le paragraphe avant px
PParagraphe deplacementParagraphe(PParagraphe px, PParagraphe pPoubelleParagrapheFin) {		//A tester
	PParagraphe py=px->pp;
	px->ps->pp=py;
	py->ps=px->ps;
	pPoubelleParagrapheFin->pp->ps=px;
	px->ps=pPoubelleParagrapheFin;
	px->pp=pPoubelleParagrapheFin->pp;
	pPoubelleParagrapheFin->pp=px;
	return py;
}

//Deplace toutes les cases caracteres entre pA (exclus) et pB (inclus) dans la liste pPoubelle appropriee, px correspond au paragraphe contenant pA
void deplacementPoubelle(PParagraphe px, PCaractere pA, PCaractere pB, PParagraphe pPoubelleParagrapheFin, PCaractere pPoubelleCaractere) {		//Fonctionne
	//pointeurPositionCaractere(quantiteCaractere(pPoubelleCaractere), pPoubelleCaractere)  => pointeur du dernier caractere dans la poubelle caractere
	PCaractere py;
	int fin=0;
	while(fin==0) {
		if(pA->cs==NULL) {		//Si il n y a pas de caractere apres pA
			//On passe au paragraphe suivant, on recolle le premier caractere apres pA et on retire le paragraphe vide
			pA->cs=px->ps->pc->cs;		//Deplacement des caracteres du paragraphe suivant apres pA
			pointeurPositionCaractere(quantiteCaractere(pPoubelleCaractere), pPoubelleCaractere)->cs=px->ps->pc;		//Deplacement du bidon du paragraphe suivant dans la poubelle caractere
			px=deplacementParagraphe(px->ps, pPoubelleParagrapheFin);		//Deplacement du paragraphe vide dans la poubelle paragraphe
		}
		py=pA->cs;
		if(py==pB) {
			fin=1;
		}
		if(py->cs==NULL) {		//Si py en bout de paragraphe
			pA->cs=NULL;
		} else {
			pA->cs=py->cs;
		}
		pointeurPositionCaractere(quantiteCaractere(pPoubelleCaractere), pPoubelleCaractere)->cs=py;
		py->cs=NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    SetConsoleTitle("Notepad--");
    int i, a=0, selection=0;																//=0 pas de selection en cours, =1 selection en cours
    int posX,posY;
    PParagraphe pdebut, pfin, px, pxA, pxB, pPoubelleParagrapheDebut, pPoubelleParagrapheFin;			//px est le pointeur du paragraphe courant
    PCaractere py, pyA, pyB, pPoubelleCaractere, pPressePapier;														//py est le pointeur du caractere courant
	int fond,couleur;
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    short taille = tailleFenetre(hConsole,0);
    fond=0;
    couleur=15;
    i=0;
    posX=0;												//Coordonnee sur les lignes / en ordonnee
    posY=0;												//Coordonnee sur les colonnes / en abscisse
	
    //Creation de la liste paragraphe
    pdebut=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
    pfin=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
    pdebut->ps=pfin;
    pfin->pp=pdebut;
	
	//Creation des listes de poubelle
	pPoubelleParagrapheDebut=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
	pPoubelleParagrapheFin=(PParagraphe)malloc(sizeof(TSuiteParagraphe));
	pPoubelleParagrapheDebut->ps=pPoubelleParagrapheFin;
	pPoubelleParagrapheFin->pp=pPoubelleParagrapheDebut;
	pPoubelleCaractere=(PCaractere)malloc(sizeof(TSuiteCaractere));
	pPoubelleCaractere->cs=NULL;
	
	//Creation du presse papier
	pPressePapier=(PCaractere)malloc(sizeof(TSuiteCaractere));
	pPressePapier->cs=NULL;
	
    //Creation du 1ier paragraphe
    px=insertionParagraphe(pdebut, pPoubelleParagrapheDebut, pPoubelleParagrapheFin, pPoubelleCaractere);
    px->numeroParagraphe=posX;
    py=px->pc;

    while (i!=3)  				//CTRL + C
    {
    	
    	
        //Verification de posX et posY, si hors limites ou valeurs interdites
        if(i==472) {		//Haut
        	if(posX<0) {		//Si trop haut
        		posX++;
			} else if(posY>quantiteCaractere(px->pp->pc)) {		//Si trop a droite par rapport a la ligne du dessus
				posY=quantiteCaractere(px->pp->pc);
			}
		}
        if(i==475) {		//Gauche
        	if(posY<0) {		//Si trop a gauche
        		if(posX>0) {
        			posX--;
        			posY=quantiteCaractere(px->pp->pc);
				} else if(posX==0) {
					posY++;
				}
			}
		}
        if(i==477) {		//Droite
        	if(posY>quantiteCaractere(px->pc)) {		//Si trop a droite
        		if(posX+1<quantiteParagraphe(pdebut, pfin)) {
        			posX++;
        			posY=0;
				} else if(posX+1==quantiteParagraphe(pdebut, pfin)) {
					posY--;
				}
			}
		}
        if(i==480) {		//Bas
        	if(posX+1>quantiteParagraphe(pdebut, pfin)) {		//Si trop bas
        		posX--;
			} else if(posY>quantiteCaractere(px->ps->pc)) {		//Si trop a droite par rapport a la ligne du dessous
				posY=quantiteCaractere(px->ps->pc);
			}
		}
        
		px=pointeurPositionParagraphe(posX, pdebut);
        py=pointeurPositionCaractere(posY, px->pc);
        position(posX, posY, hConsole, taille);		//position(quantiteCaractere(pPoubelleCaractere), quantiteParagraphe(pPoubelleParagrapheDebut, pPoubelleParagrapheFin), hConsole, taille); -> pour afficher les quantites de cases dans les poubelles
        positionChar(posX, posY);
		
		//Lecture du caractere tape au clavier
		i=lireCaract();
		
        if (i==13)  					//Entree
        {
            posX++;
            posY=0;
            if(py->cs==NULL) {		//Si retour a la ligne en bout de ligne
            	//Insertion d un nouveau paragraphe apres px
            	px=insertionParagraphe(px, pPoubelleParagrapheDebut, pPoubelleParagrapheFin, pPoubelleCaractere);
            	px->numeroParagraphe=posX;
            	py=px->pc;
			} else {		//Si retour a la ligne pas en bout de ligne (il y a des caracteres apres)
				px->quantiteCaractere=px->quantiteCaractere-quantiteCaractere(py);
				px=insertionParagraphe(px, pPoubelleParagrapheDebut, pPoubelleParagrapheFin, pPoubelleCaractere);
				px->pc->cs=py->cs;
				py->cs=NULL;
				px->numeroParagraphe=posX;
				py=px->pc;
				px->quantiteCaractere=quantiteCaractere(py);
			}
            dynamiqueAffichageAll(hConsole, pdebut, pfin);
        } else if (i==472) { 				//Fleche haut posX--
            posX--;
        } else if (i==475) { 				//Fleche gauche posY--
            posY--;
        } else if (i==477) { 				//Fleche droite posY++
	        posY++;
        } else if (i==480) { 				//Fleche bas posX++
            posX++;
        } else if (i==19) { 				//CTRL + S
            //Sauvegarder dans un fichier
            enregistrer(hConsole, pdebut, pfin);
            position(posX,posY,hConsole,taille);
        } else if (i==15) { 				//CTRL + O
            //Ouvrir depuis un fichier
            system("cls");
            ouvrir(pdebut, pfin, pPoubelleParagrapheDebut, pPoubelleParagrapheFin, pPoubelleCaractere);
            px=pfin->pp;	//Px sur dernier para
            py=px->pc;		//Py sur dernier caract
            posY=0;
            while(py->cs!=NULL) {
                py=py->cs;
                posY++;
            }
            posX=px->numeroParagraphe; //-1
            positionChar(posX,posY);
            position(posX, posY,hConsole,taille);
        } else if (i==4) { 				//CTRL + D debut de selection
        	selection=1;
            pyA=py->cs;
            pxA=px;
        } else if(i==6) {				//CTRL + F fin de selection
        	if(selection==1) {
        		selection=0;
        		pyB=py;
        		pxB=px;
			}
		} else if(i==26) {				//CTRL + z anulation de la selection
			if(selection==1) {
				selection=0;
			}
		} else if(i==22) {				//CTRL + v coller
			
			
		} else if (i==8) {                 //Backspace
            if(posY!=0) {		//Si on n efface pas un retour a la ligne
            	posY--;
	            py=pointeurPositionCaractere(posY, px->pc);
	            deplacementPoubelle(px, py, py->cs, pPoubelleParagrapheFin, pPoubelleCaractere);
	            position(posX,posY,hConsole,taille);
	            affiche(hConsole, 0, fond, couleur);
	            px->quantiteCaractere=px->quantiteCaractere-1;
			} else if(posY==0 && posX!=0) {		//Si on efface un retour a la ligne
				posX--;
				posY=quantiteCaractere(px->pp->pc);
				if(px->pc->cs!=NULL) {		//Si paragraphe contient des caracteres, il faut recuperer son contenu
					//Mise a jour du nombre de caractere dans le paragraphe recuperateur
					px->pp->quantiteCaractere=px->pp->quantiteCaractere+quantiteCaractere(px->pc);
					//Deplacement des caracteres du paragraphe px apres le dernier caractere du paragraphe precedent
					pointeurPositionCaractere(quantiteCaractere(px->pp->pc),  px->pp->pc)->cs=px->pc->cs;
				}
				px->pc->cs=NULL;
				pointeurPositionCaractere(quantiteCaractere(pPoubelleCaractere), pPoubelleCaractere)->cs=px->pc;		//Deplacement du bidon dans la liste poubelle
				px=deplacementParagraphe(px, pPoubelleParagrapheFin);		//Deplacement du paragraphe px dans la liste poubelle
				py=pointeurPositionCaractere(posY, px->pc);
			}
			dynamiqueAffichageAll(hConsole, pdebut, pfin);
        }
//		else if (i==560){fond=0;couleur=15;} 												// touche F2
//		else if (i==561){fond=10;couleur=15;} 												// touche F3
//		else if (i==562){fond=10;couleur=1;} 												// touche F3

        else if (i==3) {}				//CTRL + C sortir du programme
        else { 				//Affiche le caractere courant
            position(posX,posY,hConsole,taille);
            positionChar(posX,posY);

            py=insertionCaractere(py, pPoubelleCaractere);
            
            py->info.c=i;
            py->info.couleur=15;
            py->info.fond=0;
            px->quantiteCaractere=px->quantiteCaractere+1;
            affiche(hConsole, i, fond, couleur);
            posY=posY+1;
            
            if(py->cs!=NULL) {
            	dynamiqueAffichageAll(hConsole, pdebut, pfin);
			}
        }
    }

    positionChar(29,0);

    return 0;

}
