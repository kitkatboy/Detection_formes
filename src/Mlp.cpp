#include "Mlp.h"


Mlp::Mlp() {
}


Mlp::~Mlp() {
}


void Mlp::run(std::vector< std::vector<double> >* examples_features, std::vector< std::vector<double> >* tests_features, int neurones_caches, double learning_err) {

    int errors;
    double time;

    std::cout << "\nMethode Perceptron multi-couches..." << std::endl;

    // Create perceptron
    create(neurones_caches, examples_features->at(0).size());

    // Start timer
    time_t seconds;
    seconds = clock();

    // Learning
    std::cout << "Apprentissage en cours avec " << neurones_caches << " neurones cachés et une erreur de " << learning_err << "..." << std::endl;
    learning(examples_features, learning_err);

    // Time calculation
    time = ((double) (clock() - seconds) / CLOCKS_PER_SEC);

    // Test
    errors = test(tests_features);


    std::cout << "-> " << errors << "% d'erreurs pour " << time << "s d'apprentissage\n" << std::endl;
}


// Creation structure reseau de neurones
void Mlp::create(int neurones_caches, unsigned long data_size) {

    // Creation des couches
    for (unsigned int i = 0; i < data_size; i++)
        couche_entree.push_back(Neurone());

    for (unsigned int i = 0; i < neurones_caches; i++)
        couche_cachee.push_back(Neurone());

    for (unsigned int i = 0; i < 10; i++)
        couche_sortie.push_back(Neurone());


    // Initialisation aléatoire des poids (entre -1 et 1)
    for (unsigned int i = 0; i < couche_entree.size(); i++)
        for (int j = 0; j < couche_cachee.size(); j++)
            couche_entree[i].add_poids(((double) rand() / (double)RAND_MAX) * (1 + 1) - 1);

    for (unsigned int i = 0; i < couche_cachee.size(); i++)
        for (int j = 0; j < couche_sortie.size(); j++)
            couche_cachee[i].add_poids(((double) rand() / (double)RAND_MAX) * (1 + 1) - 1);
}


// Base d'apprentissage
void Mlp::learning(std::vector< std::vector<double> >* examples, double learning_err) {

    double err, totErr, moy_erreur = 10;
    std::vector<double> erreurs;


    while (moy_erreur > learning_err) {

        err = 0;
        totErr = 0;
        erreurs.clear();
        std::vector<double>().swap(erreurs);

        for (unsigned int i = 0; i < 20; i++) {
            for (unsigned int j = 0; j < 10; j++) {

                // Attribution des données d'apprentissage à la couche d'entrée
                for (unsigned int k = 0; k < couche_entree.size(); k++) {
                    couche_entree[k].set_entree(examples->at(j*20+i).at(k));

                    // Fonction identité d'activation neurones entree -> f(x) = x
                    couche_entree[k].set_sortie(examples->at(j*20+i).at(k));
                }


                // Attribution des données d'apprentissage à la couche de sortie
                for (unsigned int k = 0; k < couche_sortie.size(); k++)
                    ((i * 20 + j) % 20 == k) ? couche_sortie[k].set_entree(0.9) : couche_sortie[k].set_entree(0.1);


                // Propagation vers l'avant
                propagation();


                // Calcul erreur quadratique
                for (unsigned int k = 0; k < couche_sortie.size(); k++)
                    err += (couche_sortie[k].get_entree() - couche_sortie[k].get_sortie())
                           * (couche_sortie[k].get_entree() - couche_sortie[k].get_sortie());

                erreurs.push_back(0.5 * err);


                // Propagation vers l'arrière
                retro_propagation();


                // Mise à jour des poids
                delta_rule();
            }
        }

        // Moyenne erreurs quadratiques sur l'apprentissage
        for (unsigned int i = 0; i < erreurs.size(); i++)
            totErr += erreurs[i];

        moy_erreur = totErr / erreurs.size();

//        std::cout << moy_erreur << std::endl;
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
            x += (choice) ? couche_cachee[j].get_sortie() * couche_cachee[j].get_poids(i)
                    : couche_entree[j].get_sortie() * couche_entree[j].get_poids(i);

        // Calcul sigmoide d'activation neurones des couches entree et cachee -> f(x) = 1 / (1 + e(-x))
        (choice) ? couche_sortie[i].set_sortie(1 / (1 + exp(-x))) : couche_cachee[i].set_sortie(1 / (1 + exp(-x)));
    }

    if(!choice) propagation(++choice);
}


// Rétro-propagation du gradient
void Mlp::retro_propagation() {

    // Calcul gradients d'erreurs couche sortie
    for (unsigned int k = 0; k < couche_sortie.size(); k++)
        couche_sortie[k].calcul_gradient();   // <- ak(1-ak)(yk-ak)


    // Calcul gradients d'erreurs couche cachee
    for (unsigned int j = 0; j < couche_cachee.size(); j++) {

        double tmp1 = 0.0;
        double tmp2 = couche_cachee[j].get_sortie() * (1 - couche_cachee[j].get_sortie());    // aj(1 - aj)

        for (unsigned int k = 0; k < couche_sortie.size(); k++)
            tmp1 += couche_sortie[k].get_gradient() * couche_cachee[j].get_poids(k);  // Somme [pour k appartenant aux indices des neurones
                                                                                      // prenant en entrée la sortie du neurone j] de dk * w_kj
        couche_cachee[j].set_gradient(tmp2 * tmp1);
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
            delta = (choice) ? alpha * couche_sortie[j].get_gradient() * couche_cachee[i].get_sortie()
                    : alpha * couche_cachee[j].get_gradient() * couche_entree[i].get_sortie();

            // Mise à jour du poids
            (choice) ? couche_cachee[i].set_poids(j, couche_cachee[i].get_poids(j) + delta)
                    : couche_entree[i].set_poids(j, couche_entree[i].get_poids(j) + delta);
        }
    }

    if(!choice) delta_rule(++choice);
}


// Reconnaissance
int Mlp::test(std::vector< std::vector<double> >* tests) {

    int err = 0, result;
    double tmp;

    for(unsigned int i = 0; i < tests->size(); i++) {

        result = 10;
        tmp = 0.0;

        // Attribution des données de test à la couche d'entrée
        for (unsigned int j = 0; j < couche_entree.size(); j++) {
            couche_entree[j].set_entree(tests->at(i).at(j));
            couche_entree[j].set_sortie(tests->at(i).at(j));
        }


        // Passage dans le reseau de neurones
        propagation();


        // Comparaison résultats attendus
        for(unsigned int j = 0; j < couche_sortie.size(); j++) {
            if(couche_sortie[j].get_sortie() > tmp) {
                tmp = couche_sortie[j].get_sortie();
                result = j;
            } else if(couche_sortie[j].get_sortie() == tmp) {
                result = 10;
            }
        }

        if(result != (int)i/10) err++;
    }

    return err;
}
