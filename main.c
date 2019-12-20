/**************************************************************************
 * Exemple de programme C manipulant un fichier T/OF / SFSD 2019 / benaziza&benakmoum *
 **************************************************************************/

#include "modele.h"

#include <stdlib.h>
#include "conio.c"

int main()
{
    int choix , i;
   // Ouverture du fichier ...
   pivot = 15;
    debut_exo7();
   // Menu principal ...
   do {
        textcolor(CYAN);
      	printf("\n--------- M E N U ---------\n");

	 textcolor(BLUE);printf("  1)");textcolor(WHITE); printf("Afficher l'entete du fichier\n");
	 textcolor(BLUE);printf("  2)");textcolor(WHITE);printf("Affichage de bloc(s)\n");
	 textcolor(BLUE);printf("  3)");textcolor(WHITE);printf("Organiser selon un pivot ( %d )\n", pivot);
	 textcolor(BLUE);printf("  0)");textcolor(WHITE); printf("Quitter le programme\n");

	printf("\tchoix : ");
	scanf(" %d", &choix);
	printf("\n---------------------------\n\n");

	switch(choix) {
	   case 1: info(); break;
	   case 2: parcours(); break;
	   case 3: orga_selon_pivot();break;
	}
   } while ( choix != 0);
   // Fermeture du fichier (sauvegarde de l'entete) ...
   fin();

   return 0;

} // main












