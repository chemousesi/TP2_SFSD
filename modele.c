/**************************************************************************
 * Implémentation du modèle simplfié de fichier TOF / SFSD 2019 / chemsou&oussama *
 **************************************************************************/
#include <stdio.h>
#include <time.h>
#include "conio.h"
#include "modele.h"


/** declaration du fichier*/
FILE *f = NULL;

tbloc buf, bufa, bufb;
t_entete ent;



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
   printf("Donnez le nombre d'enregistrement %c ins%crer : ",133 , 130);
   scanf(" %ld", &n);
   printf("Donnez le taux de chargement souhait%c (entre 0 et 1) : ", 130);
   scanf(" %lf", &u);
   if ( u < 1/MAXTAB ) u = 1/MAXTAB; // pour avoir au moins un enregistrement par bloc
   if ( u > 1 ) u = 1.0;	     // maximum = 100%

   printf("Remplissage des blocs avec %d enreg (sauf %cventuellement le dernier)\n" ,\
	   (int)(MAXTAB*u), 130);
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
   printf("\tNombre de blocs utilis%cs = %ld\n",130,  ent.nb_bloc);
   printf("\tNombre d'enregistrements ins%cr%cs = %ld\n",130, 130, ent.nb_ins);
   printf("\tNombre d'enregistrements supprim%cs = %ld\n",130,  ent.nb_sup);
   printf("\tFacteur de chargement moyen = %ld\n", \
	(ent.nb_bloc == 0 ? 0 : (long)(((double)ent.nb_ins / (ent.nb_bloc*MAXTAB))*100)) );
} // info

// Affichage d'une séquence de blocs contigus (entre a et b)

void parcours()
{
   long i, a, b;
   int j;

   printf("Affichage d'une s%cquence de bloc(s). ayant des adresses dans [a , b] \n", 130);
   printf("le fichier commence dans le bloc num 1 et se termine dans le bloc num %ld\n", ent.nb_bloc);
   printf("donnez a : ");
   textcolor(GREEN);scanf(" %ld", &a);textcolor(WHITE);
   printf("donnez b : ");
   textcolor(GREEN);scanf(" %ld", &b);textcolor(WHITE);


    printf("\n");
   for (i = a; i <= b; i++) {
      lireDir(f, i, &buf);
      printf("[Bloc Num:%3ld \t "); //NB = %2d \tCapacité max = %2d]\n", i, buf.nb, MAXTAB);
      for( j=0; j<buf.nb; j++)
	 if ( buf.eff[j] == ' ' )
	    printf("%ld ", buf.tab[j]);
	 else
	    printf("*%ld* ", buf.tab[j]);
      printf("      ");
     printf("\n-------------------\n");
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
    textcolor(RED);
    scanf("%d", &pivot);
    textcolor(WHITE);
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

        while(!(bloc_inf(bufa)) && !(bloc_sup(bufb)))
            /** dans cette boucle on teste
            si le bufa a(inf) est en entier inférieur au pivot pour l'incrémenter
            ou si le buf b (sup) est en entier supérieur au pivot pour décrémenter
             */
        {
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
            ecrireDir(f, a, &bufa);
        if (bloc_inf(bufa))
        {
            ecrireDir(f, a, &bufa);
            a++;
        }else if (bloc_sup(bufb))
            {
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
    textcolor(GREEN);
    printf("Le fichier a %ct%c r%corganis%c selon le pivot %d avec succ%cs.", 130, 130, 130, 130, pivot, 138);
    textcolor(WHITE);
}


void debut()

/** prépare le debut du programme nom est ouvre le fichier
           crée le fichier quand il est nouveau */

{

   char nom[20], mode[20];
   textcolor(GREEN);
   printf("\n***************************************************\n");
   printf("Op%crations d'acc%cs sur un Fichier de type T/oF\n", 130, 138);
   printf("Capacit%c maximale des blocs = %d enregistrements\t\n\n",130,  MAXTAB);
    textcolor(WHITE);
   printf("Taille d'un bloc = %ld \tTaille entete = %ld\n\n", sizeof(tbloc), sizeof(t_entete) );

    printf("\nDonnez le nom du fichier : ");
   scanf(" %s", nom);
   printf("Ancien ou Nouveau ? (a/n) : ");
   scanf(" %s", mode);
   if ( mode[0] == 'a' || mode[0] == 'A' )
	ouvrir( &f, nom , 'A', &ent );
   else {
	ouvrir( &f, nom , 'N', &ent );
	charg();
   }

}


void fin() // ferme le fichier
{
    fermer(f, &ent);
}


