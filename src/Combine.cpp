#include "Combine.h"


Combine::Combine() {
}


Combine::~Combine() {
}


void Combine::run() {

    std::cout << "\nMethode par combinaison de classifieurs..." << std::endl;

    // Get vectors from files
    entree_1 = readFile("data/Distances.proba");
    entree_2 = readFile("data/Kppv.proba");

    calcul();
}


std::vector< std::vector<double>* >* Combine::readFile(std::string file_path) {

    std::vector< std::vector<double>* >* tmp2 = new std::vector< std::vector<double>* >;

    std::ifstream inputFile;
    inputFile.open(file_path.c_str());
    if (inputFile.is_open()) {

        std::string rstring;
        std::vector<double> *tmp;

        for(int i = 0; i < 100; i++) {

            tmp = new std::vector<double>;

            for(int j = 0; j < 10; j++) {
                inputFile >> rstring;
                tmp->push_back(atof(rstring.c_str()));
            }
            tmp2->push_back(tmp);
        }
    }
    inputFile.close();

    return tmp2;
}


/*
 * choice = 0 -> Somme
 * choice = 1 -> Produit
 */
void Combine::calcul(int choice) {

    int err = 0, unknow = 0;
    double sum = 0;
    std::vector<double> *probabilites;

    for(unsigned long i = 0; i < 100; i++) {

        sum = 0;
        probabilites = new std::vector<double>;

        for(unsigned long j = 0; j < 10; j++)
            sum += (choice) ? entree_1->at(i)->at(j) * entree_2->at(i)->at(j)
                            : entree_1->at(i)->at(j) + entree_2->at(i)->at(j);

        for(unsigned long j = 0; j < 10; j++)
            (choice) ? probabilites->push_back((entree_1->at(i)->at(j) * entree_2->at(i)->at(j)) / sum)
                     : probabilites->push_back((entree_1->at(i)->at(j) + entree_2->at(i)->at(j)) / sum);

        std::vector<double>::iterator it = std::max_element(probabilites->begin(), probabilites->end());

        for(std::vector<double>::iterator it2 = probabilites->begin(); it2 != probabilites->end(); ++it2) {
            if(it != it2 && *it == *it2) {
                unknow++;
                break;
            }
        }

        if(unknow || (int)(it - probabilites->begin()) != (int)(i / 10)) err++;
    }

    (choice) ? std::cout << "Produit -> " << err << "% d'erreurs" << std::endl
             : std::cout << "Somme   -> " << err << "% d'erreurs" << std::endl;

    if(!choice) calcul(++choice);
}