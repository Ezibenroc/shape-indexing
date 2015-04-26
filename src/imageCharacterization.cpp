#include "imageCharacterization.h"
#include "perimeterVSarea.h"

ImageCharacterization::ImageCharacterization(Image image) : image(image), object(image.domain()), signatureVector() {
    SetFromImage<DigitalSet>::append<Image>(this->object, image, 0, 255) ;
}

void ImageCharacterization::computeSignatureVector(void) {
    this->signatureVector.push_back(perimeterVSarea(this->image.domain(), this->object)) ;
}
