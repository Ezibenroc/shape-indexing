#ifndef _CLASSIFIER_H
#define _CLASSIFIER_H

#include <vector>
#include <unordered_map>

#include "imageClass.h"

using namespace std;

// A class to classify an image with respect to a set of image classes.
class Classifier {
    private:

        // The classes of the database.
        vector<ImageClass> classes ;

        // Map index → name.
        vector<string> classNames ;

        // Map name → index.
        unordered_map<string, int> classNameToIndex ;

        // Fill the maps with the CSV file.
        void readCSV(string dirCSV) ;

        // Read the descriptors.
        void readDescriptors(string dirDescriptors) ;

        // Verify the integrity of the descriptors.
        void checkSignatures(void) ;

        // Compute the normalization vector.
        vector<double> getNormalization(void) ;

    public:

        // Initialize the classifier with the CSV and the descriptors files.
        Classifier(string dirCSV, string dirDescriptors) ;

        // Numbers of classes, images and descriptors.
        size_t numberClasses(void) ;
        size_t numberImages(void) ;
        size_t numberDescriptors(void) ;

        // Print the classification of the given image.
        // For each class (in the order of the CSV file), print the probability
        // that the image belongs to it.
        void classify(ImageClass image) ;
};

#endif
