#include <cmath>
#include "imageCharacterization.h"
#include "perimeterVSarea.h"

string infoFile(string filename) {
    int lastindex = filename.find_last_of(".");
    return filename.substr(0, lastindex) + ".info";
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
        DigitalSet object(image.domain()) ;
        SetFromImage<DigitalSet>::append<Image>(object, image, 0, 255) ;
        this->computeSignatureVector(image, object) ;
    }
}

void ImageCharacterization::computeSignatureVector(const Image &image, const DigitalSet object) {
    if(this->signatureVector.size() != 0)
        return ;
    this->signatureVector.push_back(perimeterVSarea(image.domain(), object)) ;
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
