/************************************************************************
 * Interface du modèle simplifé d'un fichier TOF / SFSD 2019 / Hidouci 	*
 * les types des blocs de données et de l'entête 			*
 * les fonctions du modèle : ouvrir, fermer, lireDir, ecrireDir ...	*
 ************************************************************************/


#ifndef MODELE_H
#define MODELE_H

#include <stdio.h>

// capacité maximale d'un bloc de données (en nombre d'enregistrements)
#define MAXTAB 5
int pivot;
FILE *f = NULL;



// type d'un bloc de données (et donc des buffers aussi)
typedef struct blc {
	   long tab[MAXTAB];	// tableau d'enregistremnts (des entiers longs)
	   char eff[MAXTAB];	// indicateurs d'effacement logique ('*' effacé / ' ' non effacé)
	   int nb;		// nombre d'enregistrements dans le bloc
  	   //char pad[4];		// pas nécessaire, juste pour avoir une taille de bloc de 512 octets
        } tbloc;


// type du bloc d'entête (les caractéristiques du fichier)
typedef struct entete {
	   // nombre de blocs utilisé par les enreg (c'est aussi le num du dernier bloc)
	   long nb_bloc;
	   // nombre d'enregistrements dans le fichier
	   long nb_ins;
	   // nombre d'enregistrements supprimés (logiquement) dans le fichier
	   long nb_sup;
	   // pas nécessire, juste pour compléter la taille de l'entête jusqu'à 512 octes
	   char pad[488];
	} t_entete;


// declaration de l'entete
t_entete ent;


// ouvrir un fichier en mode 'N' ou 'A'
// retourne le flux f et l'entête ent
void ouvrir( FILE **f, char *nom, char mode, t_entete *ent );

// fermer le fichier en sauvegardant l'entête ent dans le 1er bloc (num 0) du fichier
void fermer( FILE *f, t_entete *ent);

// lecture du bloc de données num i dans la variable buf
void lireDir( FILE *f, long i, tbloc *buf );

// ecriture du contenu de la variable buf dans le bloc de données num i
void ecrireDir( FILE *f, long i, tbloc *buf );


void charg();		// Chargement initial du fichier
void info();		// Affichage de l'entête
void parcours();  	// Affichage d'une séquence de blocs contigus (entre a et b)
void orga_selon_pivot(); // organise le fichier selon un pivot donner au debut les valeurs inf pui à la fin les valeurs inf




#endif

