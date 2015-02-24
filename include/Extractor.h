#ifndef EXTRACTOR_H
#define EXTRACTOR_H


#include <cv.h>
#include <highgui.h>

using namespace cv;

class Extractor
{
protected:
    Mat source;
    int tot_lines = 0;
    int tot_columns = 0;
    std::vector< std::pair<int, int> > positions;
    std::vector<std::vector<float>> moy_class;

public:
    Extractor(Mat& img);
    ~Extractor();
    void run();
    void get_positions();
    void show_element(int line, int column);
    void show_histo(Mat& img, int choice);
    std::vector<float> profil(std::pair<int,int> haut,std::pair<int,int> bas);
    std::vector<float> moyenne(std::vector<std::vector<float>> entree);
};


#endif