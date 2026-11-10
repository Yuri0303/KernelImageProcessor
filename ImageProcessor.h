//
// Created by utente on 10/11/2026.
//

#ifndef KERNELIMAGEPROCESSOR_IMAGEPROCESSOR_H
#define KERNELIMAGEPROCESSOR_IMAGEPROCESSOR_H

#include "Image.h"
#include "Kernel.h"

enum class KernelMode {
    Extended,
    Warp,
    Mirror,
    Crop,
    KernelCrop,
    Constant
};

class ImageProcessor {
private:
    static Image* convolve(Image* input, Kernel kernel, KernelMode kernelMode);
public:
    static Image* applyEdgeDetection(Image* input);

    static Image* applySharpen(Image* input);

    static Image* applyGaussianBlur(Image* input);
};



#endif //KERNELIMAGEPROCESSOR_IMAGEPROCESSOR_H
