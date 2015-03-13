#ifndef DISTANCE_H
#define DISTANCE_H


#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>


class Distance
{
protected:
    std::vector< std::vector<double> > moy_class;
    std::vector< std::vector<double> > *vecteurs_probabilites = new std::vector< std::vector<double> >;

public:
    Distance();
    ~Distance();
    void run(std::vector< std::vector<double> >* examples_features_1, std::vector< std::vector<double> >* tests_features_1);
    std::vector<double> moyenne(std::vector<std::vector<double>> *entree);
    double distance_euclidienne(std::vector<double> X, std::vector<double> Y);
    unsigned int proba(std::vector<double> a_classer);
    void writeFile();
};


#endif