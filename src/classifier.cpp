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

Classifier::Classifier(string dirCSV, string dirDescriptors) {
    this->readCSV(dirCSV) ;
    this->classes = vector<ImageClass>(this->classNames.size()) ;
    this->readDescriptors(dirDescriptors) ;
}

size_t Classifier::size(void) {
    return this->classes.size() ;
}
