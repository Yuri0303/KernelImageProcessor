#include <iostream>
#include "FileHandler.h"
#include "Image.h"
#include "PnmImage.h"

int main() {
    Image* ppmB = FileHandler::loadImage("KernelImageProcessorTestBinary.ppm");
    FileHandler::saveImage("BinToPlain.ppm", *ppmB, false, ppmB->getMaxValue());
    FileHandler::saveImage("BinToBin.ppm", *ppmB, true, ppmB->getMaxValue());

    Image* ppmP = FileHandler::loadImage("KernelImageProcessorTestPlain.ppm");
    FileHandler::saveImage("PlainToPlain.ppm", *ppmP, false, ppmP->getMaxValue());
    FileHandler::saveImage("PlainToBin.ppm", *ppmP, true, ppmP->getMaxValue());

    Image* pgmB = FileHandler::loadImage("KernelImageProcessorTestPlain.pgm");
    FileHandler::saveImage("BinToPlain.pgm", *pgmB, false, pgmB->getMaxValue());
    FileHandler::saveImage("BinToBin.pgm", *pgmB, true, pgmB->getMaxValue());

    Image* pgmP = FileHandler::loadImage("KernelImageProcessorTestPlain.pgm");
    FileHandler::saveImage("PlainToPlain.pgm", *pgmP, false, pgmP->getMaxValue());
    FileHandler::saveImage("PlainToBin.pgm", *pgmP, true, pgmP->getMaxValue());

    Image* pbmB = FileHandler::loadImage("KernelImageProcessorTestPlain.pbm");
    FileHandler::saveImage("BinToPlain.pbm", *pbmB, false, pbmB->getMaxValue());
    FileHandler::saveImage("BinToBin.pbm", *pbmB, true, pbmB->getMaxValue());

    Image* pbmP = FileHandler::loadImage("KernelImageProcessorTestPlain.pbm");
    FileHandler::saveImage("PlainToPlain.pbm", *pbmP, false, pbmP->getMaxValue());
    FileHandler::saveImage("PlainToBin.pbm", *pbmP, true, pbmP->getMaxValue());

    return 0;
}
