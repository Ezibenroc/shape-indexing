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

    public:
        Classifier(string dirCSV, string dirDescriptors) ;
        size_t size(void) ;
};

#endif
