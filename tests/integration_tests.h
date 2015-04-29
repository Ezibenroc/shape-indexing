#ifndef INTEGRATION_TESTS_H
#define INTEGRATION_TESTS_H

#include <cppunit/TestFixture.h>
class IntegrationTests : public CppUnit::TestFixture {
    private:
        void testApple();
        void testDeer();
    public:
        static CppUnit::Test* suite();
};

#endif
