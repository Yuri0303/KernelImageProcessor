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
using pixelType = std::conditional_t<bitDepth == 8 || bitDepth == 1, uint8_t, uint16_t>;

template<uint8_t templateNChannels, uint8_t templateBitDepth>
class PnmImage : public Image {
private:
    pixelType<templateBitDepth>* pixels;
    uint16_t maxValue;
public:
    PnmImage(uint16_t width, uint16_t height, uint16_t maxValue);
    PnmImage(uint16_t width, uint16_t height, uint16_t maxValue, pixelType<templateBitDepth>* pixels);

    [[nodiscard]] uint16_t getMaxValue() const override;

    [[nodiscard]] uint16_t* getAt(int index) const override;

    [[nodiscard]] uint16_t* getAt(int row, int col) const override;

    ~PnmImage() override;

};

template<uint8_t templateNChannels, uint8_t templateBitDepth>
PnmImage<templateNChannels, templateBitDepth>::PnmImage(uint16_t width, uint16_t height, uint16_t maxValue) {
    this->width = width;
    this->height = height;
    this->maxValue = maxValue;
    this->nChannels = templateNChannels;
    this->bitDepth = templateBitDepth;
    this->pixels = new pixelType<templateBitDepth>[width * height * nChannels];
}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
PnmImage<templateNChannels, templateBitDepth>::PnmImage(uint16_t width, uint16_t height,
                                                        uint16_t maxValue, pixelType<templateBitDepth>* pixels) {
    this->width = width;
    this->height = height;
    this->maxValue = maxValue;
    this->nChannels = templateNChannels;
    this->bitDepth = templateBitDepth;
    this->pixels = pixels;
}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
uint16_t PnmImage<templateNChannels, templateBitDepth>::getMaxValue() const {
    return maxValue;
}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
uint16_t* PnmImage<templateNChannels, templateBitDepth>::getAt(const int index) const {
    if (index < 0 || index >= width * height) {
        throw std::out_of_range("Index out of bounds");
    }
    auto* pixel = new uint16_t[nChannels];
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = static_cast<uint16_t>(pixels[index * nChannels + i]);
    }
    return pixel;
}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
uint16_t* PnmImage<templateNChannels, templateBitDepth>::getAt(const int row, const int col) const {
    if (row < 0 || col < 0 || row >= height || col >= width) {
        throw std::out_of_range("Index out of bound");
    }
    auto* pixel = new uint16_t[nChannels];
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = static_cast<uint16_t>(pixels[(row * width + col) * nChannels + i]);
    }
    return pixel;
}

template<uint8_t templateNChannels, uint8_t templateBitDepth>
PnmImage<templateNChannels, templateBitDepth>::~PnmImage() {
    delete[] pixels;
}
#endif //KERNELIMAGEPROCESSOR_PNMIMAGE_H
