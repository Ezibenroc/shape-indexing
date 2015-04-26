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
    if(argc != 2) {
        cerr << "Syntax:" << argv[0] << " <filename>" << endl ;
        return 1 ;
    }
    std::string filename = argv[1];
    //Image type (image of unsigned int)

    ImageCharacterization img = ImageCharacterization(filename) ;
    vector<double> signature = img.getSignatureVector() ;
    cout << signature[0] << endl ;
    img.dump() ;
    return 0;
}
