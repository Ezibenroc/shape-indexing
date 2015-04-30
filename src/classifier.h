#ifndef _CLASSIFIER_H
#define _CLASSIFIER_H

#include <vector>
#include <unordered_map>

#include "imageClass.h"

using namespace std;

class Classifier {
    private:
        vector<ImageClass> classes ;
        vector<string> classNames ;
        unordered_map<string, int> classNameToIndex ;
        void readCSV(string dirCSV) ;
        void readDescriptors(string dirDescriptors) ;
        void checkSignatures(void) ;
        vector<double> getNormalization(void) ;

    public:
        Classifier(string dirCSV, string dirDescriptors) ;
        size_t numberClasses(void) ;
        size_t numberImages(void) ;
        size_t numberDescriptors(void) ;
        void classify(ImageClass image) ;
};

#endif
