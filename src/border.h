#ifndef _BORDER_H
#define _BORDER_H

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

// Return the largest border (in number of points) of the given object.
vector<Point> extractBorder(const Domain domain, const DigitalSet object) ;

#endif
