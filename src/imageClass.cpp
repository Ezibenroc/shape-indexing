#include <cassert>
#include "imageClass.h"

ImageClass::ImageClass(const vector<string> &filenames) {
    assert(filenames.size() > 0) ;
    for(unsigned i = 0 ; i < filenames.size() ; i++) {
        this->characterizations.push_back(ImageCharacterization(filenames[i])) ;
    }
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
    assert(false) ;
    return 0 ;
}

double ImageClass::medianDistance(const ImageCharacterization image) {
    assert(false) ;
    return 0 ;
}
