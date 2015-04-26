#include "imageClass.h"

ImageClass::ImageClass(vector<string> filenames) {
    for(unsigned i = 0 ; i < filenames.size() ; i++) {
        this->characterizations.push_back(ImageCharacterization(filenames[i])) ;
    }
}
