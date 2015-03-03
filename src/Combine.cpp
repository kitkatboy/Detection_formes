#include "Combine.h"


Combine::Combine() {
}


Combine::~Combine() {
}


void Combine::run() {

    std::cout << "\nMethode par combinaison de classifieurs..." << std::endl;

    // Get vectors from files
    entree_1 = readFile("data/entree_1.proba");
    entree_2 = readFile("data/entree_2.proba");


//    for(unsigned long i = 0; i < entree_2->size(); i++) {
//        for(unsigned long j = 0; j < entree_2->at(i)->size(); j++)
//            std::cout << entree_2->at(i)->at(j) << "\t";
//        std::cout << std::endl;
//    }

    somme();
    produit();
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


void Combine::somme() {

    int cpt = 0;
    double sum = 0;
    std::vector<double> *probabilites;

    for(unsigned long i = 0; i < 100; i++) {

        sum = 0;
        probabilites = new std::vector<double>;

        for(unsigned long j = 0; j < 10; j++)
            sum += entree_1->at(i)->at(j) + entree_2->at(i)->at(j);

        for(unsigned long j = 0; j < 10; j++)
            probabilites->push_back((entree_1->at(i)->at(j) + entree_2->at(i)->at(j)) / sum);

        auto it = std::max_element(probabilites->begin(), probabilites->end());
        if((int)(it - probabilites->begin()) != (int)(i / 10)) cpt++;

//        std::cout << i << " -> " << (int)(it - probabilites->begin()) << std::endl;

//        std::cout << "proba : " << (int)(it - probabilites->begin()) << "\tAtt : " << (int)(i / 10) << std::endl;

//        for(unsigned long j = 0; j < probabilites->size(); j++)
//            std::cout << probabilites->at(j) << "\t";
//        std::cout << std::endl;
    }
    std::cout << "Somme   -> " << cpt << "% d'erreurs" << std::endl;
}


void Combine::produit() {

    int cpt = 0;
    double sum = 0;
    std::vector<double> *probabilites;

    for(unsigned long i = 0; i < 100; i++) {

        sum = 0;
        probabilites = new std::vector<double>;

        for(unsigned long j = 0; j < 10; j++)
            sum += entree_1->at(i)->at(j) * entree_2->at(i)->at(j);

        for(unsigned long j = 0; j < 10; j++)
            probabilites->push_back((entree_1->at(i)->at(j) * entree_2->at(i)->at(j)) / sum);

        auto it = std::max_element(probabilites->begin(), probabilites->end());
        if((int)(it - probabilites->begin()) != (int)(i / 10)) cpt++;

//        for(unsigned long j = 0; j < 10; j++)
//            std::cout << probabilites->at(j) << "\t";
//        std::cout << std::endl;
    }
    std::cout << "Produit -> " << cpt << "% d'erreurs" << std::endl;
}