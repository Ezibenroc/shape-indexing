#include <cassert>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>
#include "imageClass.h"

ImageClass::ImageClass(string dirname) {
    this->addImages(dirname) ;
    assert(this->size()) ;
}

ImageClass::ImageClass(void) {}

void ImageClass::addSignature(vector<double> signature) {
    this->characterizations.push_back(ImageCharacterization(signature)) ;
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

// Pre-condition: v is sorted in increasing order.
double minElt(const vector<double> v) {
    return v[0] ;
}

// Pre-condition: v is sorted in increasing order.
double maxElt(const vector<double> v) {
    return v[v.size()-1] ;
}

double meanElt(const vector<double> v) {
    double d = 0 ;
    for(unsigned i = 0 ; i < v.size() ; i++) {
        d += v[i] ;
    }
    return d/((double)v.size()) ;
}

// Pre-condition: v is sorted in increasing order.
double medianElt(const vector<double> v) {
    if(v.size()%2 == 1) {
        return v[v.size()/2] ;
    }
    else {
        return (v[v.size()/2-1] + v[v.size()/2])/2 ;
    }
}

// Return [min, max, mean, median]
vector<double> measures(vector<double> &sample) {
    sort(begin(sample), end(sample)) ;
    return {minElt(sample), maxElt(sample), meanElt(sample), medianElt(sample)} ;
}

vector<double> ImageClass::distances(const vector<double> &normalization) {
    if(this->characterizations.size() == 1) {
        return vector<double>(this->characterizations[0].size(), 0) ;
    }
    vector<double> d ;
    for(unsigned i = 0 ; i < this->characterizations.size() ; i++) {
        for(unsigned j = 0 ; j < this->characterizations.size() ; j++) {
            if(i != j)
                d.push_back(this->characterizations[i].distance(this->characterizations[j], normalization)) ;
        }
    }
    return measures(d) ;
}

vector<double> ImageClass::distances(const ImageClass &other, const vector<double> &normalization) {
    vector<double> d ;
    for(unsigned i = 0 ; i < this->characterizations.size() ; i++) {
        for(unsigned j = 0 ; j < other.characterizations.size() ; j++) {
            d.push_back(this->characterizations[i].distance(other.characterizations[j], normalization)) ;
        }
    }
    return measures(d) ;
}

void ImageClass::collectDescriptors(vector<vector<double>> &descriptors) {
    for(unsigned i = 0 ; i < this->characterizations.size() ; i++) {
        this->characterizations[i].collectDescriptors(descriptors) ;
    }
}

unsigned ImageClass::nbDescriptors(void) {
    return this->characterizations[0].size() ;
}

ostream& operator<< (ostream &out, ImageClass &img) {
    for(unsigned i = 0 ; i < img.characterizations.size() ; i++)
        out << img.characterizations[i] ;
    return out;
}
