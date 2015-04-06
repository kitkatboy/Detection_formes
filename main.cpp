#include <cv.h>
#include "Extractor.h"
#include "Distance.h"
#include "Parzen.h"
#include "Kppv.h"
#include "Combine.h"
#include "Mlp.h"


int main(int argc, char *argv[])
{
    Mat* app = new Mat(imread("data/app.tif", 0));
    Mat* test = new Mat(imread("data/test.tif", 0));
    std::vector< std::vector<double> > *examples_features_1, *tests_features_1, *examples_features_2, *tests_features_2;


    // Display source
//    namedWindow("Source", WINDOW_NORMAL);
//    imshow("Source",app);
//    waitKey(0);


    // Extraction positions, profils, densites
    Extractor * extract = new Extractor();

    extract->set_data(app, "examples");
    extract->extraction(std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, new Mat());
    examples_features_1 = extract->get_profils();
    examples_features_2 = extract->get_densites();

    extract->set_data(test, "tests");
    extract->extraction(std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, new Mat());
    tests_features_1 = extract->get_profils();
    tests_features_2 = extract->get_densites();

    delete extract;


    // Classifieur par distance euclidienne minimum
    Distance * extract_2 = new Distance();
    extract_2->run(examples_features_1, tests_features_1);

    delete extract_2;


    // Kppv
    Kppv * extract_3 = new Kppv();
    extract_3->run(examples_features_2, tests_features_2);

    delete extract_3;


    // Combinaison de classifieurs
    Combine * extract_4 = new Combine();
    extract_4->run();

    delete extract_4;


    // Parzen Windowing with kppv
    Parzen * extract_5 = new Parzen();
    extract_5->run(examples_features_2, tests_features_2);

    delete extract_5;


    // Perceptron multi-couches + densites
    Mlp *extract_6 = new Mlp();
    extract_6->run(examples_features_2, tests_features_2, 28, 1.2);   // Best result

    delete extract_6;


    delete app;
    delete test;
    delete examples_features_1;
    delete tests_features_1;
    delete examples_features_2;
    delete tests_features_2;

    return 0;
}