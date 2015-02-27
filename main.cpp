#include <cv.h>
#include "Extractor.h"
#include "Distance.h"
#include "kppv.h"


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


    // Question 3
    Distance * extract_2 = new Distance();
    extract_2->run(positions_app, positions_test, app, test);

    delete extract_2;


    // Question 4
    Kppv * extract_3 = new Kppv();
    extract_3->run(positions_app, positions_test, app, test);

    delete extract_3;


    return 0;
}