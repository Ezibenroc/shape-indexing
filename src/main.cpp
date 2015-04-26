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

int main()
{
  std::string filename = "hop.pgm";
  //Image type (image of unsigned int)

  //We read the PGM file
  Image image = PGMReader<Image>::importPGM(filename);

  ImageCharacterization img = ImageCharacterization(image) ;

  trace.info() << "Image read :"<< image <<std::endl;

  return 0;
}
