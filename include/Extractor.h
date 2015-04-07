#ifndef EXTRACTOR_H
#define EXTRACTOR_H


#include <cv.h>
#include <highgui.h>
#include <fstream>


using namespace cv;

class Extractor
{
protected:
    Mat *img;
    std::string filename;
    int n;
    int m;
    std::vector< std::pair<int,int> > to_write;
    std::vector< std::vector<double> > *features_1 = new std::vector< std::vector<double> >;
    std::vector< std::vector<double> > *features_2 = new std::vector< std::vector<double> >;

public:
    Extractor();
    ~Extractor();
    void set_data(Mat* data, std::string name);
    void extraction(std::pair<int,int> haut_g, std::pair<int,int> bas_d, int choice, Mat* tmp);
    void profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit);
    void set_zoning(int horizontal, int vertical);
    void zoning(std::pair<int,int> haut_g, std::pair<int,int> bas_d);
    std::vector< std::vector<double> >* get_profils();
    std::vector< std::vector<double> >* get_densites();
    void writeFile();
    void show_element(int line, int column);
    void show_histo(int choice);
};


#endif