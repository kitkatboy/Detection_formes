#include "kppv.h"


Kppv::Kppv() {
}


Kppv::~Kppv() {
}


void Kppv::run(Mat& app, std::vector< std::pair<int,int> >* pos_app) {

    std::vector< std::vector<double> > densities;

    for(unsigned long i = 0; i < pos_app->size(); i+=2) {
        densities.push_back(zoning(app, pos_app->at(i), pos_app->at(i + 1)));
    }
}


std::vector<double> Kppv::zoning(Mat& img, std::pair<int,int> haut_g, std::pair<int,int> bas_d) {

    Mat tmp;
    int n = 5;  // vertical zoning
    int m = 5;  // horizontal zoning
    int density;
    double density_normalize;
    std::vector<double> results;

//    tmp = img.rowRange(haut_g.second, bas_d.second);
//    tmp = tmp.colRange(haut_g.first, bas_d.first);
//
//    namedWindow( "Element", WINDOW_NORMAL);
//    imshow("Element",tmp);
//    waitKey(0);

    int correction = 3; // Facteur de correction du rectangle englobant
    std::pair<int,int> x, y;

    x.first = haut_g.first - correction;
    x.second = bas_d.first + correction;
    y.first = haut_g.second - correction;
    y.second = bas_d.second + correction;

    int x_step = (x.second - x.first) / m;
    int y_step = (y.second - y.first) / n;

    for(int i = y.first; i < (y.first + n * y_step); i += y_step) {
        for(int j = x.first; j < (x.first + m * x_step); j += x_step) {

            density = 0;

            tmp = img.rowRange(i, i + y_step);
            tmp = tmp.colRange(j, j + x_step);

            for(int k = 0; k < tmp.cols; k++)
                density += tmp.rows - countNonZero(tmp.col(k));

            density_normalize = density / (double)(tmp.rows * tmp.cols);
            results.push_back(density_normalize);
        }
    }

    return results;
}