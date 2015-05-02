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

// A class to manipulate the characterization of an image.
// A characterization is a vector of descriptors.
class ImageCharacterization {
    private:

        // File name of the image, if initialized with a filename.
        string filename ;

        // Signature of the image.
        vector<double> signatureVector ;

    public:

        // Initialization with a PGM file.
        ImageCharacterization(string filename) ;

        // Initialization with a signature.
        ImageCharacterization(vector<double> &signature) ;

        // Return the number of descriptors.
        size_t size(void) ;

        // Compute the signature vector.
        void computeSignatureVector(const Image &image, const DigitalSet object) ;

        // Return the signature vector.
        vector<double> getSignatureVector(void) ;

        // Print function (used to print in the database file).
        friend ostream& operator<< (ostream &out, ImageCharacterization &img);

        // Compute the normalized distance between the two images.
        double distance(const ImageCharacterization &other, const vector<double> &normalization) ;

        // Append all the descriptors in the given vector.
        void collectDescriptors(vector<vector<double>> &descriptors) ;
};

#endif
