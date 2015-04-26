#ifndef _IMAGECLASS_H
#define _IMAGECLASS_H

#include "imageCharacterization.h"

using namespace std;

class ImageClass {
    private:
        vector<ImageCharacterization> characterizations ;
    public:
        ImageClass(const vector<string> &filenames) ;
        void dump(void) ;
        double minDistance(const ImageCharacterization image) ;
        double maxDistance(const ImageCharacterization image) ;
        double meanDistance(const ImageCharacterization image) ;
        double medianDistance(const ImageCharacterization image) ;
};

#endif
