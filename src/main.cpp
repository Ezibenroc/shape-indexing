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
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

int main(int argc, char *argv[])
{
    if(argc != 3) {
        cerr << "Syntax:" << argv[0] << " <filename 1> <filename 2>" << endl ;
        return 1 ;
    }
    string filename1 = argv[1];
    string filename2 = argv[2];
    //Image type (image of unsigned int)
    ImageCharacterization img1 = ImageCharacterization(filename1) ;
    ImageCharacterization img2 = ImageCharacterization(filename2) ;
    cout << "distance = " << img1.distance(img2) << endl ;
    img1.dump() ;
    img2.dump() ;
    return 0;
}
