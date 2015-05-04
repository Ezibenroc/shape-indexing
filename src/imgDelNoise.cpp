#include <iostream>
#include <vector>
#include <DGtal/base/Common.h>
#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/io/writers/GenericWriter.h"
#include "DGtal/helpers/StdDefs.h"

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i;

typedef ImageContainerBySTLMap<Domain, unsigned char> Image2D;

Image2D filterNoise(const Image2D& img) {
    Domain dom = img.domain();
    Image2D out(dom);
    unsigned int matrix[] = {
        1,2,1,
        2,4,2,
        1,2,1
    };
    unsigned int max = 0;
    for(unsigned int k=0;k<9;k++) {
        max += matrix[k];
    }
    for (Domain::ConstIterator it = dom.begin(), itend = dom.end(); it != itend; ++it) {
        Point p = *it;

        unsigned int value = 0;
        for(int i=-1; i<= 1; i++) {
            for(int j=-1; j<= 1; j++) {
                Point q = p + Point(i,j);
                if(dom.isInside(q) && img(q) > 0) {
                    value += matrix[(i+1)*3+(j+1)];
                }
            }
        }

        if (value >= max/2)
            out.setValue(p,1);
        else
            out.setValue(p,0);
    }
    return out;
}

int main(int argc, char **argv) {
    if(argc!=3) {
        cout << "Usage: " << argv[0] << " input.pgm output.pgm" << endl;
        exit(1);
    }
    const std::string input = argv[1];
    const std::string output = argv[2];

    Image2D image = GenericReader<Image2D>::import(input);
    Image2D filtered = filterNoise(image);
    Image2D result(image.domain());
    for(Domain::ConstIterator it = image.domain().begin(), itend = image.domain().end(); it!= itend; ++it)
    {
        if (filtered(*it))
            result.setValue(*it, 255);
        else
            result.setValue(*it, 0);
    }

    result >> output;

    exit(0);
}
