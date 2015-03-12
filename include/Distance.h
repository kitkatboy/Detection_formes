#ifndef DISTANCE_H
#define DISTANCE_H


#include <cv.h>
#include <vector>
#include <math.h>
#include <fstream>


class Distance
{
protected:
    std::vector<std::vector<double>> moy_class;
    std::vector<std::vector<double>> vecteurs_probabilites;

public:
    Distance();
    ~Distance();
    void run(std::vector< std::pair<int, int> >* positions_app, std::vector< std::pair<int, int> >* positions_test, cv::Mat& app, cv::Mat& test);
    std::vector<double> moyenne(std::vector<std::vector<double>> *entree);
    std::vector<double> profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit, cv::Mat& source);
    double distance_euclidienne(std::vector<double> X, std::vector<double> Y);
    unsigned int proba(std::vector<double> a_classer);
    void writeFile();
};


#endif