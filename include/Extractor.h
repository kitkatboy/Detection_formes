#ifndef EXTRACTOR_H
#define EXTRACTOR_H


#include <cv.h>
#include <highgui.h>

using namespace cv;

class Extractor
{
protected:
    Mat source;
    std::vector< std::pair<int, int> > positions;

public:
    Extractor(Mat& img);
    ~Extractor();
    void run();
    void get_positions();
    void show_element(int line, int column);
    void show_histo(Mat& img, int choice);
    std::vector<float> profil(std::pair<int,int> haut,std::pair<int,int> bas);
};


#endif