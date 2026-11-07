//
// Created by yuri on 07/11/26.
//

#include "Image.h"

uint16_t Image::getWidth() const {
    return width;
}

uint16_t Image::getHeight() const {
    return height;
}

uint8_t Image::getNChannels() const {
    return nChannels;
}

uint8_t Image::getBitDepth() const {
    return bitDepth;
}
