#include <cv.h>
#include "Extractor.h"


void get_Columns(Mat& img) {

    int data, column = 0;
    bool in_column = false;
    Mat tmp_array = Mat::zeros(1, img.cols, CV_8U);

    for(int i = 0; i < img.cols; i++) {
        data = img.rows - countNonZero(img.col(i));
//        tmp_array.at<unsigned char>(i) = data;
    }


//    Mat histo = Mat::zeros(Size(img.cols,img.rows), CV_8U);
//
//    for(int i = 0; i< img.cols; i++) {
//        for (int j = 0; j < tmp_array.at<unsigned char>(i); j++) {
//            histo.at<unsigned char>(img.rows - j - 1, i) = 255;
//        }
//    }
//
//    namedWindow( "histo vertical", WINDOW_NORMAL);
//    imshow("histo vertical",histo);
}


int main(int argc, char *argv[])
{
    Mat img = imread(argv[1], 0);

    Extractor * extract = new Extractor(img);
    extract->get_positions();

    extract->show_element(0, 0);
//    extract->show_histo(0);

    // Display source
//    namedWindow( "Source", WINDOW_NORMAL);
//    imshow("Source",img);
//    waitKey(0);

    return 0;
}