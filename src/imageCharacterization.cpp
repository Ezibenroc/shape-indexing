#include <cmath>
#include "imageCharacterization.h"
#include "perimeterVSarea.h"
#include "convexity.h"

string infoFile(string filename) {
    int lastindex = filename.find_last_of(".");
    return filename.substr(0, lastindex) + ".info";
}

bool isOK(Image &image) {
    int nbBlackSidePixels = 0 ;
    Point lowerBound = image.domain().lowerBound(), upperBound = image.domain().upperBound() ;
    for(int i = lowerBound[0] ; i < upperBound[0] ; i++) {
        if(image(Point(i, lowerBound[1])))
            nbBlackSidePixels ++ ;
        if(image(Point(i, upperBound[1]-1)))
            nbBlackSidePixels ++ ;
    }
    for(int i = lowerBound[1] ; i < upperBound[1] ; i++) {
        if(image(Point(lowerBound[0], i)))
            nbBlackSidePixels ++ ;
        if(image(Point(upperBound[0]-1, i)))
            nbBlackSidePixels ++ ;
    }
    return (nbBlackSidePixels <= upperBound[0]-lowerBound[0] + upperBound[1]-lowerBound[1]) ;
}

void swap(Image &image) {
    for(Domain::ConstIterator it = image.domain().begin(); it != image.domain().end(); ++it) {
        image.setValue(*it, !image(*it)) ;
    }
}

ImageCharacterization::ImageCharacterization(string filename) {
    this->filename = filename ;
    string infoFileName = infoFile(filename) ;
    std::ifstream infile(infoFileName);
    double value ;
    if(infile.good()) {
         while (infile >> value) {
             this->signatureVector.push_back(value) ;
         }
         infile.close() ;
    }
    else {
        Image image(PGMReader<Image>::importPGM(filename)) ;
        if(!isOK(image))
            swap(image) ;
        DigitalSet object(image.domain()) ;
        SetFromImage<DigitalSet>::append<Image>(object, image, 0, 255) ;
        this->computeSignatureVector(image, object) ;
    }
}

void ImageCharacterization::computeSignatureVector(const Image &image, const DigitalSet object) {
    if(this->signatureVector.size() != 0)
        return ;
    this->signatureVector.push_back(perimeterVSarea(image.domain(), object)) ;
    this->signatureVector.push_back(convexity(image.domain(), object)) ;
}

vector<double> ImageCharacterization::getSignatureVector(void) {
    return vector<double>(this->signatureVector) ;
}

void ImageCharacterization::dump(void) {
    std::ofstream outfile(infoFile(this->filename));
    for(unsigned i = 0 ; i < this->signatureVector.size() ; i++) {
        outfile << this->signatureVector[i] << "\n" ;
    }
    outfile.close() ;
}

double ImageCharacterization::distance(const ImageCharacterization &other) {
    double dist = 0;
    for(unsigned i = 0 ; i < this->signatureVector.size() ; i++) {
        dist += (this->signatureVector[i]-other.signatureVector[i])*(this->signatureVector[i]-other.signatureVector[i]) ;
    }
    return sqrt(dist) ;
}
