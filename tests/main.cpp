#include <cppunit/ui/text/TestRunner.h>
#include "border_tests.h"
#include "ratio_tests.h"
#include "integration_tests.h"
#include <time.h>
#include <stdlib.h>


int main(void) {
    srand(static_cast<unsigned int>(time(NULL)));
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(BorderTests::suite());
    runner.addTest(RatioTests::suite());
    runner.addTest(IntegrationTests::suite());
    runner.run();
    return 0;
}
