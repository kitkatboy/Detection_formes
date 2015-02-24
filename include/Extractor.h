#ifndef EXTRACTOR_H
#define EXTRACTOR_H


#include <cv.h>
#include <highgui.h>
#include <math.h>

using namespace cv;

class Extractor
{
protected:
    Mat app;
    Mat test;
    int tot_lines = 0;
    int tot_columns = 0;
//    std::vector< std::pair<int, int> > positions;
    std::vector<std::vector<float>> moy_class;

public:
    Extractor(Mat& img,Mat& img2);
    ~Extractor();
    void run();
    std::vector< std::pair<int, int> > get_positions(Mat source);
    void show_element(int line, int column,Mat source,std::vector< std::pair<int, int> > positions);
    void show_histo(Mat& img, int choice);
    std::vector<float> profil(std::pair<int,int> haut,std::pair<int,int> bas,Mat source);
    std::vector<float> moyenne(std::vector<std::vector<float>> entree);
    float distance_euclidienne(std::vector<float> X, std::vector<float> Y);
    int proba(std::vector<float> a_classer);
};


#endif