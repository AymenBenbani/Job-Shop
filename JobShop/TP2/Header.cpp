#include "Header.h"

void lire_fichier_instance(t_probleme& P, string nom)
{
    ifstream fichier(nom);
    if (fichier.is_open())
    {
        // Récupération du nombre de jobs et de machines
        fichier >> P.nb_de_job;
        fichier >> P.nb_de_machine;

        // Récupération du numéro et de la durée des machines
        for (int i = 0; i < P.nb_de_job; i++)
        {
            for (int j = 0; j < P.nb_de_machine; j++)
            {
                fichier >> P.machine[i][j];
                fichier >> P.duree_des_operation[i][j];
            }
        }
        fichier.close();
    }
}

void generer_vecteur(t_probleme P, t_solution& S)
{
    int* count = (int*)calloc(P.nb_de_job, sizeof(int));
    int taille_vecteur = P.nb_de_job * P.nb_de_machine;
    //initialisation
    for (int i = 0; i < P.nb_de_job; i++)
    {
        count[i] = P.nb_de_machine;
    }

    //generation du vecteur
    int r;
    for (int i = 0; i < taille_vecteur; i++)
    {
        r = rand() % P.nb_de_job;
        while (count[r] <= 0)
        {
            r = rand() % P.nb_de_job;
        }
        S.vecteur[i] = r;
        count[r]--;
    }
    free(count);
}

int verifier_vecteur(t_probleme& P,t_solution& S)
{
    int val = 1;
    int taille = P.nb_de_machine * P.nb_de_job;
    int tab[nb_max_job] ;
    

    /* Initialisation du tableau resultat */
    for (int i = 0; i < nb_max_job; i++)
    {
        tab[i] = 0;
        
    }

     //On compte le nombre de fois qu'apparait chaque pi�ce 
    for (int j = 0; j < taille; j++)
    {
        int m = S.vecteur[j];
        tab[m]++;
        
    }

     //Verification du nombre de fois qu'appara�t une piece
    for (int k = 0; k < P.nb_de_job; k++)
    {
        if (tab[k] != P.nb_de_machine)
        {
            val = 0;
        }
    }

    return val;
}


void evaluer_solution(t_probleme P, t_solution& S)
{
    // Initialisation 
    int nb_jobs_actuels[nb_max_job] = { 0 }, // Tableau pour suivre le nombre d'opérations déjà effectuées pour chaque job
        job = 0, // Job en cours
        machine = 0, // Machine en cours
        debut = 0, // Date de début de l'opération en cours
        fin_operation = 0, // Date de fin de l'opération en cours
        temps_operation = 0; // Durée de l'opération en cours
    t_couple nb_machines_actuelles[nb_max_machine] = { 0 }; // Tableau pour suivre les opérations en cours sur chaque machine

    // Init solution S
    for (int i = 0; i < nb_max_job; i++)
    {
        for (int j = 0; j < nb_max_machine; j++)
        {
            if (!i)
            {
                nb_machines_actuelles[j].numero_de_piece = -1;
                nb_machines_actuelles[j].rang_dans_la_gamme = -1;
            }
            S.St[i][j] = 0; // Initialise le temps de début de l'opération du job i sur la machine j
            S.pere[i][j].numero_de_piece = -1; // Initialise le job parent du job i sur la machine j
            S.pere[i][j].rang_dans_la_gamme = -1; // Initialise le rang du job parent du job i sur la machine j
        }
    }
    //Parcours
    for (int i = 0; i < P.nb_de_job * P.nb_de_machine; i++)
    {
        job = S.vecteur[i]; // Sélectionne le prochain job à traiter
        machine = P.machine[job][nb_jobs_actuels[job]]; // Sélectionne la machine pour le job en cours

        if (nb_jobs_actuels[job] > 0)
        {
            debut = S.St[job][nb_jobs_actuels[job] - 1]; // Récupère la date de début de la dernière opération du job
            fin_operation = debut + P.duree_des_operation[job][nb_jobs_actuels[job] - 1]; // Calcule la date de fin de l'opération en cours

            // Mise à jour du parent et de la date au plus tard si nécessaire
            if (fin_operation > S.St[job][nb_jobs_actuels[job]])
            {
                S.St[job][nb_jobs_actuels[job]] = fin_operation; // Met à jour la date de début de l'opération en cours
                S.pere[job][nb_jobs_actuels[job]].numero_de_piece = job; // Met à jour le job parent
                S.pere[job][nb_jobs_actuels[job]].rang_dans_la_gamme = nb_jobs_actuels[job] - 1; // Met à jour le rang du job parent
            }
        }

        // le cas d'une opération en cours
        if (nb_machines_actuelles[machine].numero_de_piece != -1 || nb_machines_actuelles[machine].rang_dans_la_gamme != -1)
        {
            debut = S.St[nb_machines_actuelles[machine].numero_de_piece][nb_machines_actuelles[machine].rang_dans_la_gamme]; // Récupère la date de début de l'opération sur la machine
            temps_operation = P.duree_des_operation[nb_machines_actuelles[machine].numero_de_piece][nb_machines_actuelles[machine].rang_dans_la_gamme]; // Récupère la durée de l'opération

            // Mise à jour de la solution si nécessaire
            if (debut + temps_operation > S.St[job][nb_jobs_actuels[job]])
            {
                S.St[job][nb_jobs_actuels[job]] = debut + temps_operation; // Met à jour la date de début de l'opération en cours
                S.pere[job][nb_jobs_actuels[job]] = nb_machines_actuelles[machine]; // Met à jour le job parent
            }
        }

       
        nb_machines_actuelles[machine].numero_de_piece = job; // Met à jour le job en cours sur la machine
        nb_machines_actuelles[machine].rang_dans_la_gamme = nb_jobs_actuels[job]; // Met à jour le rang du job en cours sur la machine
        nb_jobs_actuels[job]++; // Incrémente le nombre d'opérations effectuées pour le job en cours
    }

    S.cout = 0;

    // Recherche du parent du sommet étoile dans le chemin critique
    for (int i = 0; i < P.nb_de_job; i++)
    {
        debut = S.St[i][nb_jobs_actuels[i] - 1]; // Récupère la date de début de la dernière opération du job
        temps_operation = P.duree_des_operation[i][nb_jobs_actuels[i] - 1]; // Récupère la durée de l'opération

        // Si un nouveau chemin critique est trouvé
        if (debut + temps_operation > S.cout)
        {
            S.cout = debut + temps_operation; // Met à jour le coût de la solution
            S.sommet.numero_de_piece = i; // Met à jour le job du sommet étoile
            S.sommet.rang_dans_la_gamme = nb_jobs_actuels[i] - 1; // Met à jour le rang du sommet étoile
        }
    }
}


