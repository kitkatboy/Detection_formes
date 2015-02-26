#ifndef EXTRACTOR_H
#define EXTRACTOR_H


#include <cv.h>
#include <highgui.h>
#include <fstream>


using namespace cv;

class Extractor
{
protected:

public:
    Extractor();
    ~Extractor();
    void get_positions(Mat& img, std::vector< std::pair<int,int> >* vec, std::pair<int,int> haut_g, std::pair<int,int> bas_d, int choice, bool write);
    void writeFile(std::vector< std::pair<int,int> >* vec, std::string name);
    void show_element(Mat& img, std::vector< std::pair<int,int> >* pos, int line, int column);
    void show_histo(Mat& img, int choice);
};


#endif