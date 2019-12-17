/**************************************************************************
 * Exemple de programme C manipulant un fichier TOF / SFSD 2019 / Hidouci *
 **************************************************************************/



#include "modele.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

FILE *f = NULL;
t_entete ent;
tbloc buf;
tbloc bufa, bufb;
int pivot = 45
;

void charg();		// Chargement initial du fichier
void reorg();		// Réorganisation du fichier
void info();		// Affichage de l'entête
void rech();		// Recherche d'un enregistrement :
long dicho( long val, int *trouv, long *i, int *j ); // par dichotomie ou
long seq( long val, int *trouv, long *i, int *j );   // par recherche séquentielle
void ins();		// Insertion par décalages d'un enregistrement
void sup();		// Suppression logique d'un enregistrement
void parcours();  	// Affichage d'une séquence de blocs contigus (entre a et b)
void orga_selon_pivot(); // organise le fichier selon un pivot donner au debut les valeurs inf pui à la fin les valeurs inf



int main()
{
   int choix , i;
   char nom[20], mode[20];

   printf("Opérations d'accès sur un Fichier de type TOF\n");
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
	printf("2) Rechercher un enregistrement dans le fichier\n");
	printf("3) Affichage de bloc(s)\n");
	printf("4) organiser selon pivot qui est %d \n", pivot);
	printf("0) Quitter le programme\n");

	printf("\tchoix : ");
	scanf(" %d", &choix);
	printf("\n---------------------------\n\n");

	switch(choix) {
	   case 1: info(); break;
	   case 2: rech(); break;
	   case 3: parcours(); break;
	   case 4: orga_selon_pivot();break;
	}
   } while ( choix != 0);
   // Fermeture du fichier (sauvegarde de l'entete) ...
   fermer(f, &ent);

   return 0;

} // main



// Chargement initial à u% ...
void charg()
{
   double u;
   long i, k, n;
   int j;

   srand(time(NULL));
   printf("Chargment initial du fichier\n");
   printf("Donnez le nombre d'enregistrement à insérer : ");
   scanf(" %ld", &n);
   printf("Donnez le taux de chargement souhaité (entre 0 et 1) : ");
   scanf(" %lf", &u);
   if ( u < 1/MAXTAB ) u = 1/MAXTAB; // pour avoir au moins un enregistrement par bloc
   if ( u > 1 ) u = 1.0;	     // maximum = 100%

   printf("Remplissage des blocs avec %d enreg (sauf éventuellement le dernier)\n",\
	   (int)(MAXTAB*u));
   j = 0;
   i = 1;
   for (k=0; k<n; k++) {
	if ( j < MAXTAB*u ) {
	   buf.tab[j] = rand()%99 + 1 ; //generating a random number
	   buf.eff[j] = ' ';
	   j++;
	}
	else {
	   buf.nb = j;
	   ecrireDir(f, i, &buf);
	   i++;
	   buf.tab[0] = rand()%99 + 1 ;
	   buf.eff[0] = ' ';
	   j = 1;
	}
   }
   // dernière écriture ...
   buf.nb = j;
   ecrireDir(f, i, &buf);
   // m-a-j de l'entete ...
   ent.nb_bloc = i;
   ent.nb_ins = n;

} // charg


// afficher l'entete du fichier ...
void info()
{
   printf("Informations sur le fichier \n");
   printf("\tNombre de blocs utilisés = %ld\n", ent.nb_bloc);
   printf("\tNombre d'enregistrements insérés = %ld\n", ent.nb_ins);
   printf("\tNombre d'enregistrements supprimés = %ld\n", ent.nb_sup);
   printf("\tFacteur de chargement moyen = %ld\n", \
	(ent.nb_bloc == 0 ? 0 : (long)(((double)ent.nb_ins / (ent.nb_bloc*MAXTAB))*100)) );
} // info


// recherche d'une valeur dans le fichier:
// utilise soit la recherche dichotomique, soit la recherche séquentielle
void rech()
{
   long i, val, cpt;
   int trouv, j, choix;

   printf("Donnez la valeur cherchée : ");
   scanf(" %ld", &val);

	cpt = seq( val, &trouv, &i, &j );

   printf("Résultats de la recherche:\n");
   if (trouv && buf.eff[j] == ' ')
	printf("élément trouvé dans le bloc %ld à la position %d\n", i, j);
   else
	printf("élément non trouvé et devrait être dans le bloc %ld à la position %d\n", i, j);
   printf("Nombre de lectures physiques effectuées = %ld\n", cpt);

} // rech


