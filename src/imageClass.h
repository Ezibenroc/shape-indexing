#ifndef _IMAGECLASS_H
#define _IMAGECLASS_H

#include "imageCharacterization.h"

using namespace std;

vector<double> measures(vector<double> &sample) ;

// A class to manipulate a set of images representing a class..
class ImageClass {
    private:

        // All the images of the class.
        vector<ImageCharacterization> characterizations ;

        // Add the image(s) contained in the given directory.
        void addImages(string dirname) ;

    public:

        // Initialization with all the PGM files contained in the given directory.
        ImageClass(string dirname) ;

        // Initialization with no images.
        ImageClass(void) ;

        // Add the signature to the class.
        void addSignature(vector<double> signature) ;

        // Return the number of images.
        size_t size(void) ;

        // Return the number of descriptors.
        size_t signatureSize(void) ;

        // Print function (used to print in the database file).
        friend ostream& operator<< (ostream &out, ImageClass &img);

        // Return statistics about the distances between the images of the class.
        // Return [min, max, mean, median].
        vector<double> distances(const vector<double> &normalization) ;

        // Return statistics about the distances between the images of the two classes.
        // Return [min, max, mean, median].
        vector<double> distances(const ImageClass &other, const vector<double> &normalization) ;

        // Append all the descriptors in the given vector.
        void collectDescriptors(vector<vector<double>> &descriptors) ;
};

#endif
