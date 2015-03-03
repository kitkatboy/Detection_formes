#ifndef KPPV_H
#define KPPV_H


#include <cv.h>
#include <vector>
#include <algorithm>


class Kppv
{
protected:
    std::vector< std::vector< std::vector <double> >* > classes;

public:
    Kppv();
    ~Kppv();
    void run(std::vector< std::pair<int, int> >* pos_app, std::vector< std::pair<int, int> >* pos_test, cv::Mat& app, cv::Mat& test);
    std::vector<double> zoning(cv::Mat& img, std::pair<int,int> haut_g, std::pair<int,int> bas_d);
    int proba(std::vector<double> a_classer);
    double distance_euclidienne(std::vector<double> X, std::vector<double> Y);
};


#endif