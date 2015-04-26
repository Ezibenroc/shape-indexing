#include "imageCharacterization.h"
#include "perimeterVSarea.h"

string infoFile(string filename) {
    int lastindex = filename.find_last_of(".");
    return filename.substr(0, lastindex) + ".info";
}

ImageCharacterization::ImageCharacterization(Image image) : filename(""), image(image), object(image.domain()), signatureVector() {
    SetFromImage<DigitalSet>::append<Image>(this->object, image, 0, 255) ;
}

ImageCharacterization::ImageCharacterization(string filename) : ImageCharacterization(PGMReader<Image>::importPGM(filename)) {
    this->filename = filename ;
    string infoFileName = infoFile(filename) ;
    std::ifstream infile(infoFileName);
    double value ;
    if(infile.good()) {
         while (infile >> value) {
             this->signatureVector.push_back(value) ;
         }
    }
    infile.close() ;
}

void ImageCharacterization::computeSignatureVector(void) {
    if(this->signatureVector.size() != 0)
        return ;
    this->signatureVector.push_back(perimeterVSarea(this->image.domain(), this->object)) ;
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
