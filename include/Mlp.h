#ifndef MLP_H
#define MLP_H


#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include "Neurone.h"


class Mlp
{
protected:
    std::vector<Neurone*> couche_entree;
    std::vector<Neurone*> couche_cachee;
    std::vector<Neurone*> couche_sortie;
//    std::vector< std::vector<int> > entrees = { {1, 0}, {0, 0}, {1, 1}, {0, 1} };   // Attendu -> 1,0,0,1
//    std::vector< std::vector<int> > exemples = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
//    std::vector< std::vector<int> > sorties = {{1, 0}, {0, 1}, {0, 1}, {1, 0}};
    std::vector< std::vector<double> >* examples = new std::vector< std::vector<double> >;
    std::vector< std::vector<double> >* tests = new std::vector< std::vector<double> >;

public:
    Mlp();
    ~Mlp();
    void set_data(std::vector< std::vector<double> >* examples_features, std::vector< std::vector<double> >* tests_features);
    void run();
    void apprentissage();
    void propagation(int choice = 0);
    void retro_propagation();
    void delta_rule(int choice = 0);
    void test();
};


#endif