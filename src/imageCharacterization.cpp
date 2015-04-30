#include <cmath>
#include <cassert>
#include "imageCharacterization.h"
#include "moment.h"
#include "border.h"
#include "perimeterVSarea.h"
#include "convexity.h"
#include "filter.h"

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
        Image filtered = filterNoise(image);
        DigitalSet object(filtered.domain()) ;
        SetFromImage<DigitalSet>::append<Image>(object, filtered, 0, 255) ;
        this->computeSignatureVector(filtered, object) ;
    }
}

ImageCharacterization::ImageCharacterization(vector<double> &signature) {
    this->signatureVector = vector<double>(signature) ;
}

size_t ImageCharacterization::size(void) {
    return this->signatureVector.size() ;
}

void ImageCharacterization::computeSignatureVector(const Image &image, const DigitalSet object) {
    if(this->signatureVector.size() != 0)
        return ;
    vector<Point> border = extractBorder(image.domain(), object) ;
    this->signatureVector.push_back(perimeterVSarea(image.domain(), object, border)) ;
    this->signatureVector.push_back(convexity(image.domain(), object, border)) ;
    vector<double> imageMoments = moment(image) ;
    for(unsigned i = 0 ; i < imageMoments.size() ; i++) {
        this->signatureVector.push_back(imageMoments[i]) ;
    }
}

vector<double> ImageCharacterization::getSignatureVector(void) {
    return vector<double>(this->signatureVector) ;
}

double ImageCharacterization::distance(const ImageCharacterization &other, const vector<double> &normalization) {
    double dist = 0;
    if(this->signatureVector.size() != other.signatureVector.size() ||\
        this->signatureVector.size() != normalization.size()) {
        cout << this->filename << " " << other.filename << endl ;
        assert(false) ;
    }
    for(unsigned i = 0 ; i < this->signatureVector.size() ; i++) {
        dist += pow((this->signatureVector[i]-other.signatureVector[i])/normalization[i], 2) ;
    }
    return sqrt(dist) ;
}

void ImageCharacterization::collectDescriptors(vector<vector<double>> &descriptors) {
    assert(this->signatureVector.size() == descriptors.size()) ;
    for(unsigned i = 0 ; i < this->signatureVector.size() ; i++) {
        descriptors[i].push_back(this->signatureVector[i]) ;
    }
}

ostream& operator<< (ostream &out, ImageCharacterization &img) {
    out << img.filename ;
    for(unsigned i = 0 ; i < img.signatureVector.size() ; i++) {
        if(isfinite(img.signatureVector[i]))
            out << " " << img.signatureVector[i];
        else
            out << " 1e30" ;    }
    out << "\n" ;
    return out ;
}
