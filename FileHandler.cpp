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

void* FileHandler::parsePixelsAscii(std::ifstream& file, const size_t matrixSize, const uint8_t bitDepth) {
    void* pixels;
    if (bitDepth == 1) {
        pixels = new uint8_t[matrixSize];
        uint8_t pixel;
        int i = 0;
        while (i < matrixSize && !file.eof()) {
            pixel = file.get() - 0x30;  //get() reads the byte, that is '1' -> so 49 decimal. To convert, subtract 30 hex (48 decimal)
            static_cast<uint8_t*>(pixels)[i] = pixel;
            i++;
            skipCommentsAndSpaces(file);
        }

    } else if (bitDepth == 8) {
        pixels = new uint8_t[matrixSize];
        uint16_t pixel;
        int i = 0;
        while (i < matrixSize && file >> pixel) {
            static_cast<uint8_t*>(pixels)[i] = pixel;
            i++;
            skipCommentsAndSpaces(file);
        }

    } else {
        pixels = new uint16_t[matrixSize];
        uint16_t pixel;
        int i = 0;
        while (i < matrixSize && file >> pixel) {
            static_cast<uint16_t*>(pixels)[i] = pixel;
            i++;
            skipCommentsAndSpaces(file);
        }
    }
    return pixels;
}

void* FileHandler::parsePixelsBinary(std::ifstream& file, const size_t matrixSize, const uint16_t maxValue,
                                     const uint16_t width, const uint16_t height) {
    void* pixels;
    int n = 0;
    char c;
    if (maxValue == 1) {
        pixels = new uint8_t[matrixSize];
        while (n < matrixSize && file.get(c)) {
            for (int i = 7; i >= 0; i--) {
                static_cast<uint8_t*>(pixels)[n] = (static_cast<unsigned char>(c) >> i) & 1;
                n++;
                if (n % width == 0) {
                    break;
                }
            }
        }
    } else if (maxValue < 256) {
        pixels = new uint8_t[matrixSize];
        while (n < matrixSize && file.get(c)) {
            static_cast<uint8_t*>(pixels)[n] = static_cast<uint16_t>(static_cast<unsigned char>(c));
            n++;
        }
    } else {
        pixels = new uint16_t[matrixSize];
        char cArr[2];
        while (n < matrixSize && file.read(cArr, 2)) {
            static_cast<uint16_t*>(pixels)[n] = (static_cast<unsigned char>(cArr[0]) << 8) | static_cast<unsigned char>(cArr[1]);
            n++;
        }
    }

    return pixels;
}

Image* FileHandler::loadImage(const std::string& path) {
    if (!isValidExtension(path))
        throw std::runtime_error("File not supported");
    std::ifstream file(path, std::ios_base::binary);
    if (!file.is_open())
        throw std::runtime_error("File not found");

    std::string magicNumber;
    uint16_t width, height;
    uint16_t maxValue = 1;
    uint8_t nChannels = 1;
    void* pixels = nullptr;


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

    uint8_t bitDepth = maxValue == 1 ? 1 : (maxValue > 255 ? 16 : 8);

    if (magicNumber == "P1" || magicNumber == "P2") {
        nChannels = 1;
        pixels = parsePixelsAscii(file, width * height, bitDepth);
    }
    if (magicNumber == "P3") {
        nChannels = 3;
        pixels = parsePixelsAscii(file, width * height * nChannels, bitDepth);
    }
    if (magicNumber == "P4" || magicNumber == "P5") {
        nChannels = 1;
        pixels = parsePixelsBinary(file, width * height, maxValue, width, height);
    }
    if (magicNumber == "P6") {
        nChannels = 3;
        pixels = parsePixelsBinary(file, width * height * nChannels, maxValue, width, height);
    }
    if (!pixels) {
        throw std::logic_error("Pixel matrix not initialized");
    }

    file.close();

    if (nChannels == 1) {
        if (bitDepth == 8) {
            return new PnmImage<1, 8>(width, height, maxValue, static_cast<uint8_t*>(pixels));
        }
        if (bitDepth == 1) {
            return new PnmImage<1, 1>(width, height, maxValue, static_cast<uint8_t*>(pixels));
        }
        return new PnmImage<1, 16>(width, height, maxValue, static_cast<uint16_t*>(pixels));
    }
    if (bitDepth == 8) {
        return new PnmImage<3, 8>(width, height, maxValue, static_cast<uint8_t*>(pixels));
    }
    if (bitDepth == 1) {
        return new PnmImage<3, 1>(width, height, maxValue, static_cast<uint8_t*>(pixels));
    }
    return new PnmImage<3, 16>(width, height, maxValue, static_cast<uint16_t*>(pixels));
}

void FileHandler::saveImage(const std::string& path, const Image& image, const bool binary, const int maxValue) {
    std::ofstream file;
    if (binary) {
        file.open(path, std::ios_base::binary | std::ios_base::trunc);
    } else {
        file.open(path, std::ios_base::trunc);
    }
    if (!file.is_open()) {
        throw std::runtime_error("Error in file saving");
    }
    int magicNumber;
    if (image.getNChannels() == 1) {
        if (!binary) {
            if (image.getBitDepth() == 1) {
                magicNumber = 1;
            } else {
                magicNumber = 2;
            }
        } else {
            if (image.getBitDepth() == 1) {
                magicNumber = 4;
            } else {
                magicNumber = 5;
            }
        }
    } else {
        if (!binary) {
            magicNumber = 3;
        } else {
            magicNumber = 6;
        }
    }

    const uint16_t width = image.getWidth();
    const uint16_t height = image.getHeight();

    file << "P" << magicNumber << std::endl;
    file << width << " " << height << std::endl;
    if (image.getBitDepth() != 1) {
        file << maxValue << std::endl;
    }

    if (!binary) {
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                auto* pixel = image.getAt(row, col);
                for (int channel = 0; channel < image.getNChannels(); channel++) {
                    file << pixel[channel] << " ";
                }
                delete[] pixel;
            }
            file << std::endl;
        }
    } else {    //binary
        if (image.getBitDepth() == 1) {
            uint8_t byte = 0;
            int n = 0;
            while (n < width * height) {
                for (int i = 0; i < 8; i++) {
                    byte = byte << 1;
                    auto* pixel = image.getAt(n);
                    byte = static_cast<uint8_t>(*pixel) | byte;
                    delete[] pixel;
                    n++;
                    if (n % width == 0) {
                        byte = byte << (8 - (i + 1));   //i+1 is the right number of bit inserted
                        break;
                    }
                }
                file.put(static_cast<char>(byte));
                byte = 0;
            }
        } else if (image.getBitDepth() == 8) {
            for (int i = 0; i < width * height; i++) {
                auto* pixel = image.getAt(i);
                for (int channel = 0; channel < image.getNChannels(); channel++) {
                    file.put(static_cast<char>(pixel[channel]));
                }
                delete[] pixel;
            }
        } else {    //bitDepth == 16
            for (int i = 0; i < width * height; i++) {
                auto pixel = image.getAt(i);
                for (int channel = 0; channel < image.getNChannels(); channel++) {
                    uint8_t mostSignificantByte = pixel[channel] >> 8;
                    uint8_t leastSignificantByte = pixel[channel] & 0x00FF;
                    file.put(static_cast<char>(mostSignificantByte));
                    file.put(static_cast<char>(leastSignificantByte));
                }
                delete[] pixel;
            }
        }
    }
    file.close();
    if (!file) {
        throw std::runtime_error("Error while saving output file");
    }
}
