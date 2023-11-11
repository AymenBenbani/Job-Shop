#include <ctime>
#include "Header.h"

int main()
{
    srand(19);
    t_probleme P;
    t_solution S;
    int nb_iter_max = 5000,
        nb_de_voisins = 10,
        nb_iter_recherche_local = 500;
    int solution_optimale[20] = { 666,655,597,590,593,926,890,863,951,958,1222,1039,1150,1292,1207,945,784,848,842,902 };
    cout << " Nombre d'iterations de grasp : " << nb_iter_recherche_local << " | Nombre de voisins : " << nb_de_voisins << " | Nombre d'iterations max : " << nb_iter_max << "\n";
    for (int i = 0; i < 20; i++) 
    {
        //string fichier = "la";
        string namefile = "donnees/la";
        if (i < 9)
        {
            namefile.append("0");
            //fichier.append("0");
        }
        //fichier.append(to_string(i + 1));
        namefile.append(to_string(i + 1));
        namefile.append(".txt");

        lire_fichier_instance(P, namefile);  
                
        grasp(P, S, nb_iter_max, nb_de_voisins, nb_iter_recherche_local);
        
        /*
        if (verifier_vecteur(P, S))
            cout << "vecteur Bierwith est valide";
        else
            cout << "vecteur Bierwith non valide";
        cout << "\n";
        */
        cout << "\n";
        //cout << " " << fichier << " |";
        cout << " " << namefile;
        afficher_Bierwith(P, S);
        cout << " cout : " << S.cout << " | optimale : " << solution_optimale[i] << " | Difference : " << (S.cout / (float)solution_optimale[i] - 1) * 100<< " %" << endl;
    }
}

