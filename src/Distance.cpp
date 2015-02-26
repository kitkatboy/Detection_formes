#include "Distance.h"


Distance::Distance() {
}


Distance::~Distance() {
}


void Distance::run() {

    std::vector< std::vector<float> > tmp;

    for(int i = 0; i < 1; i++) {

        tmp.resize(0);

        for(int j = 0; j < 20; j++) {
//            std::cout<<"ok2"<<std::endl;
//            tmp.push_back(profil(positions_app[i*40 + j*2], positions_app[i*40 + j*2 + 1],app));
//            std::cout<< tmp.at(j).size()<<std::endl;

//            show_element(i, j,app,positions_app);

            // TODO Moyenne de classe

        }
//        std::cout<<moyenne(tmp).size()<<std::endl;
//        for(int k =0;k<moyenne(tmp).size();k++){
//            std::cout<< moyenne(tmp).at(k)<<std::endl;
//        }
        moy_class.push_back(moyenne(tmp));
    }
//    std::cout<<"tralala"<<moy_class.size()<<std::endl;

    std::cout<<"ok"<<std::endl;
    for(int i = 5; i < 6; i++) {

        for (int j = 0; j < 20; j++) {
//            show_element(i, j,test,positions_test);
//            std::cout<<positions_test[5 * 40 + 0 * 2+1].second<<std::endl;
//            std::cout<<"i : "<<i<<" j : "<<j<<" classe : "<<proba(profil(positions_test[i * 40 + j * 2], positions_test[i * 40 + j * 2 + 1], test))<<std::endl;
//            std::cout<<"i : "<<i<<" j : "<<j<<" classe : "<<proba(profil(positions_app[i * 40 + j * 2], positions_app[i * 40 + j * 2 + 1], app))<<std::endl;
        }
    }
}


std::vector<float> Distance::moyenne(std::vector< std::vector<float> > entree) {
    float cpt;
    int j;
    std::vector<float> moy;

    for(int i = 0; i< entree.at(0).size();i++) {
        cpt = 0;

        for(j = 0; j < entree.size(); j++) {
            cpt+=entree.at(j).at(i);
//            std::cout<<entree.at(i).at(j)<<std::endl;
        }
//        std::cout<<entree.at(i).size()<<std::endl;

        moy.push_back(cpt/entree.at(0).size());
    }

    if(moy.size() != 10){
        std::cout<<"c'est la merde"<<std::endl;
    }

    return moy;
}


std::vector<float> Distance::profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit, cv::Mat& source) {
    int j;
    std::vector<float> result;

    std::cout<<"    "<<(bas_droit.first - haut_gauche.first)<<std::endl;

    for(int i = 0; i < 5; i += 1) {
        for(j = haut_gauche.first ; j < bas_droit.first; j++) {
            if((int) source.at<unsigned char>(haut_gauche.second+i*(((bas_droit.second - haut_gauche.second) / 5.0) + 0.5), j) == 0) {
                break;
            }
        }
        result.push_back((bas_droit.first - haut_gauche.first) / (float)j);
    }

    for(int i = 0; i < 5; i += 1) {
        for(j = bas_droit.first ; j > haut_gauche.first ; j--) {
            if((int)source.at<unsigned char>(haut_gauche.second + i*(((bas_droit.second - haut_gauche.second) / 5.0) + 0.5), j) == 0) {
                break;
            }
        }
        result.push_back((bas_droit.first - haut_gauche.first) / (float)j);
    }
//    if(result.size() != 10){
//        std::cout << "Go..." << std::endl;
//        std::cout << "deb : " << haut_gauche.second << std::endl;
//        std::cout << "fin : " << bas_droit.second << std::endl;
//        std::cout << "nb loop : " << (int)((bas_droit.second - haut_gauche.second) / 5.0) + 0.5  << std::endl;
//        std::cout << "nb loop : " << (int)(((bas_droit.second - haut_gauche.second) / 5.0) + 0.5)  << std::endl;
//        std::cout << "j : "<<j<<std::endl;
//    }

    return result;
}


int Distance::proba(std::vector<float> a_classer){
    float sum;
    float result = -1;
    float tmp;
    int rclass;

    for(int i =0 ; i < moy_class.size(); i++){
        sum = 0;
        std::cout<<a_classer.at(i)<<std::endl;

        for(int j = 0; j < moy_class.size(); j++){

//            std::cout<<moy_class.at(j).size()<<std::endl;
            sum += exp(-distance_euclidienne(a_classer,moy_class.at(j)));

//            std::cout<<"         4"<<std::endl;
        }

//        std::cout<<"         11"<<std::endl;
        tmp = exp(-distance_euclidienne(a_classer,moy_class.at(i)))/sum;

//        std::cout<<"         44"<<std::endl;
//        std::cout<< distance_euclidienne(a_classer,moy_class.at(i))<<std::endl;
        if(tmp > result) {
            result = tmp;
            rclass = i;
        }
    }
//    std::cout<<result<<std::endl;
    return rclass;
}


float Distance::distance_euclidienne(std::vector<float> X, std::vector<float> Y){
    float sum = 0;
//    std::cout<<"X : "<<X.at(0)<<" Y : "<<Y.at(0)<<std::endl;

    if(X.size() != Y.size()) {
        std::cout<<"calcul distance euclidienne impossible"<<std::endl;
//        exit -1;
    }

    for(int i = 0; i < X.size(); i++){
        sum += (X.at(i) - Y.at(i))*(X.at(i) - Y.at(i));
    }

//    std::cout <<sum<<std::endl;
    if(sum<0) std::cout <<"c'est la merde dans la distance euclidienne"<<std::endl;

    return sqrt(sum);
}