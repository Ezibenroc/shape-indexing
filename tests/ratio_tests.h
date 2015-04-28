#ifndef RATIO_TESTS_H
#define RATIO_TESTS_H

#include <cppunit/TestFixture.h>
class RatioTests : public CppUnit::TestFixture {
    private:
        void testCircle();
        void testConvexity();
    public:
        static CppUnit::Test* suite();
};

#endif
