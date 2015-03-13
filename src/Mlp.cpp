#include "Mlp.h"


Mlp::Mlp() {
}


Mlp::~Mlp() {
    for (unsigned int i = 0; i < couche_entree.size(); i++)
        delete couche_entree[i];

    for (unsigned int i = 0; i < couche_cachee.size(); i++)
        delete couche_cachee[i];

    for (unsigned int i = 0; i < couche_sortie.size(); i++)
        delete couche_sortie[i];
}


// Attribution des données
void Mlp::set_data(std::vector< std::vector<double> >* examples_features, std::vector< std::vector<double> >* tests_features) {
    examples = examples_features;
    tests = tests_features;
}


// Creation structure reseau de neurones
void Mlp::run() {

    int neurones_caches = 20;

    // Creation des couches
    for (unsigned int i = 0; i < examples->at(0).size(); i++)
        couche_entree.push_back(new Neurone());

    for (unsigned int i = 0; i < neurones_caches; i++)
        couche_cachee.push_back(new Neurone());

    for (unsigned int i = 0; i < 10; i++)
        couche_sortie.push_back(new Neurone());


    // Initialisation aléatoire des poids (entre 0 et 1)
    for (unsigned int i = 0; i < couche_entree.size(); i++) {
        for (int j = 0; j < couche_cachee.size(); j++)
            couche_entree[i]->add_poids((double) rand() / (RAND_MAX));
    }

    for (unsigned int i = 0; i < couche_cachee.size(); i++) {
        for (int j = 0; j < couche_sortie.size(); j++)
            couche_cachee[i]->add_poids((double) rand() / (RAND_MAX));
    }

    apprentissage();
}


// Base d'apprentissage
void Mlp::apprentissage() {

    double err, totErr, moy_erreur = 1.0;
    std::vector<double> *erreurs;


    while (moy_erreur > 0.4) {

        err = 0;
        totErr = 0;
        erreurs = new std::vector<double>;

        for (unsigned int i = 0; i < examples->size(); i++) {

            // Attribution des données d'apprentissage à la couche d'entrée
            for (unsigned int j = 0; j < couche_entree.size(); j++) {
                couche_entree[j]->set_entree(examples->at(i).at(j));

                // Fonction d'activation neurones entree -> f(x) = x
                couche_entree[j]->set_sortie(examples->at(i).at(j));
            }


            // Attribution des données d'apprentissage à la couche de sortie
            for (unsigned int j = 0; j < couche_sortie.size(); j++)
                (j == (int)(i/20)) ? couche_sortie[j]->set_entree(1) : couche_sortie[j]->set_entree(0);


            // Propagation vers l'avant
            propagation();


            // Calcul erreur quadratique
            for (unsigned int j = 0; j < couche_sortie.size(); j++)
                err += pow(couche_sortie[j]->get_entree() - couche_sortie[j]->get_sortie(), 2);

            erreurs->push_back(0.5 * err);


            // Propagation vers l'arrière
            retro_propagation();


            // Mise à jour des poids
            delta_rule();
        }

        // Moyenne erreurs quadratiques sur l'apprentissage
        for (unsigned int i = 0; i < erreurs->size(); i++)
            totErr += erreurs->at(i);

        moy_erreur = totErr / erreurs->size();

        std::cout << moy_erreur << std::endl;
    }
}


/* Calcul de l'activation
 * choice = 0 : propagation couche entree -> couche cachee
 * choice = 1 : propagation couche cachee -> couche sortie
 */