void afficher_Bierwith(t_probleme& P, t_solution& S)
{
    cout << "\n Bierwith : \n";
    cout << " [" << S.vecteur[0];
    for (int i = 1; i < P.nb_de_job * P.nb_de_machine; ++i)
    {
        // Afficher l'élément actuel
        cout << " " << S.vecteur[i];

        // Si ce n'est pas le dernier élément et que le reste de la division par 5 est égal à 4 (0, 1, 2, 3 ou 4)
        if (i < P.nb_de_job * P.nb_de_machine - 1 && (i + 1) % 5 != 0)
        {
            // Afficher une virgule et un espace
            cout << " ";
        }
        else
        {
            // Si on atteint un multiple de 5, afficher une barre verticale et un espace
            if (i < P.nb_de_job * P.nb_de_machine - 1)
            {
                cout << " | ";
            }
        }
    }

    cout << "]" << endl;
}



int recherche_locale(t_probleme P, t_solution& S, int max_iter)
{
    // initialisation variables 
    t_solution  S_nouveau;
    int pos1 = -1,
        pos2 = -1,
        index = -1,
        iterration = 0;
   
    evaluer_solution(P, S);

    t_couple i = S.sommet,//le sommet
        j = S.pere[i.numero_de_piece][i.rang_dans_la_gamme];//le pere

    while (iterration < max_iter && (j.numero_de_piece != -1 || j.rang_dans_la_gamme != -1))
    {
        int condition_sortie = 0; //on n'a pas encore trouve la solution meilleure

        // traitement d'arc disjonctif
        if (i.numero_de_piece != j.numero_de_piece) 
        {               
            for (pos1 = index = -1; index != i.rang_dans_la_gamme;) //recherche du position du sommet
                if (S.vecteur[++pos1] == i.numero_de_piece)
                    index++;
            for (pos2 = index = -1; index != j.rang_dans_la_gamme;)// recherche du position du pere
                if (S.vecteur[++pos2] == j.numero_de_piece)
                    index++;

            S_nouveau = S;

            // Échange les positions du pere et du sommet dans S_nouveau
            S_nouveau.vecteur[pos1] = S.vecteur[pos2];
            S_nouveau.vecteur[pos2] = S.vecteur[pos1];

            evaluer_solution(P, S_nouveau);

            // on a trouve une solution meilleure 
            if (S_nouveau.cout < S.cout)
            {
                S = S_nouveau;
                i = S.sommet;
                j = S.pere[i.numero_de_piece][i.rang_dans_la_gamme];
                condition_sortie = 1;
            }
        }
        //on ne trouve plus de solution meilleure 
        if (!condition_sortie)
        {
            i = j;
            j = S.pere[i.numero_de_piece][i.rang_dans_la_gamme];
        }
    }
    iterration++;
    return iterration;
}

void permutation(t_probleme& P, t_solution& S1, t_solution& S2)
{
    //choix aleatoire de 2 jobs 
    int indice1 = rand() % (P.nb_de_job * P.nb_de_machine);
    int indice2 = rand() % (P.nb_de_job * P.nb_de_machine);

    while (indice1 == indice2)
    {
        indice2 = rand() % (P.nb_de_job * P.nb_de_machine);
    }
    //echange de leur position 
    S1.vecteur[indice1] = S2.vecteur[indice2];
    S1.vecteur[indice2] = S2.vecteur[indice1];
}

void grasp(t_probleme P, t_solution& S, int nb_iter_max, int nb_de_voisins, int nb_iter_recherche_local)
{
    generer_vecteur(P, S);   //generer un vecteur de Bierwith aleatoire
    
    evaluer_solution(P, S);   //evaluer le vecteur de Bierwith

    //intialisation 
    t_solution best_solution = S,
        current_best_solution = S,
        neighbor_solution = best_solution;
    int indice1 = 0,
        indice2 = 0;

    for (int iter = 0; iter < nb_iter_max; iter++)
    {
        //Copier S dans current_best_solution
        current_best_solution = S;
                
        for (int i = 0; i < nb_de_voisins; i++)
        {
            neighbor_solution = best_solution;
           //echanger les positions de 2 jobs choisis aleatoirement 
            permutation(P, neighbor_solution, best_solution);
            recherche_locale(P, neighbor_solution, nb_iter_recherche_local);
            
            if (neighbor_solution.cout < current_best_solution.cout)   //solution est meilleure
            {
                current_best_solution = neighbor_solution;
            }
        }        
        best_solution = current_best_solution;
    }    
    S = best_solution;
}
