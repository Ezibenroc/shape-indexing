///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <cassert>
#include <boost/filesystem.hpp>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "imageCharacterization.h"
#include "imageClass.h"
///////////////////////////////////////////////////////////////////////////////

namespace fs = boost::filesystem;
using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

int main(int argc, char *argv[])
{
    if(argc < 3) {
        cerr << "Syntax:" << argv[0] << " <image filename> <classes repositories>" << endl ;
        return 1 ;
    }

    vector<ImageClass> classes ;
    // Check validity of directories
    for(int i = 2 ; i < argc ; i++) {
        fs::path dir(argv[i]) ;
        if(!fs::exists(dir) || !fs::is_directory(dir)) {
            cerr << "Error: " << argv[i] << " is not a directory." << endl ;
            return 1 ;
        }
    }
    for(int i = 2 ; i < argc ; i++) {
        vector<string> filenames ;
        fs::path dir(argv[i]) ;
        fs::directory_iterator end ;
        for(fs::directory_iterator iter(dir) ; iter != end ; ++iter) {
            assert(fs::is_regular_file(iter->status())) ;
            filenames.push_back(iter->path().string());
        }
        classes.push_back(ImageClass(filenames));
        filenames.clear() ;
    }

    string filename1 = argv[1] ;
    ImageCharacterization img1(filename1) ;
    for(unsigned i = 0 ; i < classes.size() ; i++) {
        cout << "# " << argv[i+2] << endl ;
        cout << "distance min = " << classes[i].minDistance(img1) << endl ;
        cout << "distance max = " << classes[i].maxDistance(img1) << endl ;
        cout << "mean distance = " << classes[i].meanDistance(img1) << endl << endl ;
        classes[i].dump() ;
    }
    img1.dump() ;
    return 0;
}
