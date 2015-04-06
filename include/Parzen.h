#ifndef PARZEN_H
#define PARZEN_H


#include "iostream"
#include <vector>
#include <math.h>
#include <algorithm>


class Parzen
{
protected:
    std::vector<double> *examples_densities = new std::vector<double>;

public:
    Parzen();
    ~Parzen();
    void run(std::vector <std::vector<double> > *examples, std::vector <std::vector<double> > *tests);
    double density(std::vector<double> vec);
    int kppv(double a_classer);
};


#endif // PARZEN_H