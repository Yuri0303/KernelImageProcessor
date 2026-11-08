//
// Created by yuri on 08/11/26.
//

#ifndef KERNELIMAGEPROCESSOR_FILEHANDLER_H
#define KERNELIMAGEPROCESSOR_FILEHANDLER_H

#include <string>

#include "Image.h"

class FileHandler {
    static Image* loadFromFile(const std::string& path);

    static void createImage(const std::string& path);

};


#endif //KERNELIMAGEPROCESSOR_FILEHANDLER_H
