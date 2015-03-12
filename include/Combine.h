#ifndef COMBINE_H
#define COMBINE_H


#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>


class Combine
{
protected:
    std::vector< std::vector<double>* >* entree_1;
    std::vector< std::vector<double>* >* entree_2;

public:
    Combine();
    ~Combine();
    void run();
    std::vector< std::vector<double>* >* readFile(std::string file_path);
    void calcul(int choice = 0);
};


#endif