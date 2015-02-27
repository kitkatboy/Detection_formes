#ifndef KPPV_H
#define KPPV_H


#include <vector>


#include <cv.h>
#include <highgui.h>
using namespace cv;


class Kppv
{
protected:

public:
    Kppv();
    ~Kppv();
    void run(Mat& app, std::vector< std::pair<int,int> >* pos_app);
    std::vector<double> zoning(Mat& img, std::pair<int,int> haut_g, std::pair<int,int> bas_d);
};


#endif