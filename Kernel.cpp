//
// Created by utente on 08/11/2026.
//

#include "Kernel.h"

#include <stdexcept>

Kernel::Kernel(const uint8_t size): size(size) {
    matrix = nullptr;
}

Kernel Kernel::createSharpen() {
    Kernel kernel(9);
    kernel.matrix = new float[kernel.size]{0, -1, 0, -1, 5, -1, 0, -1, 0};
    return kernel;
}

Kernel Kernel::createGaussian() {
    Kernel kernel(9);
    kernel.matrix = new float[kernel.size]{1, 2, 1, 2, 4, 2, 1, 2, 1};
    for (int i = 0; i < kernel.size; i++) {
        kernel.matrix[i] = kernel.matrix[i] / 16;
    }
    return kernel;
}

float Kernel::getAt(const int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of bound");
    }
    return matrix[index];
}

