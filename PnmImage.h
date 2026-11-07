//
// Created by yuri on 07/11/26.
//

#ifndef KERNELIMAGEPROCESSOR_PNMIMAGE_H
#define KERNELIMAGEPROCESSOR_PNMIMAGE_H

#include <cstdint>
#include <stdexcept>
#include <type_traits>

#include "Image.h"

template<uint8_t bitDepth>
using pixelType = std::conditional_t<bitDepth == 8, uint8_t, uint16_t>;

template<uint8_t templateNChannels, uint8_t templateBitDepth>
class PnmImage : public Image {
private:
    pixelType<templateBitDepth>* pixels;
public:
    PnmImage(uint16_t width, uint16_t height, pixelType<templateBitDepth>* pixels);
    PnmImage(uint16_t width, uint16_t height);


    [[nodiscard]] uint16_t* getAt(int index) const override;

    [[nodiscard]] uint16_t* getAt(int posX, int posY) const override;

    ~PnmImage() override;

};

template<uint8_t templateNChannels, uint8_t templateBitDepth>
PnmImage<templateNChannels, templateBitDepth>::PnmImage(uint16_t width, uint16_t height) {
    this->width = width;
    this->height = height;
    this->nChannels = templateNChannels;
    this->bitDepth = templateBitDepth;
    this->pixels = new pixelType<templateBitDepth>[width * height * nChannels];

}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
PnmImage<templateNChannels, templateBitDepth>::PnmImage(uint16_t width, uint16_t height,
    pixelType<templateBitDepth>* pixels) {
    this->width = width;
    this->height = height;
    this->pixels = pixels;
    this->nChannels = templateNChannels;
    this->bitDepth = templateBitDepth;
}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
uint16_t* PnmImage<templateNChannels, templateBitDepth>::getAt(const int index) const {
    if (index < 0 || index >= width * height) {
        throw std::out_of_range("Index out of bounds");
    }
    auto* pixel = new uint16_t[nChannels];
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = pixels[index + i];
    }
    return pixel;
}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
uint16_t* PnmImage<templateNChannels, templateBitDepth>::getAt(const int posX, const int posY) const {
    if (posX < 0 || posY < 0 || posX >= width || posY >= height) {
        throw std::out_of_range("Index out of bound");
    }
    auto* pixel = new uint16_t[nChannels];
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = pixels[(posX * width) + posY + i];
    }
    return pixel;
}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
PnmImage<templateNChannels, templateBitDepth>::~PnmImage() {
    delete[] pixels;
}
#endif //KERNELIMAGEPROCESSOR_PNMIMAGE_H
