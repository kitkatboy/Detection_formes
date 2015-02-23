#include "Extractor.h"


Extractor::Extractor(Mat& img) {
    source = img;
    start_lines.resize(10);
    end_lines.resize(10);
}


Extractor::~Extractor() {
}


void Extractor::get_positions() {

    int data;
    bool in_line = false, in_column = false;

    std::pair<int, int> line_pos, haut_gauche, bas_droit;

    // Get lines positions
    for(int i = 0; i < source.rows; i++) {
        data = source.cols - countNonZero(source.row(i));

        if(data != 0 && !in_line) {
            line_pos.first = i - 2;
            in_line = true;
        } else if (data == 0 && in_line) {
            line_pos.second = i + 2;
//            positions.push_back(tmp);
            in_line = false;

            // Get columns positions
            Mat tmp2 = Mat::zeros(source.cols, line_pos.second - line_pos.first, CV_8U);
            tmp2  = source.rowRange(line_pos.first, line_pos.second);

//            namedWindow( "test lignes", WINDOW_NORMAL);
//            imshow("test lignes",tmp2);
//            waitKey(0);

//            show_histo(tmp2, 0);

            haut_gauche.second = line_pos.first;
            bas_droit.second = line_pos.second;

            for(int j = 0; j < tmp2.cols; j++) {
                data = tmp2.rows - countNonZero(tmp2.col(j));

                if(data != 0 && !in_column) {
                    haut_gauche.first = j - 2;
                    positions.push_back(haut_gauche);
                    in_column = true;
                } else if (data == 0 && in_column) {
                    bas_droit.first = j + 2;
                    positions.push_back(bas_droit);
                    in_column = false;
                }
            }
        }
    }
}


void Extractor::show_element(int line, int column) {

    // Init Matrix
    int width = positions[line*40 + column*2 + 1].first - positions[line*40 + column*2].first;
    int height = positions[line*40 + column*2 + 1].second - positions[line*40 + column*2].second;
    Mat tmp = Mat::zeros(width, height, CV_8U);

    // Set line Matrix
    tmp = source.rowRange(positions[line*40 + column*2].second, positions[line*40 + column*2 + 1].second);

    Mat tmp2 = tmp.colRange(positions[line*40 + column*2].first, positions[line*40 + column*2 + 1].first);

    namedWindow( "test lignes", WINDOW_NORMAL);
    imshow("test lignes",tmp2);
    waitKey(0);
}


/*
 * 0 -> horizontal histogram
 * 1 -> vertical histogram
 */
void Extractor::show_histo(Mat&img, int choice) {

    int sz, tmp;
    std::string name;
    Mat tmp_array;
    Mat histo = Mat::zeros(Size(img.cols, img.rows), CV_8U);

    if(choice) {
        sz = img.rows;
        name = "vertical histogram";
    } else {
        sz = img.cols;
        name = "horizontal histogram";
    }

    tmp_array = Mat::zeros(1, sz, CV_8U);

    // Count non zero data
    for(int i = 0; i < sz; i++) {
        tmp = (choice) ? img.cols - countNonZero(img.row(i)) : img.rows - countNonZero(img.col(i));
        tmp_array.at<unsigned char>(i) = tmp;
    }

    // Color histogram
    for(int i = 0; i < sz; i++) {
        for(int j = 0; j < tmp_array.at< unsigned char >(i); j++) {
            (choice) ? histo.at< unsigned char >(i,j) = 255 : histo.at<unsigned char>(img.rows - j - 1, i) = 255;
        }
    }

    // Display
    namedWindow(name, WINDOW_NORMAL);
    imshow(name,histo);
    waitKey(0);
}