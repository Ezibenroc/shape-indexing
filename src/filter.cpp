#include "filter.h"

Image filterNoise(const Image& img) {
    Domain dom = img.domain();
    Image out(dom);
    unsigned int matrix[] = {
        1,2,1,
        2,4,2,
        1,2,1
    };
    unsigned int max = 0;
    for(unsigned int k=0;k<9;k++) {
        max += matrix[k];
    }
	for (auto it = dom.begin(), itend = dom.end(); it != itend; ++it) {
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
