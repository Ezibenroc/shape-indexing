#include <cassert>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include "imageClass.h"

ImageClass::ImageClass(string dirname) {
    this->addImages(dirname) ;
    assert(this->size()) ;
}

void ImageClass::addImages(string dirname) {
    struct stat st;
    lstat((char*)dirname.c_str(), &st);
    if(S_ISDIR(st.st_mode)) {
        struct dirent *entry; // sorry for dirent, boost/filesystem was not available on servsls@ens-lyon.fr
        DIR *dp;
        dp = opendir((char*)dirname.c_str());
        while((entry = readdir(dp))) {
            if(!((strcmp(".",entry->d_name)==0) || (strcmp("..",entry->d_name)==0))) {
                this->addImages((dirname)+"/"+((string)entry->d_name)) ;
            }
        }
        closedir(dp);
    }
    else {
        unsigned dotPos = dirname.find_last_of(".") + 1 ;
        unsigned slashPos = dirname.find_last_of("/") + 1 ;
        if(dotPos != dirname.npos && dotPos > slashPos && dirname.substr(dotPos) == "pgm")
            this->characterizations.push_back(ImageCharacterization(dirname)) ;
    }
}

size_t ImageClass::size(void) {
    return this->characterizations.size() ;
}

void ImageClass::dump(void) {
    for(unsigned i = 0 ; i < this->characterizations.size() ; i++) {
        this->characterizations[i].dump() ;
    }
}

double ImageClass::minDistance(const ImageCharacterization image) {
    double d = this->characterizations[0].distance(image) ;
    for(unsigned i = 1 ; i < this->characterizations.size() ; i++) {
        d = min(d, this->characterizations[i].distance(image)) ;
    }
    return d ;
}

double ImageClass::maxDistance(const ImageCharacterization image) {
    double d = this->characterizations[0].distance(image) ;
    for(unsigned i = 1 ; i < this->characterizations.size() ; i++) {
        d = max(d, this->characterizations[i].distance(image)) ;
    }
    return d ;
}

double ImageClass::meanDistance(const ImageCharacterization image) {
    double d = this->characterizations[0].distance(image) ;
    for(unsigned i = 1 ; i < this->characterizations.size() ; i++) {
        d += this->characterizations[i].distance(image) ;
    }
    return d/((double)this->characterizations.size()) ;
}

double ImageClass::medianDistance(const ImageCharacterization image) {
    vector<double> distances ;
    for(unsigned i = 0 ; i < this->characterizations.size() ; i++) {
        distances.push_back(this->characterizations[i].distance(image)) ;
    }
    size_t n = distances.size() / 2;
    nth_element(distances.begin(), distances.begin()+n, distances.end());
    return distances[n];
}
