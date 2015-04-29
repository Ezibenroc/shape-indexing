#include "common.h"

/* Add a rectangle to the set. */
void addRectangle(DigitalSet &object, Point lowerBound, Point upperBound) {
    for(int i = lowerBound[0] ; i < upperBound[0] ; i++) {
        for(int j = lowerBound[1] ; j < upperBound[1] ; j++) {
            object.insert(Point(i, j)) ;
        }
    }
}
