#include <cppunit/Test.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <cmath>
#include <cassert>
#include <algorithm>
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "ratio_tests.h"
#include "../src/border.h"
#include "../src/perimeterVSarea.h"
#include "../src/convexity.h"

double distPoint(Point p1, Point p2) {
    return sqrt(pow(p1[0]-p2[0], 2) + pow(p1[1]-p2[1], 2)) ;
}

DigitalSet getCircle(Domain domain, int radius, Point center) {
    //We define a Set (set of points in a domain)
    DigitalSet circle(domain);

    //We first iterate on a domain
    for(Domain::ConstIterator it = domain.begin();
        it != domain.end(); ++it)
      {
        Point p = *it; //Domain::ConstIterator returns a point when
               //dereferrenced
       if(distPoint(p, center) <= radius)
          circle.insert(p) ;
      }
    return circle ;
}

void RatioTests::testCircle() {
    Domain domain(Point(0, 0), Point(500, 500)) ;
    DigitalSet object = getCircle(domain, 50, Point(250, 250)) ;
    vector<Point> border = extractBorder(domain, object) ;
    double d = perimeterVSarea(domain, object, border) ;
    double expected = 1 ;
    double delta = 0.1 ;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, d, delta) ;
}

void RatioTests::testConvexity() {
    Domain domain(Point(0, 0), Point(500, 500)) ;
    DigitalSet object = getCircle(domain, 50, Point(250, 250)) ;
    vector<Point> border = extractBorder(domain, object) ;
    double d = convexity(domain, object, border) ;
    double expected = 1 ;
    double delta = 0.1 ;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, d, delta) ;
}

CppUnit::Test* RatioTests::suite() {
    CppUnit::TestSuite *suite = new CppUnit::TestSuite("RatioTests");
    suite->addTest(new CppUnit::TestCaller<RatioTests>("RatioTests_testCircle",
                &RatioTests::testCircle));
    suite->addTest(new CppUnit::TestCaller<RatioTests>("RatioTests_testConvexity",
                &RatioTests::testConvexity));
    return suite;
}
