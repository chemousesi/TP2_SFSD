/**************************************************************************
 * Exemple de programme C manipulant un fichier T/OF / SFSD 2019 / benaziza&benakmoum *
 **************************************************************************/

#include "modele.h"
#include <stdlib.h>


int main()
{
    int choix , i;
   // Ouverture du fichier ...
   debut();
   // Menu principal ...
   do {
      	printf("\n--------- M E N U ---------\n");
	printf("1) Afficher l'entete du fichier\n");
	printf("2) Affichage de bloc(s)\n");
	printf("3) organiser selon pivot qui est %d \n", pivot);
	printf("0) Quitter le programme\n");

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












