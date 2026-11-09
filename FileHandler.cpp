//
// Created by yuri on 08/11/26.
//

#include "FileHandler.h"
#include "PnmImage.h"

#include <fstream>

bool FileHandler::isValidExtension(const std::string& path) {
    const std::string extension = path.substr(path.find_last_of('.'));
    return extension == ".pbm" || extension == ".pgm" || extension == ".ppm";
}

void FileHandler::skipCommentsAndSpaces(std::ifstream& file) {
    char c;
    while (file.get(c)) {
        if (c == '#') {
            std::string line;
            std::getline(file, line);
        } else if (!std::isspace(static_cast<unsigned char>(c))) {
            file.unget();
            break;
        }
    }
}

uint16_t* FileHandler::parsePixelsAscii(std::ifstream& file, size_t matrixSize) {
    auto* pixels = new uint16_t[matrixSize];
    uint16_t pixel;
    int i = 0;
    while (i < matrixSize && file >> pixel) {
        pixels[i] = pixel;
        i++;
        skipCommentsAndSpaces(file);
    }
    return pixels;
}

uint16_t* FileHandler::parsePixelsBinary(std::ifstream& file, const size_t matrixSize, const uint16_t maxValue, const uint16_t width, const uint16_t height) {
    auto* pixels = new uint16_t[matrixSize];
    int n = 0;
    char c;
    if (maxValue == 1) {
        while (n < matrixSize && file.get(c)) {
            for (int i = 7; i >= 0; i--) {
                pixels[n] =(static_cast<unsigned char>(c) >> i) & 1;
                n++;
                if (n % width == 0) {
                    break;
                }
            }
        }
    }
    if (maxValue < 256) {
        while (n < matrixSize && file.get(c)) {
            pixels[n] = static_cast<uint16_t>(static_cast<unsigned char>(c));
            n++;
        }
    } else {
        char cArr[2];
        while (n < matrixSize && file.read(cArr, 2)) {
            pixels[n] = (static_cast<unsigned char>(cArr[0]) << 8) | static_cast<unsigned char>(cArr[1]);
            n++;
        }
    }

    return pixels;
}

Image* FileHandler::loadFile(const std::string& path) {
    if (!isValidExtension(path))
        throw std::runtime_error("File not supported");
    std::ifstream file(path, std::ios_base::binary);
    if (!file.is_open())
        throw std::runtime_error("File not found");

    std::string magicNumber;
    uint16_t width, height;
    uint16_t maxValue = 1;
    uint8_t nChannels = 1;
    uint16_t* pixels = nullptr;


    file >> magicNumber;
    skipCommentsAndSpaces(file);

    file >> width;
    skipCommentsAndSpaces(file);

    file >> height;
    skipCommentsAndSpaces(file);

    if (magicNumber != "P1" && magicNumber != "P4") {
        file >> maxValue;
        skipCommentsAndSpaces(file);
    }

    uint8_t bitDepth = maxValue > 255 ? 8 : 16;

    if (magicNumber == "P1" || magicNumber == "P2") {
        nChannels = 1;
        pixels = parsePixelsAscii(file, width * height);
    }
    if (magicNumber == "P3") {
        nChannels = 3;
        pixels = parsePixelsAscii(file, width * height * nChannels);

    }
    if (magicNumber == "P4" || magicNumber == "P5") {
        nChannels = 1;
        pixels = parsePixelsBinary(file, width * height, maxValue, width, height);
    }
    if (magicNumber == "P6") {
        nChannels = 3;
        pixels = parsePixelsBinary(file, width * height * nChannels, maxValue, width, height);
    }

    if (nChannels == 1) {
        if (bitDepth == 8) {
            return new PnmImage<1, 8>(width, height, reinterpret_cast<uint8_t*>(pixels));
        }
        return new PnmImage<1, 16>(width, height, pixels);
    }
    if (bitDepth == 8) {
        return new PnmImage<3, 8>(width, height, reinterpret_cast<uint8_t*>(pixels));
    }
    return new PnmImage<3, 16>(width, height, pixels);
}

void FileHandler::saveImage(const std::string& path) {
}
