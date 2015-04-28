#ifndef BORDER_TESTS_H
#define BORDER_TESTS_H

#include <cppunit/TestFixture.h>
class BorderTests : public CppUnit::TestFixture {
    private:
        void testSimpleBorder();
    public:
        static CppUnit::Test* suite();
};

#endif
