#include <set>
#include "border.h"

vector<Point> pos = {Point(-1, -1), Point(-1, 0), Point(-1, 1), Point(0, 1), Point(1, 1), Point(1, 0), Point(1, -1), Point(0, -1)};

vector<Point> getNeighbours(const Domain d, Point p) {
    vector<Point> neigh ;
    for(unsigned i = 0 ; i < pos.size()  ; i++) {
        Point other = pos[i] + p ;
        if(d.isInside(other))
            neigh.push_back(other) ;
    }
    return neigh ;
}


unsigned mod(int p, int n) {
    if(p>=n)
        return p-n ;
    if(p<0)
        return p+n ;
    return p ;
}

Point nextPoint(const DigitalSet object, Point before, Point now) {
    Point diff = now-before ;
    unsigned i = 0 ;
    for(i = 0 ; i < pos.size() && pos[i] != diff ; i++)
        continue ;
    assert(i < pos.size()) ;
    i = mod(i-2, pos.size()) ;
    while(!object(now + pos[i]))
        i = mod(i+1, pos.size()) ;
    return now + pos[mod(i, pos.size())] ;
}

// Return the border containing the point start
vector<Point> extractBorderFromPoint(const Domain domain, const DigitalSet object, Point start) {
    vector<Point> border ;
    border.push_back(start) ;
    vector<Point> neigh = getNeighbours(domain, start) ;
    for(unsigned i = 0 ; i <= neigh.size() ; i++) {
        if(!object(neigh[mod(i-1, neigh.size())]) && object(neigh[mod(i, neigh.size())])) {
          border.push_back(neigh[mod(i, neigh.size())]) ;
          break ;
      }
    }
    assert(border.size() == 2) ;
    while(border.back() != start) {
        border.push_back(nextPoint(object, border.end()[-2], border.end()[-1])) ;
    }
    return border ;
}

// Return the border of the biggest connected component (in number of points).
vector<Point> extractBorder(const Domain domain, const DigitalSet object) {
    vector<Point> border;
    vector<Point> tmp ;
    set<Point> visited ;
    Point lowerBound = domain.lowerBound(), upperBound = domain.upperBound() ;
    for(int i = lowerBound[0]+1 ; i < upperBound[0] ; i++) {
        for(int j = lowerBound[1] ; j < upperBound[1] ; j++) {
            if(object(Point(i, j)) && !object(Point(i-1, j)) && visited.find(Point(i, j)) == visited.end()) {
                tmp = extractBorderFromPoint(domain, object, Point(i, j)) ;
                visited.insert(tmp.begin(), tmp.end()) ;
                if(tmp.size() > border.size()) {
                    border = tmp ;
                }
            }
        }
    }
    return border ;
}
