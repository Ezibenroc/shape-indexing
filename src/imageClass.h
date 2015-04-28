#ifndef _IMAGECLASS_H
#define _IMAGECLASS_H

#include "imageCharacterization.h"

using namespace std;

class ImageClass {
    private:
        vector<ImageCharacterization> characterizations ;
        void addImages(string dirname) ;
    public:
        ImageClass(string dirname) ;
        size_t size(void) ;
        void dump(void) ;
        // Return [min, max, mean, median]
        vector<double> distances(void) ;
        // Return [min, max, mean, median]
        vector<double> distances(const ImageClass &other) ;
};

#endif
