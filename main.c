/**************************************************************************
 * Exemple de programme C manipulant un fichier T/OF / SFSD 2019 / benaziza&benakmoum *
 **************************************************************************/



#include "modele.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>






int main()
{
   int choix , i;
   char nom[20], mode[20];

   printf("Opérations d'accès sur un Fichier de type TÔF\n");
   printf("Capacité maximale des blocs = %d enregistrements\t", MAXTAB);
   printf("Taille d'un bloc = %ld \tTaille entete = %ld\n\n", sizeof(tbloc), sizeof(t_entete) );

   // Ouverture du fichier ...
   printf("Donnez le nom du fichier : ");
   scanf(" %s", nom);
   printf("Ancien ou Nouveau ? (a/n) : ");
   scanf(" %s", mode);
   if ( mode[0] == 'a' || mode[0] == 'A' )
	ouvrir( &f, nom , 'A', &ent );
   else {
	ouvrir( &f, nom , 'N', &ent );
	charg();
   }

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
   fermer(f, &ent);

   return 0;

} // main












