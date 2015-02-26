#ifndef DISTANCE_H
#define DISTANCE_H


#include <cv.h>
#include <vector>
//#include <math.h>


class Distance
{
protected:
    std::vector<std::vector<float>> moy_class;

public:
    Distance();
    ~Distance();
    void run();
    std::vector<float> profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit, cv::Mat& source);
    std::vector<float> moyenne(std::vector<std::vector<float>> entree);
    float distance_euclidienne(std::vector<float> X, std::vector<float> Y);
    int proba(std::vector<float> a_classer);
};


#endif