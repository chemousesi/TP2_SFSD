/**************************************************************************
 * Implémentation du modèle simplfié de fichier TOF / SFSD 2019 / Hidouci *
 **************************************************************************/




#include "modele.h"
#include <stdio.h>




tbloc buf;
tbloc bufa, bufb;






// ouvre un fichier et m-a-j de l'entête ent
void ouvrir( FILE **f, char *nom, char mode, t_entete *ent )
{
   if ( mode == 'A' || mode == 'a' ) {
	*f = fopen( nom, "rb+" );
	if ( *f == NULL ) perror( "ouvrir (probleme dans fopen)" );
	fread( ent, sizeof(t_entete), 1, *f );
   }
   else { // mode == 'N' || mode == 'n'
	*f = fopen( nom, "wb+" );
	if ( *f == NULL ) perror( "ouvrir (probleme dans fopen)" );
	ent->nb_bloc = 0;
	ent->nb_ins = 0;
	ent->nb_sup = 0;
	fwrite( ent, sizeof(t_entete), 1, *f );
   }
}


// ferme le fichier et sauvegarde de l'entete au début du fichier
void fermer( FILE *f, t_entete *ent )
{
   fseek(f, 0L, SEEK_SET);
   fwrite( ent, sizeof(t_entete), 1, f );
   fclose(f);
}


// lecture du bloc de données numéro i dans la variable buf
void lireDir( FILE *f, long i, tbloc *buf )
{
   fseek(f, sizeof(t_entete) + (i-1)*sizeof(tbloc), SEEK_SET);
   fread( buf, sizeof(tbloc), 1, f );
}


// écriture du contenu de buf dans le bloc de données numéro i
void ecrireDir( FILE *f, long i, tbloc *buf )
{
   fseek(f, sizeof(t_entete) + (i-1)*sizeof(tbloc), SEEK_SET);
   fwrite( buf, sizeof(tbloc), 1, f );
}



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








