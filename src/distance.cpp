///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <thread>
#include <mutex>
#include <dirent.h>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "imageCharacterization.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

int main(int argc, char *argv[]) {
    if(argc != 3) {
        cerr << "Syntax: " << argv[0] << " <image 1> <image 2>" << endl ;
        exit(EXIT_FAILURE) ;
    }


    ImageCharacterization img1(argv[1]) ;
    ImageCharacterization img2(argv[2]) ;
    vector<double> normalization = { // generated with the file descriptors.info
        0.897024,
        1.00415,
        1.54932,
        2.16182,
        0.941636,
        0.327149,
        0.181375,
        0.203551,
        0.0035972,
        0.0128021
    } ;
    cout << img1.distance(img2, normalization) << endl ;
    return 0;
}
