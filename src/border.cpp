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

vector<Point> extractBorder(const Domain domain, const DigitalSet object) {
    vector<Point> border;
    Point start ;
    //Search a point which is in the domain
    for(Domain::ConstIterator it = domain.begin();
        it != domain.end(); ++it)
      {
       if(object(*it)) {
          start = *it ;
          break ;
        }
      }
      border.push_back(start) ;
      vector<Point> neigh = getNeighbours(domain, start) ;
      for(unsigned i = 0 ; i <= neigh.size() ; i++) {
          if(!object(neigh[mod(i-1, neigh.size())]) && object(neigh[mod(i, neigh.size())]))
            border.push_back(neigh[mod(i, neigh.size())]) ;
      }
      assert(border.size() == 2) ;
      while(border.back() != start) {
          border.push_back(nextPoint(object, border.end()[-2], border.end()[-1])) ;
      }
      return border ;
}
