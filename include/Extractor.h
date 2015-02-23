#ifndef EXTRACTOR_H
#define EXTRACTOR_H


#include <cv.h>
#include <highgui.h>

using namespace cv;

class Extractor
{
protected:
    Mat source;
    std::vector<int> start_lines;
    std::vector<int> end_lines;
    std::vector< std::pair<int, int> > positions;

public:
    Extractor(Mat& img);
    ~Extractor();
    void get_positions();
    void show_element(int line, int column);
    void show_histo(Mat& img, int choice);
};


#endif