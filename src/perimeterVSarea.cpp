#include <cmath>
#include "border.h"
#include "perimeterVSarea.h"

double dist(Point p1, Point p2) {
    return sqrt(pow(p1[0]-p2[0], 2) + pow(p1[1]-p2[1], 2)) ;
}

// Return the perimeter of the object.
double getPerimeter(const Domain &domain, const DigitalSet &object, const vector<Point> &border) {
    double perimeter = dist(border[0], border[border.size()-1]);
    for(unsigned i = 0 ; i < border.size()-1 ; i++) {
        perimeter += dist(border[i], border[i+1]) ;
    }
    return perimeter ;
}

// Return the area of the object.
double getArea(const DigitalSet object) {
    return (double)object.size() ;
}

// Return the quotient of the area of the object by the area of the circle of same perimeter.
double perimeterVSarea(const Domain &domain, const DigitalSet &object, const vector<Point> &border) {
    double perimeter = getPerimeter(domain, object, border) ;
    double area = getArea(object) ;
    return area*4*M_PI/(perimeter*perimeter) ;
}
