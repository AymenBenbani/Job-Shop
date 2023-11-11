#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const int nb_max_job = 30;
const int nb_max_machine = 30;

//déclaration des structures
typedef struct t_probleme 
{
	int nb_de_job; 
	int nb_de_machine;
	int duree_des_operation[nb_max_job][nb_max_machine];
	int machine[nb_max_job][nb_max_machine];
}t_probleme;

typedef struct t_couple 
{
	int numero_de_piece;
	int rang_dans_la_gamme;
}t_couple;

typedef struct t_solution 
{
	int cout;
	int vecteur[nb_max_job * nb_max_machine];
	t_couple pere[nb_max_job][nb_max_machine];
	t_couple sommet;
	int St[nb_max_job][nb_max_machine];
}t_solution;


//déclaration des fonctions
void lire_fichier_instance(t_probleme& P, string nom);

void generer_vecteur(t_probleme P, t_solution& S);

int verifier_vecteur(t_probleme& P, t_solution& S);

void evaluer_solution(t_probleme P, t_solution& S);

void afficher_Bierwith(t_probleme& P, t_solution& S);

int recherche_locale(t_probleme P, t_solution& S,int nb_iter_max);

void permutation(t_probleme& P, t_solution& S1, t_solution& S2);

void grasp(t_probleme P, t_solution& S, int nb_iter_max, int nb_de_voisins, int nb_iter_recherche_local);


#endif