#include "Distance.h"

Distance::Distance() {
}


Distance::~Distance() {
}


void Distance::run(std::vector< std::pair<int, int> >* positions_app,std::vector< std::pair<int, int> >* positions_test,cv::Mat app,cv::Mat test) {

    std::vector< std::vector<double> > *tmp;

    for(int i = 0; i < 10; i++) {

        tmp = new std::vector< std::vector<double> >;

        for(int j = 0; j < 20; j++) {
            tmp->push_back(profil(positions_app->at((i * positions_app->size() / 10) + (j * 2)), positions_app->at((i * positions_app->size() / 10) + (j * 2)+1),app));
        }
        moy_class.push_back(moyenne(tmp));
    }

    int cpt;
    int cptg = 0;

    for(int i = 0; i < 10; i++) {

        cpt=0;

        for (int j = 0; j < 10; j++) {
            if(i!=proba(profil(positions_test->at((i * positions_test->size() / 10) + (j * 2)), positions_test->at((i * positions_test->size() / 10) + (j * 2)+1), test))) {
                cpt++;
            }
        }
        cptg += cpt;
        std::cout<<"il y a : "<<cpt<<" erreurs pour les "<<i<<std::endl;
    }
    std::cout<<"il y a : "<<cptg<<"% erreurs au total"<<std::endl;
}


std::vector<double> Distance::moyenne(std::vector< std::vector<double> > *entree) {
    double cpt;
    int j;
    std::vector<double> moy;

    for(int i = 0; i< entree->at(0).size();i++) {
        cpt = 0;

        for(j = 0; j < entree->size(); j++) {
            cpt+=entree->at(j).at(i);
        }
        moy.push_back(cpt/entree->at(0).size());
    }

    return moy;
}


std::vector<double> Distance::profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit, cv::Mat& source) {
    int j;
    std::vector<double> result;
    int d = 9;

    for(int i = 1; i < d+1; i += 1) {
        for(j = haut_gauche.first; j < bas_droit.first; j++) {
            if((int) source.at<unsigned char>(haut_gauche.second+i*(((bas_droit.second - haut_gauche.second) / (double)(d+2)) + 0.5), j) == 0) {
                break;
            }
        }
        result.push_back((double)(j - haut_gauche.first)/(bas_droit.first - haut_gauche.first));
    }
    for(int i = 1; i < d+1; i += 1) {
        for(j = bas_droit.first ; j > haut_gauche.first ; j--) {
            if((int)source.at<unsigned char>(haut_gauche.second + i*(((bas_droit.second - haut_gauche.second) / (double)(d+2)) + 0.5), j) == 0) {
                break;
            }
        }
        result.push_back( (double)(bas_droit.first-j)/(bas_droit.first - haut_gauche.first));
    }
    return result;
}


int Distance::proba(std::vector<double> a_classer){
    double sum;
    double result = -1;
    double tmp;
    int rclass;

    for(int i =0 ; i < moy_class.size(); i++){
        sum = 0;
        for(int j = 0; j < moy_class.size(); j++){
            sum += exp(-distance_euclidienne(a_classer,moy_class.at(j)));
        }
        tmp = exp(-distance_euclidienne(a_classer,moy_class.at(i)))/sum;
        if(tmp > result) {
            result = tmp;
            rclass = i;
        }
    }
    return rclass;
}


double Distance::distance_euclidienne(std::vector<double> X, std::vector<double> Y){
    double sum = 0;
    if(X.size() != Y.size()) {
        std::cout<<"calcul distance euclidienne impossible"<<std::endl;
    }
    for(int i = 0; i < X.size(); i++){
        sum += (X.at(i) - Y.at(i))*(X.at(i) - Y.at(i));
    }
    return sqrt(sum);
}