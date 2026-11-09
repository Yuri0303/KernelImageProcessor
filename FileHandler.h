//
// Created by yuri on 08/11/26.
//

#ifndef KERNELIMAGEPROCESSOR_FILEHANDLER_H
#define KERNELIMAGEPROCESSOR_FILEHANDLER_H

#include <string>

#include "Image.h"

class FileHandler {
private:
    [[nodiscard]] static bool isValidExtension(const std::string& path);

    static void skipCommentsAndSpaces(std::ifstream& file);

    [[nodiscard]] static uint16_t* parsePixelsAscii(std::ifstream& file, size_t matrixSize);

    [[nodiscard]] static uint16_t* parsePixelsBinary(std::ifstream& file, size_t matrixSize, uint16_t maxValue, uint16_t width, uint16_t height);


public:
    static Image* loadFile(const std::string& path);

    static void saveImage(const std::string& path);

};



#endif //KERNELIMAGEPROCESSOR_FILEHANDLER_H
