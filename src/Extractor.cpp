#include "Extractor.h"


Extractor::Extractor(Mat& img,Mat& img2) {
    app = img;
    test = img2;
}


Extractor::~Extractor() {
}


void Extractor::run() {

    std::vector< std::vector<float> > tmp;
    std::vector< std::pair<int, int> > positions_app;

    std::vector< std::pair<int, int> > positions_test;

    positions_app = get_positions(app);

    positions_test = get_positions(test);

    std::cout<<positions_test[5 * 40 + 11 * 2+1].second<<std::endl;
//    namedWindow( "test", WINDOW_NORMAL);
//    imshow("test",test);
//    waitKey(0);
//    std::cout<<"ok1"<<std::endl;
    for(int i = 0; i < 1; i++) {

        tmp.resize(0);

        for(int j = 0; j < 20; j++) {
//            std::cout<<"ok2"<<std::endl;
            tmp.push_back(profil(positions_app[i*40 + j*2], positions_app[i*40 + j*2 + 1],app));
//            std::cout<< tmp.at(j).size()<<std::endl;

//            show_element(i, j,app,positions_app);

            // TODO Moyenne de classe

        }
//        std::cout<<moyenne(tmp).size()<<std::endl;
//        for(int k =0;k<moyenne(tmp).size();k++){
//            std::cout<< moyenne(tmp).at(k)<<std::endl;
//        }
        moy_class.push_back(moyenne(tmp));
    }
//    std::cout<<"tralala"<<moy_class.size()<<std::endl;

    std::cout<<"ok"<<std::endl;
    for(int i = 5; i < 6; i++) {

        for (int j = 0; j < 20; j++) {
//            show_element(i, j,test,positions_test);
//            std::cout<<positions_test[5 * 40 + 0 * 2+1].second<<std::endl;
            std::cout<<"i : "<<i<<" j : "<<j<<" classe : "<<proba(profil(positions_test[i * 40 + j * 2], positions_test[i * 40 + j * 2 + 1], test))<<std::endl;
//            std::cout<<"i : "<<i<<" j : "<<j<<" classe : "<<proba(profil(positions_app[i * 40 + j * 2], positions_app[i * 40 + j * 2 + 1], app))<<std::endl;
        }
    }

//    show_element(1, 0);

//    show_histo(source, 0);
}

std::vector<float> Extractor::moyenne(std::vector<std::vector<float>> entree) {
    float cpt;
    int j;
    std::vector<float> moy;
    for(int i = 0; i< entree.at(0).size();i++) {
        cpt = 0;
        for(j = 0; j < entree.size(); j++) {
            cpt+=entree.at(j).at(i);
//            std::cout<<entree.at(i).at(j)<<std::endl;
        }
//        std::cout<<entree.at(i).size()<<std::endl;

        moy.push_back(cpt/entree.at(0).size());
    }
    if(moy.size() != 10){
        std::cout<<"c'est la merde"<<std::endl;
    }
    return moy;
}


std::vector< std::pair<int, int> > Extractor::get_positions(Mat source) {

    int data, tmp_haut,tmp_bas;
    bool in_line = false, in_column = false, in_line_2 = false;
    std::vector< std::pair<int, int> > positions;
    std::pair<int, int> haut_gauche, bas_droit;

    // Iterate in source lines
    for(int i = 0; i < source.rows; i++) {
        data = source.cols - countNonZero(source.row(i));

        if(data != 0 && !in_line) {

            // Get Y haut gauche
            haut_gauche.second = i - 2;
                tmp_haut = i - 2;
            in_line = true;

        } else if (data == 0 && in_line) {

            // Get Y bas droit
            bas_droit.second = i + 2;

            tmp_bas = i +2;

            in_line = false;


            // Iterate in source columns
            tot_columns = 0;
            Mat tmp2 = Mat::zeros(source.cols, bas_droit.second - haut_gauche.second, CV_8U);
            tmp2  = source.rowRange(haut_gauche.second, bas_droit.second);


//            namedWindow( "Element", WINDOW_NORMAL);
//            imshow("Element",tmp2);
//            waitKey(0);
            for(int j = 0; j < tmp2.cols; j++) {
                data = tmp2.rows - countNonZero(tmp2.col(j));

                if(data != 0 && !in_column) {

                    // Get X and push haut gauche position
                    haut_gauche.first = j - 2;
                    //positions.push_back(haut_gauche);

                    in_column = true;
                    tot_columns++;

                } else if (data == 0 && in_column) {

                    // Get X and push bas droit position
                    bas_droit.first = j + 2;
                    //positions.push_back(bas_droit);

                    in_column = false;
                    tot_columns++;

                    Mat tmp3 = Mat::zeros(bas_droit.first - haut_gauche.first,tmp2.rows, CV_8U);
                    tmp3  = tmp2.colRange(haut_gauche.first, bas_droit.first);

                    in_line_2 = false;

                    for(int k = 0; k < tmp3.rows; k++) {
                        data = tmp3.cols - countNonZero(tmp3.row(k));

                        if (data != 0 && !in_line_2) {

                            // Get Y haut gauche
//                            std::cout << haut_gauche.second <<" "<<k <<" "<<i<<std::endl;
                            haut_gauche.second =tmp_haut+ k - 2;
                            positions.push_back(haut_gauche);
                            in_line_2 = true;

                        } else if (data == 0 && in_line_2) {

                            // Get Y bas droit
                            bas_droit.second =  k + 2 +tmp_haut;
                            positions.push_back(bas_droit);
                            in_line_2 = false;
                        }

                    }
                    if(haut_gauche.second >= bas_droit.second) std::cout<<":("<<std::endl;
                    if(haut_gauche.first >= bas_droit.first) std::cout<<"   :("<<std::endl;
                }
            }
            tot_lines++;
        }
    }
    return positions;
}


