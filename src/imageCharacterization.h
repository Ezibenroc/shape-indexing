#ifndef _IMAGECHARACTERIZATION_H
#define _IMAGECHARACTERIZATION_H


#include <iostream>
#include <vector>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i;

typedef ImageContainerBySTLVector< Domain, unsigned int > Image;

class ImageCharacterization {
    private:
        string filename ;
        vector<double> signatureVector ;
    public:
        ImageCharacterization(string filename, bool dumpVector=true) ;
        size_t size(void) ;
        void computeSignatureVector(const Image &image, const DigitalSet object) ;
        vector<double> getSignatureVector(void) ;
        void dump(void) ;
        double distance(const ImageCharacterization &other) ;
};

#endif
