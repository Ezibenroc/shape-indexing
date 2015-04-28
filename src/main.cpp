///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <thread>
#include <mutex>
#include <dirent.h>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "imageCharacterization.h"
#include "imageClass.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

// Verify that the given directory contains at least one pgm file.
bool checkDir(string dirname) {
    bool flag = false ;
    struct stat st;
    lstat((char*)dirname.c_str(), &st);
    if(S_ISDIR(st.st_mode)) {
        struct dirent *entry; // sorry for dirent, boost/filesystem was not available on servsls@ens-lyon.fr
        DIR *dp;
        dp = opendir((char*)dirname.c_str());
        while((entry = readdir(dp)) && !flag) {
            if(!((strcmp(".",entry->d_name)==0) || (strcmp("..",entry->d_name)==0))) {
                flag = flag||checkDir((dirname)+"/"+((string)entry->d_name)) ;
            }
        }
        closedir(dp);
        return flag ;
    }
    else {
        unsigned dotPos = dirname.find_last_of(".") + 1 ;
        unsigned slashPos = dirname.find_last_of("/") + 1 ;
        return (dotPos != dirname.npos && dotPos > slashPos && dirname.substr(dotPos) == "pgm") ;
    }
}

void computeClasses(int argc, char *argv[], int *index, mutex *index_mutex, vector<ImageClass> *classes, mutex *classes_mutex) {
    bool flag = true ;
    int my_index ;
    while(flag) {
        index_mutex->lock() ;
        if(*index < argc) {
            my_index = *index ;
            *index = *index + 1 ;
            cout << argv[my_index] << endl ;
        }
        else {
            flag = false ;
        }
        index_mutex->unlock() ;
        if(!flag)
            break ;
        ImageClass img(argv[my_index]) ;
        classes_mutex->lock() ;
        classes->push_back(img);
        classes_mutex->unlock() ;
    }
}

void syntaxError(char *argv[]) {
    cerr << "Syntax:" << argv[0] << " [-j <number of threads>] <directories>" << endl ;
    exit(EXIT_FAILURE) ;
}

void printTitle(string str) {
    cout << "\033[1;32m" << str << "\033[0m\n";
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        syntaxError(argv) ;
    }
    int indexFiles = 1, nbThreads = 1 ;
    if((string)argv[1] == "-j") {
        if(argc < 4) {
            syntaxError(argv) ;
        }
        nbThreads = atoi(argv[2]) ;
        if(nbThreads <= 0) {
            syntaxError(argv) ;
        }
        indexFiles = 3 ;
    }
    for(int i = indexFiles ; i < argc ; i++) {
        if(!checkDir(argv[i])) {
            std::cerr << "Error: no pgm image in directory " << argv[i] << std::endl ;
            exit(EXIT_FAILURE) ;
        }
    }
    int index = indexFiles ;
    int nbFiles = argc-indexFiles ;
    mutex index_mutex ;
    vector<ImageClass> classes ;
    mutex classes_mutex ;
    vector<thread> threads ;
    for(int i = 0 ; i < nbThreads ; i++) {
        threads.push_back(thread(computeClasses, argc, argv, &index, &index_mutex, &classes, &classes_mutex)) ;
    }

    for(int i = 0 ; i < nbThreads ; i++) {
        threads[i].join() ;
    }

    if(nbFiles == 1) { // Info about some class
        printTitle("\n\nInformation about some class.");
        vector<double> measures = classes[0].distances() ;
        cout << "min distance    = " << measures[0] << endl ;
        cout << "max distance    = " << measures[1] << endl ;
        cout << "mean distance   = " << measures[2] << endl ;
        cout << "median distance = " << measures[3] << endl << endl ;
    }
    else if(nbFiles ==2){ // Distance between two classes
        printTitle("\n\nDistance between two classes.");
        vector<double> measures = classes[0].distances(classes[1]) ;
        cout << "min distance    = " << measures[0] << endl ;
        cout << "max distance    = " << measures[1] << endl ;
        cout << "mean distance   = " << measures[2]<< endl ;
        cout << "median distance = " << measures[3] << endl << endl ;
    }
    else { // Retrieval of the class to which belong classes[0]
        printTitle("\n\nRetrieval of a class.");
        double median ;
        double minMedian = 1e20;
        int minIndex = -1 ;
        for(int unsigned i = 1 ; i < classes.size() ; i++) {
            median = classes[0].distances(classes[i])[3] ;
            if(median < minMedian) {
                minMedian = median ;
                minIndex = i ;
            }
        }
        cout << argv[index] << " is likely to belong to " << argv[index+minIndex] << endl ;
    }
    return 0;
}
