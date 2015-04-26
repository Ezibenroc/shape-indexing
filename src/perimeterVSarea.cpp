#include <cmath>
#include "border.h"
#include "perimeterVSarea.h"

double dist(Point p1, Point p2) {
    return sqrt(pow(p1[0]-p2[0], 2) + pow(p1[1]-p2[1], 2)) ;
}

double getPerimeter(const Domain domain, const DigitalSet object) {
    vector<Point> border = extractBorder(domain, object) ;
    double perimeter = dist(border[0], border[border.size()-1]);
    for(unsigned i = 0 ; i < border.size()-1 ; i++) {
        perimeter += dist(border[i], border[i+1]) ;
    }
    return perimeter ;
}

double getArea(const DigitalSet object) {
    return (double)object.size() ;
}

double perimeterVSarea(const Domain domain, const DigitalSet object) {
    double perimeter = getPerimeter(domain, object) ;
    double area = getArea(object) ;
    return area*4*M_PI/(perimeter*perimeter) ;
}
