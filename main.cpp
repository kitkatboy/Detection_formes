#include <cv.h>
#include "Extractor.h"


int main(int argc, char *argv[])
{
    Mat img = imread(argv[1], 0);
    Mat img2 = imread(argv[2], 0);

    Extractor * extract = new Extractor(img,img2);
    extract->run();

    // Display source
//    namedWindow( "Source", WINDOW_NORMAL);
//    imshow("Source",img);
//    waitKey(0);

    return 0;
}