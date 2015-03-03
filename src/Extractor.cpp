#include "Extractor.h"


Extractor::Extractor() {
}


Extractor::~Extractor() {
}


void Extractor::get_positions(Mat& img, std::vector< std::pair<int,int> >* vec, std::pair<int,int> haut_g, std::pair<int,int> bas_d, int choice, bool write) {

    Mat tmp;
    int data;
    bool in = false;

    int sz = (choice == 0 || choice == 2) ? img.rows : img.cols;

    for (int i = 0; i < sz; i++) {
        data = (choice == 0 || choice == 2) ? img.cols - countNonZero(img.row(i)) : img.rows - countNonZero(img.col(i));

        if (data != 0 && !in) {
            if(choice == 0) {
                haut_g.second = i - 2;
            } else if(choice == 1) {
                haut_g.first = i - 2;
            } else {
                vec->push_back(std::pair<int, int>(haut_g.first, haut_g.second + i - 2));
            }
            in = true;
        } else if (data == 0 && in) {
            if(choice == 0) {
                bas_d.second = i + 2;
                get_positions(tmp = img.rowRange(haut_g.second, bas_d.second), vec, haut_g, bas_d, choice + 1, false);
            } else if(choice == 1) {
                bas_d.first = i + 2;
                get_positions(tmp = img.colRange(haut_g.first, bas_d.first), vec, haut_g, bas_d, choice + 1, false);
            } else {
                vec->push_back(std::pair<int, int>(bas_d.first, bas_d.second - (img.rows - i) + 2));
            }
            in = false;
        }
    }

    if(choice == 0)
        (write) ? writeFile(vec, "app") : writeFile(vec, "test");
}


void Extractor::writeFile(std::vector< std::pair<int,int> >* vec, std::string name) {

    std::string output = "data/" + name + ".positions";

    std::ofstream outputFile;
    outputFile.open(output.c_str());
    if (outputFile.is_open()) {
        for(unsigned long i = 0; i < vec->size() - 1; i+=2)
            outputFile << vec->at(i).first << "\t" << vec->at(i).second << "\t" << vec->at(i+1).first << "\t" << vec->at(i+1).second << std::endl;

        outputFile.close();
    } else {
        std::cout << "File " << output << " not found" << std::endl;
    }
}


void Extractor::show_element(Mat& img, std::vector< std::pair<int,int> >* pos, int line, int column) {

    Mat tmp = img.rowRange(pos->at((line * pos->size() / 10) + (column * 2)).second, pos->at((line * pos->size() / 10) + (column * 2) + 1).second);
    tmp = tmp.colRange(pos->at((line * pos->size() / 10) + (column * 2)).first, pos->at((line * pos->size() / 10) + (column * 2) + 1).first);

    namedWindow( "Element", WINDOW_NORMAL);
    imshow("Element",tmp);
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