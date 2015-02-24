#include "Extractor.h"


Extractor::Extractor(Mat& img) {
    source = img;
}


Extractor::~Extractor() {
}


void Extractor::run() {

    std::vector< std::vector<float> > tmp;

    get_positions();

    for(int i = 0; i < 10; i++) {

        tmp.resize(0);

        for(int j = 0; j < 20; j++) {
            tmp.push_back(profil(positions[i*40 + j*2], positions[i*40 + j*2 + 1]));

            // TODO Moyenne de classe

        }
    }

//    show_element(1, 0);

//    show_histo(source, 0);
}


void Extractor::get_positions() {

    int data;
    bool in_line = false, in_column = false;
    std::pair<int, int> haut_gauche, bas_droit;

    // Iterate in source lines
    for(int i = 0; i < source.rows; i++) {
        data = source.cols - countNonZero(source.row(i));

        if(data != 0 && !in_line) {

            // Get Y haut gauche
            haut_gauche.second = i - 2;

            in_line = true;

        } else if (data == 0 && in_line) {

            // Get Y bas droit
            bas_droit.second = i + 2;

            in_line = false;


            // Iterate in source columns
            tot_columns = 0;
            Mat tmp2 = Mat::zeros(source.cols, bas_droit.second - haut_gauche.second, CV_8U);
            tmp2  = source.rowRange(haut_gauche.second, bas_droit.second);

            for(int j = 0; j < tmp2.cols; j++) {
                data = tmp2.rows - countNonZero(tmp2.col(j));

                if(data != 0 && !in_column) {

                    // Get X and push haut gauche position
                    haut_gauche.first = j - 2;
                    positions.push_back(haut_gauche);

                    in_column = true;
                    tot_columns++;

                } else if (data == 0 && in_column) {

                    // Get X and push bas droit position
                    bas_droit.first = j + 2;
                    positions.push_back(bas_droit);

                    in_column = false;
                    tot_columns++;
                }
            }
            tot_lines++;
        }
    }
}


void Extractor::show_element(int line, int column) {

    int width = positions[(line * tot_columns) + (column * 2) + 1].first - positions[(line * tot_columns) + (column * 2)].first;
    int height = positions[(line * tot_columns) + (column * 2) + 1].second - positions[(line * tot_columns) + (column * 2)].second;

    // Selected Lines Matrix
    Mat tmp = Mat::zeros(width, height, CV_8U);
    tmp = source.rowRange(positions[(line * tot_columns) + (column * 2)].second, positions[(line * tot_columns) + (column * 2) + 1].second);

    // Selected Columns Matrix
    Mat tmp2 = tmp.colRange(positions[(line * tot_columns) + (column * 2)].first, positions[(line * tot_columns) + (column * 2) + 1].first);

    // Display
    namedWindow( "Element", WINDOW_NORMAL);
    imshow("Element",tmp2);
    waitKey(0);
}


/*
 * 0 -> horizontal histogram
 * 1 -> vertical histogram
 */
void Extractor::show_histo(Mat&img, int choice) {

    int sz, tmp;
    std::string name;
    Mat histo = Mat::zeros(Size(img.cols, img.rows), CV_8U);

    if(choice) {
        sz = img.rows;
        name = "vertical histogram";
    } else {
        sz = img.cols;
        name = "horizontal histogram";
    }

    Mat tmp_array = Mat::zeros(1, sz, CV_8U);

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


std::vector<float> Extractor::profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit) {
    int j;
    std::vector<float> result;

//    std::cout << "Go..." << std::endl;
//    std::cout << "deb : " << haut_gauche.second << std::endl;
//    std::cout << "fin : " << bas_droit.second << std::endl;
//    std::cout << "nb loop : " << ((bas_droit.second - haut_gauche.second) / 5.0) + 0.5  << std::endl;

    for(int i = haut_gauche.second; i < bas_droit.second; i += ((bas_droit.second - haut_gauche.second) / 5.0) + 0.5) {

//        std::cout << "loop : " << i << std::endl;

//        std::cout << "HG : " << haut_gauche.first << std::endl;
//        std::cout << "BD : " << bas_droit.first << std::endl;

        for(j = haut_gauche.first ; j < bas_droit.first; j++) {

//            std::cout << "Attention en dessous c de la merde" << std::endl;
            if((int) source.at<unsigned char>(i, j) == 0) {
//                std::cout << "Ce qu'on ne verra jamais" << std::endl;
                break;
            }
        }
        result.push_back((bas_droit.first - haut_gauche.first) / j);

//        std::cout << "Param 1" << std::endl;
    }

    for(int i = haut_gauche.second; i < bas_droit.second; i += ((bas_droit.second - haut_gauche.second) / 5.0) + 0.5) {
        for(j = bas_droit.first ; j > haut_gauche.first ; j--) {
            if((int)source.at<unsigned char>(i, j) == 0) {
                break;
            }
        }
        result.push_back((bas_droit.first - haut_gauche.first) / j);

    }
    return result;
}