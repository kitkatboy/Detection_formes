#include <cv.h>
#include "Extractor.h"


int main(int argc, char *argv[])
{
    Mat img = imread(argv[1], 0);

    Extractor * extract = new Extractor(img);
    extract->run();

    // Display source
//    namedWindow( "Source", WINDOW_NORMAL);
//    imshow("Source",img);
//    waitKey(0);

    return 0;
}