void Extractor::show_element(int line, int column,Mat source,std::vector< std::pair<int, int> > positions) {

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


std::vector<float> Extractor::profil(std::pair<int,int> haut_gauche, std::pair<int,int> bas_droit,Mat source) {
    int j;
    std::vector<float> result;
    std::cout<<"       "<<(bas_droit.first - haut_gauche.first)<<std::endl;
    for(int i = 0; i < 5; i += 1) {
        for(j = haut_gauche.first ; j < bas_droit.first; j++) {
            if((int) source.at<unsigned char>(haut_gauche.second+i*(((bas_droit.second - haut_gauche.second) / 5.0) + 0.5), j) == 0) {
                break;
            }
        }
        result.push_back((bas_droit.first - haut_gauche.first) / (float)j);
    }

    for(int i = 0; i < 5; i += 1) {
        for(j = bas_droit.first ; j > haut_gauche.first ; j--) {
            if((int)source.at<unsigned char>(haut_gauche.second + i*(((bas_droit.second - haut_gauche.second) / 5.0) + 0.5), j) == 0) {
                break;
            }
        }
        result.push_back((bas_droit.first - haut_gauche.first) / (float)j);
    }
//    if(result.size() != 10){
//        std::cout << "Go..." << std::endl;
//        std::cout << "deb : " << haut_gauche.second << std::endl;
//        std::cout << "fin : " << bas_droit.second << std::endl;
//        std::cout << "nb loop : " << (int)((bas_droit.second - haut_gauche.second) / 5.0) + 0.5  << std::endl;
//        std::cout << "nb loop : " << (int)(((bas_droit.second - haut_gauche.second) / 5.0) + 0.5)  << std::endl;
//        std::cout << "j : "<<j<<std::endl;
//    }

    return result;
}

int Extractor::proba(std::vector<float> a_classer){
    float sum;
    float result = -1;
    float tmp;
    int rclass;

    for(int i =0 ; i < moy_class.size(); i++){
        sum = 0;
        std::cout<<a_classer.at(i)<<std::endl;
        for(int j = 0; j < moy_class.size(); j++){

//            std::cout<<moy_class.at(j).size()<<std::endl;
            sum += exp(-distance_euclidienne(a_classer,moy_class.at(j)));

//            std::cout<<"         4"<<std::endl;
        }

//        std::cout<<"         11"<<std::endl;
        tmp = exp(-distance_euclidienne(a_classer,moy_class.at(i)))/sum;

//        std::cout<<"         44"<<std::endl;
//        std::cout<< distance_euclidienne(a_classer,moy_class.at(i))<<std::endl;
        if(tmp > result) {
            result = tmp;
            rclass = i;
        }
    }
//    std::cout<<result<<std::endl;
    return rclass;

}

float Extractor::distance_euclidienne(std::vector<float> X, std::vector<float> Y){
    float sum = 0;
//    std::cout<<"X : "<<X.at(0)<<" Y : "<<Y.at(0)<<std::endl;

    if(X.size() != Y.size()) {
        std::cout<<"calcul distance euclidienne impossible"<<std::endl;
//        exit -1;
    }
    for(int i = 0; i < X.size(); i++){
        sum += (X.at(i) - Y.at(i))*(X.at(i) - Y.at(i));
    }
//    std::cout <<sum<<std::endl;
    if(sum<0) std::cout <<"c'est la merde dans la distance euclidienne"<<std::endl;
    return sqrt(sum);
}