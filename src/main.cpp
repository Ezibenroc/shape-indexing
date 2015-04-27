///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <thread>
#include <mutex>
#include <dirent.h>
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
        struct dirent *entry; // sorry for dirent, boost/filesystem was not available on servsls@ens-lyon.fr
        DIR *dp;
        dp = opendir(argv[my_index]);
        while((entry = readdir(dp))) {
            if(!((strcmp(".",entry->d_name)==0) || (strcmp("..",entry->d_name)==0))) {
                filenames.push_back(((string)argv[my_index])+"/"+((string)entry->d_name));
            }
        }
        closedir(dp);
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
        DIR *dp;
        dp = opendir(argv[i]);
        if (dp == NULL) {
            cerr << "Error with " << argv[i] << endl ;
            perror("opendir: Path does not exist or could not be read.");
            return 1;
        }
        closedir(dp);
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

    string filename1 = argv[2] ;
    ImageCharacterization img1(filename1) ;
    for(unsigned i = 0 ; i < classes.size() ; i++) {
        cout << "# " << argv[i+2] << endl ;
        cout << "distance min = " << classes[i].minDistance(img1) << endl ;
        cout << "distance max = " << classes[i].maxDistance(img1) << endl ;
        cout << "mean distance = " << classes[i].meanDistance(img1) << endl ;
        cout << "median distance = " << classes[i].medianDistance(img1) << endl << endl ;
    }
    img1.dump() ;
    return 0;
}
