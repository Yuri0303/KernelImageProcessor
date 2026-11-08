//
// Created by utente on 08/11/2026.
//

#ifndef KERNELIMAGEPROCESSOR_KERNEL_H
#define KERNELIMAGEPROCESSOR_KERNEL_H

#include "Image.h"

class Kernel {
private:
    float* matrix;
    uint8_t size;
public:
    explicit Kernel(uint8_t size);

    static Kernel createSharpen();

    static Kernel createGaussian();

    [[nodiscard]] float getAt(int index) const;

};


#endif //KERNELIMAGEPROCESSOR_KERNEL_H
