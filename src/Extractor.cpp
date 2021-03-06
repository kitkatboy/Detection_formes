#include "Extractor.h"


Extractor::Extractor() {
}


Extractor::~Extractor() {
}


void Extractor::set_data(Mat* data, std::string name) {
    img = data;
    filename = name;

    to_write.clear();
    features_1 = new std::vector< std::vector<double> >;
    features_2 = new std::vector< std::vector<double> >;
}


void Extractor::extraction(std::pair<int,int> haut_g, std::pair<int,int> bas_d, int choice, Mat* tmp) {

    int data;
    bool in = false;
    int edge = 1;   // marge en pixels sur chaque bord entre le chiffre et le rectangle englobant

    Mat* ref = (choice == 0) ? img : tmp;
    int sz = (choice == 0 || choice == 2) ? ref->rows : ref->cols;

    for (int i = 0; i < sz; i++) {
        data = (choice == 0 || choice == 2) ? ref->cols - countNonZero(ref->row(i)) : ref->rows - countNonZero(ref->col(i));

        if (data != 0 && !in) {
            if(choice == 0) {
                haut_g.second = i - edge;
            } else if(choice == 1) {
                haut_g.first = i - edge;
            } else {
                to_write.push_back(std::pair<int, int>(haut_g.first, haut_g.second + i - edge));
            }
            in = true;
        } else if (data == 0 && in) {
            if(choice == 0) {
                bas_d.second = i + edge;
                extraction(haut_g, bas_d, choice + 1, new Mat(ref->rowRange(haut_g.second, bas_d.second)));
            } else if(choice == 1) {
                bas_d.first = i + edge;
                extraction(haut_g, bas_d, choice + 1, new Mat(ref->colRange(haut_g.first, bas_d.first)));
            } else {
                to_write.push_back(std::pair<int, int>(bas_d.first, bas_d.second - (ref->rows - i) + edge));

                // Get features
                profil(to_write[to_write.size() - 2], to_write[to_write.size() - 1]);
                zoning(to_write[to_write.size() - 2], to_write[to_write.size() - 1]);
            }
            in = false;
        }
    }

    if(choice == 0) writeFile();
}


void Extractor::profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit) {

    int j;
    std::vector<double> result;

    int d = 11;
    int correction = -1; // Facteur de correction du rectangle englobant
    std::pair<int,int> x,y;

    x.first = haut_gauche.first - correction;
    x.second = bas_droit.first + correction;
    y.first = haut_gauche.second - correction;
    y.second = bas_droit.second + correction;

    for(int i = 1; i < d+1; i++) {
        for(j = x.first; j < x.second; j++) {
            if((int)img->at<unsigned char>(y.first + i * (((y.second - y.first) / (double)(d+2)) + 0.5), j) == 0) {
                break;
            }
        }
        result.push_back((double)(j - x.first) / (x.second - x.first));
    }

    for(int i = 1; i < d+1; i++) {
        for(j = x.second ; j > x.first ; j--) {
            if((int)img->at<unsigned char>(y.first + i * (((y.second - y.first) / (double)(d+2)) + 0.5), j) == 0) {
                break;

            }
        }
        result.push_back((double)(x.second - j) / (x.second - x.first));
    }

    features_1->push_back(result);
}


void Extractor::set_zoning(int horizontal, int vertical) {
    n = horizontal;
    m = vertical;
}


void Extractor::zoning(std::pair<int,int> haut_g, std::pair<int,int> bas_d) {

    cv::Mat tmp;
    int density;
    double density_normalized;
    std::vector<double> results;

    int correction = -1; // Facteur de correction du rectangle englobant
    std::pair<int,int> x, y;

    x.first = haut_g.first - correction;
    x.second = bas_d.first + correction;
    y.first = haut_g.second - correction;
    y.second = bas_d.second + correction;


//    cv::Mat tmp2;
//    tmp2 = img->rowRange(y.first, y.second);
//    tmp2 = tmp2.colRange(x.first, x.second);
//    namedWindow("Source", WINDOW_NORMAL);
//    imshow("Source",tmp2);
//    waitKey(0);


    int x_step = (x.second - x.first) / m;
    int y_step = (y.second - y.first) / n;

    int modulo_x = (x.second - x.first) % m;
    int modulo_y = (y.second - y.first) % n;


    for(int i = y.first; i < y.second; i += y_step) {

        int mod_x = modulo_x;

        for(int j = x.first; j < x.second; j += x_step) {

            density = 0;

            tmp = img->rowRange(i, (modulo_y) ? i + y_step + 1 : i + y_step);
            tmp = tmp.colRange(j, (mod_x) ? j + x_step + 1 : j + x_step);

//            namedWindow("Zoning", WINDOW_NORMAL);
//            imshow("Zoning",tmp);
//            waitKey(0);


            for(int k = 0; k < tmp.cols; k++)
                density += tmp.rows - countNonZero(tmp.col(k));

            density_normalized = density / (double)(tmp.rows * tmp.cols);

            results.push_back(density_normalized);


            if(mod_x) {
                j++;
                mod_x--;
            }
        }

        if(modulo_y) {
            i++;
            modulo_y--;
        }
    }

    features_2->push_back(results);
}


std::vector< std::vector<double> >* Extractor::get_profils() {
    return features_1;
}


std::vector< std::vector<double> >* Extractor::get_densites() {
    return features_2;
}


void Extractor::writeFile() {

    std::string output = "data/" + filename + ".positions";

    std::ofstream outputFile;
    outputFile.open(output.c_str());
    if (outputFile.is_open()) {
        for(unsigned long i = 0; i < to_write.size() - 1; i+=2)
            outputFile << to_write[i].first << "\t" << to_write[i].second << "\t" << to_write[i+1].first << "\t" << to_write[i+1].second << std::endl;

        outputFile.close();
    } else {
        std::cout << "File " << output << " not found" << std::endl;
    }
}


void Extractor::show_element(int line, int column) {

    Mat tmp = img->rowRange(to_write[(line * to_write.size() / 10) + (column * 2)].second, to_write[(line * to_write.size() / 10) + (column * 2) +1].second);
    tmp = tmp.colRange(to_write[(line * to_write.size() / 10) + (column * 2)].first, to_write[(line * to_write.size() / 10) + (column * 2) +1].first);

    namedWindow( "Element", WINDOW_NORMAL);
    imshow("Element",tmp);
    waitKey(0);
}


/*
 * 0 -> horizontal histogram
 * 1 -> vertical histogram
 */
void Extractor::show_histo(int choice) {

    int sz, tmp;
    std::string name;
    Mat histo = Mat::zeros(Size(img->cols, img->rows), CV_8U);

    if(choice) {
        sz = img->rows;
        name = "vertical histogram";
    } else {
        sz = img->cols;
        name = "horizontal histogram";
    }

    Mat tmp_array = Mat::zeros(1, sz, CV_8U);

    // Count non zero data
    for(int i = 0; i < sz; i++) {
        tmp = (choice) ? img->cols - countNonZero(img->row(i)) : img->rows - countNonZero(img->col(i));
        tmp_array.at<unsigned char>(i) = tmp;
    }

    // Color histogram
    for(int i = 0; i < sz; i++) {
        for(int j = 0; j < tmp_array.at< unsigned char >(i); j++) {
            (choice) ? histo.at< unsigned char >(i,j) = 255 : histo.at<unsigned char>(img->rows - j - 1, i) = 255;
        }
    }

    // Display
    namedWindow(name, WINDOW_NORMAL);
    imshow(name,histo);
    waitKey(0);
}