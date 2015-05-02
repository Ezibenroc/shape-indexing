#ifndef _MOMENT_H
#define _MOMENT_H


#include <iostream>
#include <vector>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i;

typedef ImageContainerBySTLVector< Domain, unsigned int > Image;

// Compute moments of the image, using http://en.wikipedia.org/wiki/Image_moment
// These moments are translation, scale and rotation invariants.
vector<double> moment(Image image) ;

#endif