void Mlp::propagation(int choice) {

    unsigned long sz_1 = (choice) ? couche_sortie.size() : couche_cachee.size();
    unsigned long sz_2 = (choice) ? couche_cachee.size() : couche_entree.size();

    for (unsigned int i = 0; i < sz_1; i++) {

        double x = 0.0;

        for (unsigned int j = 0; j < sz_2; j++)
            // Calcul intensite du signal reçu par j
            x += (choice) ? couche_cachee[j]->get_sortie() * couche_cachee[j]->get_poids(i)
                    : couche_entree.at(j)->get_sortie() * couche_entree.at(j)->get_poids(i);

        // Calcul activation neurones des couches entree et cachee -> f(x) = 1 / (1 + e(-x))
        (choice) ? couche_sortie[i]->set_sortie(1 / (1 + exp(-x))) : couche_cachee[i]->set_sortie(1 / (1 + exp(-x)));
    }

    if(!choice) propagation(++choice);
}


// Rétro-propagation du gradient
void Mlp::retro_propagation() {

    // Calcul gradients d'erreurs couche sortie
    for (unsigned int k = 0; k < couche_sortie.size(); k++)
        couche_sortie[k]->calcul_gradient();   // <- ak(1-ak)(yk-ak)


    // Calcul gradients d'erreurs couche cachee
    for (unsigned int j = 0; j < couche_cachee.size(); j++) {

        double tmp1 = 0.0;
        double tmp2 = couche_cachee[j]->get_sortie() * (1 - couche_cachee[j]->get_sortie());    // aj(1 - aj)

        for (unsigned int k = 0; k < couche_sortie.size(); k++)
            tmp1 += couche_sortie[k]->get_gradient() * couche_cachee[j]->get_poids(k);  // Somme [pour k appartenant aux indices des neurones
                                                                                        // prenant en entrée la sortie du neurone j] de dk * w_kj

        couche_cachee[j]->set_gradient(tmp2 * tmp1);
    }
}


/* Règle du delta
 * choice = 0 : correction poids des arcs couche entree -> couche cachee
 * choice = 1 : correction poids des arcs couche cachee -> couche sortie
 */
void Mlp::delta_rule(int choice) {

    double alpha = 0.1;     // taux d'apprentissage
    double delta;           // le pas

    unsigned long sz_1 = (choice) ? couche_cachee.size() : couche_entree.size();
    unsigned long sz_2 = (choice) ? couche_sortie.size() : couche_cachee.size();

    for (unsigned int i = 0; i < sz_1; i++) {
        for (unsigned int j = 0; j < sz_2; j++) {

            // Calcul du pas
            delta = (choice) ? alpha * couche_sortie[j]->get_gradient() * couche_cachee[i]->get_sortie()
                    : alpha * couche_cachee[j]->get_gradient() * couche_entree[i]->get_sortie();

            // Mise à jour du poids
            (choice) ? couche_cachee[i]->set_poids(j, couche_cachee[i]->get_poids(j) + delta)
                    : couche_entree[i]->set_poids(j, couche_entree[i]->get_poids(j) + delta);
        }
    }

    if(!choice) delta_rule(++choice);
}


// Test de l'apprentissage
void Mlp::test() {

    int cpt = 0;

    for(unsigned int i = 0; i < tests->size(); i++) {

        std::cout << i << " -> " << std::endl;

        // Attribution des données de test à la couche d'entrée
        for (unsigned int j = 0; j < couche_entree.size(); j++) {
            couche_entree[j]->set_entree(tests->at(i).at(j));
            couche_entree[j]->set_sortie(tests->at(i).at(j));
        }


        // Passage dans le reseau de neurones
        propagation();


        // Affichage des resultats
//        for (unsigned int j = 0; j < couche_sortie.size(); j++) {
//            std::cout << "Sortie " << j << " -> " << couche_sortie[j]->get_sortie() << std::endl;
//
//        }

//        for (unsigned int j = 0; j < couche_sortie.size(); j++) {
//            if((int)i/10 != j) cpt++;
//        }

        std::vector<Neurone*>::iterator it = std::max_element(couche_sortie.begin(), couche_sortie.end());

        if(it - couche_sortie.begin() != (int)i/10) cpt++;
    }

    std::cout << " -> " << cpt << " erreurs" << std::endl;
}