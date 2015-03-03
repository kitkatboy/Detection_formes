#include "kppv.h"


Kppv::Kppv() {
}


Kppv::~Kppv() {
}


void Kppv::run(std::vector< std::pair<int, int> >* pos_app, std::vector< std::pair<int, int> >* pos_test, cv::Mat& app, cv::Mat& test) {

    int cpt;
    int cptg = 0;
    std::vector< std::vector<double> > *densities;

    std::cout << "\nMethode Classifieur par densites et k plus proches voisins..." << std::endl;

    // Apprentissage
    for(int i = 0; i < 10; i++) {   // Iteration lignes

        densities = new std::vector< std::vector<double> >;

        for(int j = 0; j < pos_app->size() / 10; j += 2)  // Iteration colonnes
            densities->push_back(zoning(app, pos_app->at(i * (pos_app->size() / 10) + j), pos_app->at(i * (pos_app->size() / 10) + j + 1)));

        classes.push_back(densities);
    }

    // Test
    for(int i = 0; i < 10; i++) {   // Iteration lignes

        cpt = 0;

        for(int j = 0; j < pos_test->size() / 10; j += 2) {  // Iteration colonnes
            if(i != proba(zoning(test, pos_test->at(i * (pos_test->size() / 10) + j), pos_test->at(i * (pos_test->size() / 10) + j + 1))))
                cpt++;
        }

        std::cout << "  -> " << cpt << " erreurs pour les " << i << std::endl;
        cptg += cpt;
    }
    std::cout << "-> " << cptg << "% d'erreurs" << std::endl;
    writeFile();
}


std::vector<double> Kppv::zoning(cv::Mat& img, std::pair<int,int> haut_g, std::pair<int,int> bas_d) {

    cv::Mat tmp;
    int n = 6;  // vertical zoning
    int m = 6;  // horizontal zoning
    int density;
    double density_normalize;
    std::vector<double> results;

    int correction = 3; // Facteur de correction du rectangle englobant
    std::pair<int,int> x, y;

    x.first = haut_g.first - correction;
    x.second = bas_d.first + correction;
    y.first = haut_g.second - correction;
    y.second = bas_d.second + correction;

    int x_step = (x.second - x.first) / m;
    int y_step = (y.second - y.first) / n;

    for(int i = y.first; i < (y.first + n * y_step); i += y_step) {
        for(int j = x.first; j < (x.first + m * x_step); j += x_step) {

            density = 0;

            tmp = img.rowRange(i, i + y_step);
            tmp = tmp.colRange(j, j + x_step);

            for(int k = 0; k < tmp.cols; k++)
                density += tmp.rows - countNonZero(tmp.col(k));

            density_normalize = density / (double)(tmp.rows * tmp.cols);

            results.push_back(density_normalize);
        }
    }

    return results;
}


int Kppv::proba(std::vector<double> a_classer) {

    int k = 6;  // nb de voisins //6
    std::vector< std::pair<double,unsigned long> > results;
    std::vector<double> probabilites(10, 0.0);

    for(unsigned long i = 0 ; i < classes.size(); i++) {  // Iteration parmis ttes les classes
        for (unsigned long j = 0; j < classes.at(0)->size(); j++)    // Iteration ds une classe
            results.push_back(std::pair<double, unsigned long>(distance_euclidienne(a_classer, classes.at(i)->at(j)), i));
    }
    std::sort(results.begin(), results.end());

    for(int i = 0; i < k; i++)
        probabilites[results[i].second]++;

    for(int i = 0; i < probabilites.size(); i++)
        if(probabilites[i] != 0) probabilites[i] /= k;

    to_write.push_back(probabilites);

    auto it = std::max_element(probabilites.begin(), probabilites.end());

    return (int)(it - probabilites.begin());
}


double Kppv::distance_euclidienne(std::vector<double> X, std::vector<double> Y) {
    double sum = 0;

    if(X.size() != Y.size()) std::cout << "calcul distance euclidienne impossible" << std::endl;

    for(unsigned long i = 0; i < X.size(); i++)
        sum += (X.at(i) - Y.at(i)) * (X.at(i) - Y.at(i));

    return sqrt(sum);   // Supp racine pr gain de perf
}


void Kppv::writeFile() {

    std::string output = "data/entree_2.proba";

    std::ofstream outputFile;
    outputFile.open(output.c_str());
    if (outputFile.is_open()) {

        for(unsigned long i = 0; i < to_write.size(); i++) {
            for(unsigned long j = 0; j < to_write.at(i).size(); j++)
                outputFile << to_write.at(i).at(j) << "\t";
            outputFile << std::endl;
        }

        outputFile.close();
    } else {
        std::cout << "File " << output << " not found" << std::endl;
    }
}