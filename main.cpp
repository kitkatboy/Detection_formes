#include <cv.h>
#include "Extractor.h"


int main(int argc, char *argv[])
{
    Mat img = imread(argv[1], 0);

    Extractor * extract = new Extractor(img);
    extract->get_positions();

    extract->show_element(0, 0);
//    extract->show_histo(img, 0);

    // Display source
//    namedWindow( "Source", WINDOW_NORMAL);
//    imshow("Source",img);
//    waitKey(0);

    return 0;
}