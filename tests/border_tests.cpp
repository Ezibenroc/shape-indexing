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

#include "border_tests.h"
#include "common.h"
#include "../src/border.h"

void BorderTests::testSimpleBorder() {
    Domain domain(Point(0, 0), Point(100, 100)) ;
    DigitalSet object(domain) ;
    addRectangle(object, Point(5, 5), Point(10, 10)) ;
    vector<Point> border = extractBorder(domain, object) ;
    unsigned value = border.size() ;
    CPPUNIT_ASSERT_EQUAL((unsigned)17, value) ;
    for(int i = 5 ; i < 10 ; i++) {
        CPPUNIT_ASSERT(std::find(border.begin(), border.end(), Point(5, i)) != border.end()) ;
        CPPUNIT_ASSERT(std::find(border.begin(), border.end(), Point(9, i)) != border.end()) ;
        CPPUNIT_ASSERT(std::find(border.begin(), border.end(), Point(i, 5)) != border.end()) ;
        CPPUNIT_ASSERT(std::find(border.begin(), border.end(), Point(i, 9)) != border.end()) ;
    }
}

CppUnit::Test* BorderTests::suite() {
    CppUnit::TestSuite *suite = new CppUnit::TestSuite("BorderTests");
    suite->addTest(new CppUnit::TestCaller<BorderTests>("BorderTests_testSimpleBorder",
                &BorderTests::testSimpleBorder));
    return suite;
}