// recherche sequentielle
// résultats: trouv (booléen), i (num de bloc), j (position dans le bloc)
// retourne aussi le nombre de lecture de blocs effectués.
long seq( long val, int *trouv, long *i, int *j )
{
   long cpt;
   int stop;

   *i = 1;
   *trouv = 0;
   stop = 0;
   cpt = 0; // compteur de lectures physiques
   while ( *i <= ent.nb_bloc && !*trouv && !stop ) {
	lireDir( f, *i, &buf ); cpt++;	// lecture physique
	*j = 0;
	while ( *j < buf.nb && !stop && !*trouv ) {
	   if ( val == buf.tab[*j] ) *trouv = 1;
	   else
		if ( val < buf.tab[*j] )
			stop = 1;
		else
			*j = *j + 1;
	} // while interne
	if ( !stop && !*trouv )
	   *i = *i + 1;
   } // while externe

   return cpt;

} // seq


// Insertion par décalages en réutilisant les emplacements des enreg effacés logiquement
//void ins()
//{
//   long i, val, sauv, cptR, cptI;
//   int trouv, continu, empl_recup, j;
//
//   printf("Insertion d'un enregistrement\n");
//   printf("Donnez la valeur à insérer : ");
//   scanf(" %ld", &val);
//
//   // recherche de la position (i,j) où insérer l'enregistrement
//   cptR = seq( val, &trouv, &i, &j );
//
//   printf("La recherche a coûté %ld lectures physiques.\n", cptR);
//   if (trouv && buf.eff[j] == ' ') {
//      // si l'enreg existe et n'est pas effacée logiquement, on arrete l'insertion.
//      printf("Insertion refusée car la valeur existe déjà dans le bloc %ld à la position %d\n"\
//	      , i, j);
//      return;
//   }
//
//   printf("La valeur doit être insérer dans le bloc %ld à la position %d", i, j);
//   printf(" ... "); fflush(stdout);
//
//   cptI = 0;	// compteur de lecture/ecriture dans la phase de décalages
//
//   empl_recup = 0; // indicateur de réutilisation d'un emplacement effacé logiquement
//		   // lors de l'insertion de la nouvelle valeur
//
//   if ( i > ent.nb_bloc ) {
//      // cas particulier d'une insertion en fin de fichier ...
//      buf.tab[0] = val;
//      buf.eff[0] = ' ';
//      buf.nb = 1;
//      ecrireDir( f, i, &buf ); cptI++;
//      ent.nb_bloc++;
//   }
//   else {
//      // cas général ...
//      continu = 1;
//      while ( continu ) {	// boucle principale pour les decalages inter-blocs
//	while ( continu && j < buf.nb ) {	// décalages intra-bloc ...
//	   sauv = buf.tab[j];
//	   buf.tab[j] = val;
//	   if ( buf.eff[j] == '*' ) {
//		continu = 0;
//		buf.eff[j] = ' ';
//	   }
//	   else {
//	   	j++;
//		val = sauv;
//	   }
//	}
//
//	if ( j == buf.nb ) 	// ou continu == 1
//	   if ( buf.nb < MAXTAB ) {
//		buf.nb++;
//		buf.tab[j] = val;  buf.eff[j] = ' ';
//		ecrireDir( f, i, &buf ); cptI++;
//		continu = 0;
//	   }
//	   else {
//		ecrireDir( f, i, &buf ); cptI++;
//		i++;  j = 0;
//		if ( i <= ent.nb_bloc ) {
//		   lireDir( f, i, &buf ); cptI++;
//		}
//		else {
//      	      	   // on a dépassé la fin de fichier, donc on rajoute un nouveau bloc ...
//      	      	   buf.tab[0] = val;
//      	      	   buf.eff[0] = ' ';
//      	      	   buf.nb = 1;
//      	      	   ecrireDir( f, i, &buf ); cptI++;
//     	      	   ent.nb_bloc++;
//	      	   continu = 0; // et on s'arrête
//		}
//	   }
//	else {
//	   // cas où les décalages se sont arrêtés sur un enreg effacé logiquement
//	   empl_recup = 1;
//	   ent.nb_sup--;	// car on réutilisé l'emplacement effacé logiquement
//	   ecrireDir( f, i, &buf ); cptI++;
//	}
//
//      } // fin de la boucle while principale
//
//   } // else (cas général)
//
//   if ( !empl_recup )		// si on n'a pas reutilisé un emplacement effacé logiq
//      ent.nb_ins++; 		// alors on incrémente le nombre d'insertions
//
//   printf("\nInsertion terminée. \n");
//   printf("\tCoût en opérations de lecture/écriture (rech:%ld + decl:%ld) = %ld\n", \
//		cptR, cptI, cptR+cptI);
//
//} // ins
//


