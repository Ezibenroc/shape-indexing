#include <cassert>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>
#include "classifier.h"

// Take /path/to/file-3.pgm and returns file.
string fileToClass(std::string filename) {
    int begin = filename.find_last_of("/");
    string tmp = filename.substr(begin+1, filename.size()-begin) ;
    int end = tmp.find_first_of("-") ;
    return tmp.substr(0, end) ;
}

// Fill the maps with the CSV file.
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

// Read the descriptors.
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

// Verify the integrity of the descriptors.
void Classifier::checkSignatures(void) {
    for(unsigned i = 1 ; i < this->classes.size() ; i++) {
        if(this->classes[0].signatureSize() != this->classes[i].signatureSize()) {
            cerr << "Error: wrong number of descriptors in signature." << endl ;
            exit(EXIT_FAILURE) ;
        }
    }
}

// Initialize the classifier with the CSV and the descriptors files.
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

// Compute the normalization vector.
vector<double> Classifier::getNormalization(void) {
    vector<vector<double>> descriptors(this->numberDescriptors()) ;
    for(unsigned i = 0 ; i < this->classes.size() ; i++) {
        this->classes[i].collectDescriptors(descriptors) ;
    }
    vector<double> normalization(this->numberDescriptors()) ;
    for(unsigned i = 0 ; i < normalization.size() ; i++) {
        normalization[i] = measures(descriptors[i])[1] ; // 0=min, 1=max, 2=mean, 3=median
    }
    return normalization ;
}

// x < epsilon is interpreted as x=0
const double epsilon = 1e-6 ;

// Print the classification of the given image.
// For each class (in the order of the CSV file), print the probability
// that the image belongs to it.
void Classifier::classify(ImageClass image) {
    vector<double> normalization = this->getNormalization() ;
    vector<double> score(this->classes.size(), 0) ;
    int imin = -1 ;
    for(unsigned i = 0 ; i < this->classes.size() ; i++) {
        vector<double> d = this->classes[i].distances(image, normalization) ;
        if(d[0] < epsilon) { // found null min distance
            imin = i ;
            break ;
        }
        score[i] = 1-d[3] ; // mean distance
    }
    if(imin != -1) { // found null min distance
        for(unsigned i = 0 ; i < this->classes.size() ; i++) {
            if((int)i == imin)
                cout << "1" << "\n" ;
            else
                cout << "0" << "\n" ;
        }
    }
    else {
        double sum = 0 ;
        for(unsigned i = 0 ; i < this->classes.size() ; i++)
            sum += score[i] ;
        for(unsigned i = 0 ; i < this->classes.size() ; i++)
            cout << score[i]/sum << "\n" ;
    }
}
