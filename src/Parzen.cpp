#include "Parzen.h"


Parzen::Parzen() {
}


Parzen::~Parzen() {
        delete examples_densities;
}


void Parzen::run(std::vector <std::vector<double> > *examples, std::vector <std::vector<double> > *tests) {

        int result, err = 0;

        std::cout << "\nMethode Classifieur par densites en Fenêtrage de Parzen et k plus proches voisins..." << std::endl;

        // Learning
        for(unsigned int i = 0; i < examples->size(); i++)
                examples_densities->push_back(density(examples->at(i)));


        // Test
        for(int i = 0; i < 10; i++)   // Iterate on matrix lines
                for(unsigned int j = 0; j < 10; j++) {  // Iterate on matrix columns

                        result = kppv(density(tests->at(i * 10 + j)));

                        if(i != result) err++;
                }

        std::cout << "-> " << err << "% d'erreurs" << std::endl;
}


double Parzen::density(std::vector<double> vec) {

        double h = 0.5;    // côte de l'hypercube
        int K = 0;         // cpt de pts ds l'hypercube par vecteur
        double tmp;

        // Kernel
        for(unsigned int i = 0; i < vec.size(); i++) {

                tmp = (vec[i] < 0) ? vec[i] * (-1) : vec[i];      // Valeur absolue

                K += (tmp < h / 2) ? 1 : 0;
        }

        return (1 / (vec.size() * pow(h, vec.size()))) * K;
}


int Parzen::kppv(double a_classer) {

        int k = 5;          // Nearest Neighbors
        int classe = 10;
        double tmp, result = -1;
        std::vector< std::pair<double, unsigned int> > results;
        std::vector<double> probabilites(10, 0.0);

        for(unsigned int i = 0 ; i < 10; i++)  // Iterate on classes
                for (unsigned int j = 0; j < 20; j++) {    // Iterate in class
                        tmp = a_classer - examples_densities->at(i * 20 + j);
                        tmp *= (tmp < 0) ? -1 : 1;   // Valeur absolue
                        results.push_back(std::pair<double, unsigned int>(tmp, i));
                }

        std::sort(results.begin(), results.end());

        // Neighbors counters
        for(int i = 0; i < k; i++)
                probabilites[results[i].second]++;

        // Get maximum probability
        for(std::vector<double>::iterator it = probabilites.begin(); it != probabilites.end(); ++it) {
                tmp = (*it != 0) ? (*it) /= k : 0.0;

                if(tmp > result) {
                        result = tmp;
                        classe = (int)(it - probabilites.begin());
                } else if(tmp == result) {
                        classe = 10;
                }
        }

        return classe;
}