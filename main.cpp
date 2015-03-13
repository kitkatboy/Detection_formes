#include <cv.h>
#include "Extractor.h"
#include "Distance.h"
#include "Kppv.h"
#include "Combine.h"
#include "Mlp.h"


int main(int argc, char *argv[])
{
    Mat* app = new Mat(imread(argv[1], 0));
    Mat* test = new Mat(imread(argv[2], 0));
    std::vector< std::vector<double> > *examples_features_1, *tests_features_1, *examples_features_2, *tests_features_2;


    // Extraction positions, profils, densites
    Extractor * extract = new Extractor();

    extract->set_data(app, "examples");
    extract->extraction(std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, new Mat());
    examples_features_1 = extract->get_profils();
    examples_features_2 = extract->get_densites();

//    for(unsigned int i = 0; i < examples_features_2->size(); i++) {
//        std::cout << i << " -> ";
//        for(unsigned int j = 0; j < examples_features_2->at(i).size(); j++) {
//            std::cout << examples_features_2->at(i).at(j) << "\t";
//        }
//        std::cout << std::endl;
//    }

    extract->set_data(test, "tests");
    extract->extraction(std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, new Mat());
    tests_features_1 = extract->get_profils();
    tests_features_2 = extract->get_densites();

//    for(unsigned int i = 0; i < tests_features_2->size(); i++) {
//        std::cout << i << " -> ";
//        for(unsigned int j = 0; j < tests_features_2->at(i).size(); j++) {
//            std::cout << tests_features_2->at(i).at(j) << "\t";
//        }
//        std::cout << std::endl;
//    }

    delete extract;


    // Classifieur par distance euclidienne minimum
//    Distance * extract_2 = new Distance();
//    extract_2->run(examples_features_1, tests_features_1);
//
//    delete extract_2;


    // Kppv
//    Kppv * extract_3 = new Kppv();
//    extract_3->run(examples_features_2, tests_features_2);
//
//    delete extract_3;


    // Combinaison de classifieurs
//    Combine * extract_4 = new Combine();
//    extract_4->run();
//
//    delete extract_4;


    // Perceptron multicouches + profils
    Mlp * extract_5 = new Mlp();
    extract_5->set_data(examples_features_1, tests_features_1);
    extract_5->run();
    extract_5->test();

    delete extract_5;


    // Perceptron multicouches + densites


    // Display source
//    namedWindow( "Source", WINDOW_NORMAL);
//    imshow("Source",app);
//    waitKey(0);

    delete app;
    delete test;
    delete examples_features_1;
    delete tests_features_1;
    delete examples_features_2;
    delete tests_features_2;

    return 0;
}