#include "Kppv.h"


Kppv::Kppv() {
}


Kppv::~Kppv() {
}


void Kppv::run(std::vector< std::vector<double> >* examples_features_2, std::vector< std::vector<double> >* tests_features_2) {

    int result, err, err_g = 0, unknow, unknow_g = 0;
    std::vector< std::vector<double> > *densities;

    std::cout << "\nMethode Classifieur par densites et k plus proches voisins..." << std::endl;

    // Apprentissage
    for(int i = 0; i < 10; i++) {   // Iteration lignes

        densities = new std::vector< std::vector<double> >;

        for(unsigned int j = 0; j < 20; j++)  // Iteration colonnes
            densities->push_back(examples_features_2->at(i * 20 + j));

        classes.push_back(densities);
    }

    // Test
    for(int i = 0; i < 10; i++) {   // Iteration lignes

        err = 0;
        unknow = 0;

        for(unsigned int j = 0; j < 10; j++) {  // Iteration colonnes

            result = proba(tests_features_2->at(i * 10 + j));

            if(result == 10) {
                unknow++;
            } else if(i != result) {
                err++;
            }
        }

        std::cout << "  -> " << err << " erreurs et " << unknow << " incertitudes pour les " << i << std::endl;
        err_g += err;
        unknow_g += unknow;
    }
    std::cout << "-> " << err_g << "% d'erreurs et " << unknow_g << "% d'incertitudes" << std::endl;

    writeFile();
}


int Kppv::proba(std::vector<double> a_classer) {

    int k = 5;  // nb de voisins
    int classe = 10;
    double tmp, result = -1;
    std::vector< std::pair<double, unsigned int> > results;
    std::vector<double> probabilites(10, 0.0);

    for(unsigned int i = 0 ; i < classes.size(); i++) {  // Iteration parmis ttes les classes
        for (unsigned int j = 0; j < classes.at(0)->size(); j++)    // Iteration ds une classe
            results.push_back(std::pair<double, unsigned int>(distance_euclidienne(a_classer, classes.at(i)->at(j)), i));
    }
    std::sort(results.begin(), results.end());

    for(int i = 0; i < k; i++)
        probabilites[results[i].second]++;

    for(std::vector<double>::iterator it = probabilites.begin(); it != probabilites.end(); ++it) {
        tmp = (*it != 0) ? (*it) /= k : 0.0;

        if(tmp > result) {
            result = tmp;
            classe = (int)(it - probabilites.begin());
        } else if(tmp == result) {
            classe = 10;
        }
    }

    vecteurs_probabilites->push_back(probabilites);

    return classe;
}


double Kppv::distance_euclidienne(std::vector<double> X, std::vector<double> Y) {
    double sum = 0;

    if(X.size() != Y.size()) std::cout << "calcul distance euclidienne impossible" << std::endl;

    for(unsigned int i = 0; i < X.size(); i++)
        sum += (X.at(i) - Y.at(i)) * (X.at(i) - Y.at(i));

    return sqrt(sum);   // Supp racine possible pr gain de perf
}


void Kppv::writeFile() {

    std::string output = "data/Kppv.proba";

    std::ofstream outputFile;
    outputFile.open(output.c_str());
    if (outputFile.is_open()) {

        for(unsigned int i = 0; i < vecteurs_probabilites->size(); i++) {
            for(unsigned int j = 0; j < vecteurs_probabilites->at(i).size(); j++)
                outputFile << vecteurs_probabilites->at(i).at(j) << "\t";
            outputFile << std::endl;
        }

        outputFile.close();
    } else {
        std::cout << "File " << output << " not found" << std::endl;
    }
}