// Affichage d'une séquence de blocs contigus (entre a et b)
void parcours()
{
   long i, a, b;
   int j;

   printf("Affichage d'une séquence de bloc(s). ayant des adresses dans [a , b] \n");
   printf("le fichiorga_selon_pivoter commence dans le bloc num 1 et se termine dans le bloc num %ld\n", ent.nb_bloc);
   printf("donnez a : ");
   scanf(" %ld", &a);
   printf("donnez b : ");
   scanf(" %ld", &b);
   for (i = a; i <= b; i++) {
      lireDir(f, i, &buf);
      printf("[Bloc Num:%3ld \t "); //NB = %2d \tCapacité max = %2d]\n", i, buf.nb, MAXTAB);
      for( j=0; j<buf.nb; j++)
	 if ( buf.eff[j] == ' ' )
	    printf("%ld ", buf.tab[j]);
	 else
	    printf("*%ld* ", buf.tab[j]);
      printf("\n--------------------------------------------------\n");

   }

} // parcours


int bloc_sup(tbloc buf2)   /** retourne vrai (1)  si tou le bloc est superieur au pivot 0 dans le cas contraire */
{
    int res = 1;
    int stop = 0;
    long i = 0;

    while (!(stop) & i < buf2.nb)
    {
        if (buf2.tab[i] <= pivot)
        {
            res = 0;
            stop = 1;
        }
        i++;
    }
    return res;
}

//-----------------------------------------------------------------------------
int bloc_inf(tbloc buf1) /** retourne vrai (1)  si tou le bloc est inferieur au pivot, 0 dans ele cas contraire */
{
    int res = 1;
    int stop = 0;
    long i = 0;

    while (!(stop) & i < buf1.nb)
    {
        if (buf1.tab[i] > pivot)
        {
            res = 0;
            stop = 1;
        }
        i++;
    }
    return res;
}

//---------------------------------

int chrg_exple(tbloc buf) /** je la ferai ultérieurement pour faire un exemple de chargement comme l'exemple du cours */
{

}


void organiseA(tbloc *buft)
    /** cette fonction fait la reorganisation du
    buffer les valeurs inf au puvot se mettent
    au debut du bloc sur le buf*/
{
    int temp, indice = 0;
    for(int i = 0;i < (*buft).nb;i++)
    {
        if ((*buft).tab[i] <= pivot)
        {

            temp = (*buft).tab[indice];
            (*buft).tab[indice] = (*buft).tab[i];
            (*buft).tab[i] = temp;
            indice++;
        }
    }
}


void orga_selon_pivot()
    /** cette procedure fait la reorganisation des bloc */

{
    // declarations
    printf("Donner votre pivot :");
    scanf("%d", &pivot);
    long a = 1; // ceci fera la borne inf bloc inferieur
    long b = ent.nb_bloc;// ceci fera la borne sup bloc inferieur
    int stop = 0;
    int tempo;
    while (!stop)
    {
        lireDir(f, a, &bufa);
        organiseA(&bufa);
        lireDir(f, b, &bufb);
        organiseA(&bufb);
        printf("a is %d b is %d\n", a, b);
        printf(" inf is %d and sup is %d\n", bloc_inf(bufa), bloc_sup(bufb));
        while(!(bloc_inf(bufa)) && !(bloc_sup(bufb)))
        {
            printf("here");
            int i = 0;
            while (bufa.tab[i] <= pivot)
            {
                i++;
            }
            tempo = bufa.tab[i];
            bufa.tab[i] = bufb.tab[0];
            bufb.tab[0] = tempo;
            organiseA(&bufb);

        }


        if (bloc_inf(bufa))
        {
            ecrireDir(f, a, &bufa);
            ecrireDir(f, b, &bufb);
            a++;
        }else if (bloc_sup(bufb))
            {
            ecrireDir(f, a, &bufa);
            ecrireDir(f, b, &bufb);
            b--;
            }


        if (a==b)
        {
            lireDir(f, a, &bufa);
            organiseA(&bufa);
            ecrireDir(f, a, &bufa);
            stop = 1;
        }

    }

}














