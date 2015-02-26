#include <cv.h>
#include "Extractor.h"


int main(int argc, char *argv[])
{
    Mat app = imread(argv[1], 0), test = imread(argv[2], 0);


    // Question 2
    std::vector< std::pair<int, int> >* positions_app = new std::vector< std::pair<int, int> >();
    std::vector< std::pair<int, int> >* positions_test = new std::vector< std::pair<int, int> >();
    Extractor * extract = new Extractor();

    extract->get_positions(app, positions_app, std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, true);
    extract->get_positions(test, positions_test, std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, false);


    delete extract;


    return 0;
}