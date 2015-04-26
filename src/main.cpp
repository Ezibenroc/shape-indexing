///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <cassert>
#include <thread>
#include <mutex>
#include <boost/filesystem.hpp>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "imageCharacterization.h"
#include "imageClass.h"
///////////////////////////////////////////////////////////////////////////////

namespace fs = boost::filesystem;
using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

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
        vector<string> filenames ;
        fs::path dir(argv[my_index]) ;
        fs::directory_iterator end ;
        for(fs::directory_iterator iter(dir) ; iter != end ; ++iter) {
            assert(fs::is_regular_file(iter->status())) ;
            filenames.push_back(iter->path().string());
        }
        ImageClass img(filenames) ;
        classes_mutex->lock() ;
        classes->push_back(img);
        img.dump() ;
        classes_mutex->unlock() ;
        filenames.clear() ;
    }
}

int main(int argc, char *argv[]) {
    if(argc < 4) {
        cerr << "Syntax:" << argv[0] << " <number of threads> <image filename> <classes repositories>" << endl ;
        return 1 ;
    }
    int nbThreads = min(atoi(argv[1]), argc-2) ;
    if(nbThreads <= 0) {
        cerr << "Syntax:" << argv[0] << " <number of threads> <image filename> <classes repositories>" << endl ;
        return 1 ;
    }    // Check validity of directories
    for(int i = 3 ; i < argc ; i++) {
        fs::path dir(argv[i]) ;
        if(!fs::exists(dir) || !fs::is_directory(dir)) {
            cerr << "Error: " << argv[i] << " is not a directory." << endl ;
            return 1 ;
        }
    }
    int index = 3 ;
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

    string filename1 = argv[1] ;
    ImageCharacterization img1(filename1) ;
    for(unsigned i = 0 ; i < classes.size() ; i++) {
        cout << "# " << argv[i+2] << endl ;
        cout << "distance min = " << classes[i].minDistance(img1) << endl ;
        cout << "distance max = " << classes[i].maxDistance(img1) << endl ;
        cout << "mean distance = " << classes[i].meanDistance(img1) << endl << endl ;
    }
    img1.dump() ;
    return 0;
}
