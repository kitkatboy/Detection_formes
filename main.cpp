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
    extract->set_zoning(8, 8);

    extract->set_data(app, "examples");
    extract->extraction(std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, new Mat());
    examples_features_1 = extract->get_profils();
    examples_features_2 = extract->get_densites();

    extract->set_data(test, "tests");
    extract->extraction(std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, new Mat());
    tests_features_1 = extract->get_profils();
    tests_features_2 = extract->get_densites();


    // Classifieur par distance euclidienne minimum
    Distance * classifieur_1 = new Distance();
    classifieur_1->run(examples_features_1, tests_features_1);

    delete classifieur_1;


    // Kppv
    Kppv * classifieur_2 = new Kppv();
    classifieur_2->run(examples_features_2, tests_features_2);

    delete classifieur_2;


    // Combinaison de classifieurs
    Combine * classifieur_3 = new Combine();
    classifieur_3->run();

    delete classifieur_3;


    // Parzen Windowing with kppv
    Parzen * classifieur_4 = new Parzen();
    classifieur_4->run(examples_features_2, tests_features_2);

    delete classifieur_4;



    extract->set_zoning(5, 5);
    extract->set_data(app, "examples");
    extract->extraction(std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, new Mat());
    examples_features_2 = extract->get_densites();

    extract->set_data(test, "tests");
    extract->extraction(std::pair<int,int>(0,0), std::pair<int,int>(0,0), 0, new Mat());
    tests_features_2 = extract->get_densites();


    // Perceptron multi-couches + densites
    Mlp *classifieur_5 = new Mlp();
    classifieur_5->run(examples_features_2, tests_features_2, 28, 1.2);   // Best result

    delete classifieur_5;


    delete extract;
    delete app;
    delete test;
    delete examples_features_1;
    delete tests_features_1;
    delete examples_features_2;
    delete tests_features_2;

    return 0;
}