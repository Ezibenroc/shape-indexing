#ifndef _IMAGECLASS_H
#define _IMAGECLASS_H

#include "imageCharacterization.h"

using namespace std;

vector<double> measures(vector<double> &sample) ;

class ImageClass {
    private:
        vector<ImageCharacterization> characterizations ;
        void addImages(string dirname) ;
    public:
        ImageClass(string dirname) ;
        ImageClass(void) ;
        void addSignature(vector<double> signature) ;
        size_t size(void) ;
        size_t signatureSize(void) ;
        friend ostream& operator<< (ostream &out, ImageClass &img);
        // Return [min, max, mean, median]
        vector<double> distances(const vector<double> &normalization) ;
        // Return [min, max, mean, median]
        vector<double> distances(const ImageClass &other, const vector<double> &normalization) ;
        void collectDescriptors(vector<vector<double>> &descriptors) ;
        unsigned nbDescriptors(void) ;
};

#endif
