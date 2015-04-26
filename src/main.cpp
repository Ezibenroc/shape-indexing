///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "imageCharacterization.h"
#include "imageClass.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

int main(int argc, char *argv[])
{
    if(argc < 3) {
        cerr << "Syntax:" << argv[0] << " <filename 1> <other filenames>" << endl ;
        return 1 ;
    }
    string filename1 = argv[1] ;
    vector<string> filenames ;
    for(int i = 2 ; i < argc ; i++) {
        filenames.push_back((string) argv[i]) ;
    }
    ImageCharacterization img1(filename1) ;
    ImageClass images(filenames) ;
    cout << "distance min = " << images.minDistance(img1) << endl ;
    cout << "distance max = " << images.maxDistance(img1) << endl ;
    cout << "mean distance = " << images.meanDistance(img1) << endl ;
    img1.dump() ;
    images.dump() ;
    return 0;
}
