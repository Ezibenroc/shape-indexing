#include <cmath>
#include <algorithm>
#include "border.h"
#include "convexity.h"

int orientation(Point p, Point q, Point r) {
    Point pq = q-p ;
    Point qr = r-q ;
    int det = pq[0]*qr[1] - pq[1]*qr[0] ;
    return (det==0)?0:(det>0?1:-1) ;
}

bool lexicalOrder(Point p, Point q) {
    if (p[0] == q[0])
        return p[1] < q[1] ;
    return p[0] < q[0] ;
}

class orientationOrder {
    Point p ;
public:
    orientationOrder(Point p0): p(p0) {}
    bool operator () (Point q, Point r) {
        return orientation(p,q,r) > 0 ;
    }
};


// Graham Scan
vector<Point> getConvexHull(Domain domain, DigitalSet object) {
    vector<Point> border = extractBorder(domain, object) ;

    if(border.size() <= 2)
        return border ;

    auto min = min_element(border.begin(), border.end(), lexicalOrder) ;
    Point p0 = *min ;
    border.erase(min) ;

    // First step
    sort(border.begin(), border.end(), orientationOrder(p0)) ;

    // Second step
    vector<Point> convexHull ;
    convexHull.push_back(p0) ;
    convexHull.push_back(border[0]) ;

    for(unsigned i = 1 ; i < border.size()-1 ; i++) {
        Point q = convexHull.back() ;
        convexHull.pop_back() ;
        Point p = convexHull.back() ;
        Point r = border[i] ;
        while(orientation(p,q,r) <= 0 && convexHull.size() >= 1) {
            q = p;
            convexHull.pop_back() ;
            p = convexHull.back();
        }
        convexHull.push_back(q);
        convexHull.push_back(r);
    }

    return convexHull;
}

double getConvexHullArea(Domain domain, DigitalSet object) {
    vector<Point> convexHull = getConvexHull(domain, object) ;
    double area = 0.0;
    for(unsigned i = 0 ; i < convexHull.size() ; i++) {
        Point p = convexHull[i];
        Point q = convexHull[i+1];
        area += (p[0]*q[1] - p[1]*q[0]) / 2.0;
    }
    return abs(area) ;
}

double convexity(Domain domain, DigitalSet object) {
    double convexArea = getConvexHullArea(domain, object) ;
    double area = (double)object.size() ;
    return area/convexArea ;
}
