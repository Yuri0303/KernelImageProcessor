//
// Created by yuri on 07/11/26.
//

#ifndef KERNELIMAGEPROCESSOR_IMAGE_H
#define KERNELIMAGEPROCESSOR_IMAGE_H
#include <cstdint>


class Image {
protected:
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t nChannels = 0;
    uint8_t bitDepth = 0;

public:
    [[nodiscard]] uint16_t getWidth() const;
    [[nodiscard]] uint16_t getHeight() const;
    [[nodiscard]] uint8_t getNChannels() const;
    [[nodiscard]] uint8_t getBitDepth() const;

    [[nodiscard]] virtual uint16_t* getAt(int index) const = 0;
    [[nodiscard]] virtual uint16_t* getAt(int posX, int posY) const = 0;

    virtual ~Image() = default;
};


#endif //KERNELIMAGEPROCESSOR_IMAGE_H
