#ifndef KPPV_H
#define KPPV_H


#include <cv.h>
#include <vector>
#include <algorithm>
#include <fstream>


class Kppv
{
protected:
    std::vector< std::vector< std::vector <double> >* > classes;
    std::vector< std::vector<double> > *vecteurs_probabilites = new std::vector< std::vector<double> >;

public:
    Kppv();
    ~Kppv();
    void run(std::vector< std::vector<double> >* examples_features_2, std::vector< std::vector<double> >* tests_features_2);
    int proba(std::vector<double> a_classer);
    double distance_euclidienne(std::vector<double> X, std::vector<double> Y);
    void writeFile();
};


#endif