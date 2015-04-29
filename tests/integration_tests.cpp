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

#include "integration_tests.h"
#include "common.h"
#include "../src/imageCharacterization.h"

#define DELTA 0.001

void IntegrationTests::testApple() {
    ImageCharacterization image("../tests/data/apple-10.pgm", false) ;
    vector<double> signature = image.getSignatureVector() ;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.524, signature[0], DELTA) ;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.913, signature[1], DELTA) ;
}

void IntegrationTests::testDeer() {
    ImageCharacterization image("../tests/data/deer-12.pgm", false) ;
    vector<double> signature = image.getSignatureVector() ;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.070, signature[0], DELTA) ;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.446, signature[1], DELTA) ;
}

CppUnit::Test* IntegrationTests::suite() {
    CppUnit::TestSuite *suite = new CppUnit::TestSuite("IntegrationTests");
    suite->addTest(new CppUnit::TestCaller<IntegrationTests>("IntegrationTests_testApple",
                &IntegrationTests::testApple));
    suite->addTest(new CppUnit::TestCaller<IntegrationTests>("IntegrationTests_testDeer",
                &IntegrationTests::testDeer));
    return suite;
}
