#include <cassert>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>
#include "classifier.h"

string fileToClass(std::string filename) {
    int begin = filename.find_last_of("/");
    string tmp = filename.substr(begin+1, filename.size()-begin) ;
    int end = tmp.find_first_of("-") ;
    return tmp.substr(0, end) ;
}

void Classifier::readCSV(string dirCSV) {
    int index = 0 ;
    ifstream  data(dirCSV);
    string line;
    while(getline(data,line)) {
        stringstream  lineStream(line);
        string cell;
        int oldindex = index ;
        while(getline(lineStream, cell,',')) {
            this->classNames.push_back(cell) ;
            this->classNameToIndex.insert(make_pair(cell, index)) ;
            index ++ ;
        }
        assert(index == oldindex + 1) ;
    }
}

void Classifier::readDescriptors(string dirDescriptors) {
    ifstream  data(dirDescriptors);
    string line;
    while(getline(data,line)) {
        stringstream  lineStream(line);
        string fileName;
        lineStream >> fileName ;
        double tmp ;
        vector<double> signature ;
        while(lineStream >> tmp) {
            signature.push_back(tmp) ;
        }
        string className = fileToClass(fileName) ;
        try {
            this->classes[this->classNameToIndex[className]].addSignature(signature) ;
        }
        catch(const  std::out_of_range & ex){
            cerr << "Error: class " << className << "(from file " << fileName << ") does not belong to the CSV file." << endl ;
            exit(EXIT_FAILURE) ;
        }
    }
}

void Classifier::checkSignatures(void) {
    for(unsigned i = 1 ; i < this->classes.size() ; i++) {
        if(this->classes[0].signatureSize() != this->classes[i].signatureSize()) {
            cerr << "Error: wrong number of descriptors in signature." << endl ;
            exit(EXIT_FAILURE) ;
        }
    }
}

Classifier::Classifier(string dirCSV, string dirDescriptors) {
    this->readCSV(dirCSV) ;
    this->classes = vector<ImageClass>(this->classNames.size()) ;
    this->readDescriptors(dirDescriptors) ;
    assert(this->numberClasses() && this->numberImages()) ;
    this->checkSignatures() ;
}

size_t Classifier::numberClasses(void) {
    return this->classes.size() ;
}

size_t Classifier::numberImages(void) {
    size_t n = 0 ;
    for(unsigned i = 0 ; i < this->classes.size() ; i++) {
        n += this->classes[i].size() ;
    }
    return n ;
}

size_t Classifier::numberDescriptors(void) {
    return this->classes[0].signatureSize() ;
}
