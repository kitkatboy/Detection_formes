#include "Distance.h"


Distance::Distance() {
}


Distance::~Distance() {
}


void Distance::run(std::vector< std::pair<int, int> >* positions_app, std::vector< std::pair<int, int> >* positions_test, cv::Mat& app, cv::Mat& test) {

    int err, err_g = 0, unknow, unknow_g = 0;
    unsigned int result;
    std::vector< std::vector<double> > *tmp;

    std::cout << "\nMethode Classifieur par profils et distances euclidiennes minimum..." << std::endl;

    for(int i = 0; i < 10; i++) {

        tmp = new std::vector< std::vector<double> >;

        for(int j = 0; j < 20; j++) {
            tmp->push_back(profil(positions_app->at((i * positions_app->size() / 10) + (j * 2)), positions_app->at((i * positions_app->size() / 10) + (j * 2)+1),app));
        }
        moy_class.push_back(moyenne(tmp));
    }

    for(int i = 0; i < 10; i++) {

        err = 0;
        unknow = 0;

        for (int j = 0; j < 10; j++) {

            result = proba(profil(positions_test->at((i * positions_test->size() / 10) + (j * 2)), positions_test->at((i * positions_test->size() / 10) + (j * 2)+1), test));

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


std::vector<double> Distance::moyenne(std::vector< std::vector<double> > *entree) {

    double cpt;
    std::vector<double> moy;

    for(unsigned int i = 0; i < entree->at(0).size(); i++) {

        cpt = 0;

        for(unsigned int j = 0; j < entree->size(); j++)
            cpt += entree->at(j).at(i);

        moy.push_back(cpt / entree->size()/*entree->at(0).size()*/);
    }

    return moy;
}


std::vector<double> Distance::profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit, cv::Mat& source) {

    int j;
    std::vector<double> result;

    int d = 9; //9
    int correction = -1; // Facteur de correction du rectangle englobant
    std::pair<int,int> x,y;

    x.first = haut_gauche.first - correction;
    x.second = bas_droit.first + correction;
    y.first = haut_gauche.second - correction;
    y.second = bas_droit.second + correction;

    for(int i = 1; i < d+1; i++) {
        for(j = x.first; j < x.second; j++) {
            if((int)source.at<unsigned char>(y.first + i * (((y.second - y.first) / (double)(d+2)) + 0.5), j) == 0) {
                break;
            }
        }
        result.push_back((double)(j - x.first) / (x.second - x.first));
    }

    for(int i = 1; i < d+1; i++) {
        for(j = x.second ; j > x.first ; j--) {
            if((int)source.at<unsigned char>(y.first + i * (((y.second - y.first) / (double)(d+2)) + 0.5), j) == 0) {
                break;

            }
        }
        result.push_back((double)(x.second - j) / (x.second - x.first));
    }

    return result;
}


unsigned int Distance::proba(std::vector<double> a_classer) {

    double sum = 0;
    double result = -1, doublon = 0;
    double tmp;
    unsigned int rclass = 10;
    std::vector<double> probabilites;

    for(unsigned int j = 0; j < moy_class.size(); j++)
        sum += exp(-distance_euclidienne(a_classer,moy_class.at(j)));

    for(unsigned int i = 0 ; i < moy_class.size(); i++) {
        tmp = exp(-distance_euclidienne(a_classer,moy_class.at(i))) / sum;
        probabilites.push_back(tmp);

        if(tmp > result) {
            result = tmp;
            rclass = i;
        } else if(tmp == result) {
            doublon = tmp;
        }

        if(result == doublon && i == moy_class.size() - 1) rclass = 10;
    }

    vecteurs_probabilites.push_back(probabilites);

    return rclass;
}


double Distance::distance_euclidienne(std::vector<double> X, std::vector<double> Y){
    double sum = 0;

    if(X.size() != Y.size()) std::cout << "Calcul de la distance euclidienne impossible." << std::endl;

    for(unsigned int i = 0; i < X.size(); i++)
        sum += (X.at(i) - Y.at(i))*(X.at(i) - Y.at(i));

    return sqrt(sum);
}


void Distance::writeFile() {

    std::string output = "data/entree_1.proba";

    std::ofstream outputFile;
    outputFile.open(output.c_str());
    if (outputFile.is_open()) {

        for(unsigned int i = 0; i < vecteurs_probabilites.size(); i++) {
            for(unsigned int j = 0; j < vecteurs_probabilites.at(i).size(); j++)
                outputFile << vecteurs_probabilites.at(i).at(j) << "\t";
            outputFile << std::endl;
        }

        outputFile.close();
    } else {
        std::cout << "File " << output << " not found" << std::endl;
    }
}