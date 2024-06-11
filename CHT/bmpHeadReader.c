#define _CRT_SECURE_NO_WARNINGS

#include "bmpHeadReader.h"

void showBmpFileHead(BMPFILEHEADER bmpFileHeader)
{
    printf("bfType: %x\n", bmpFileHeader.bfType);
    printf("bfSize: %ld\n", bmpFileHeader.bfSize);
    printf("bfOffBits: %lx\n", bmpFileHeader.bfOffBits);
}

void showBmpFileInfo(BMPINFOHEADER bmpInfoHeader)
{
    printf("BMP Info Header\n");
    printf("biInfoSize: %d\n", bmpInfoHeader.biInfoSize);
    printf("biWidth: %d\n", bmpInfoHeader.biWidth);
    printf("biHeigth: %d\n", bmpInfoHeader.biHeigth);
    printf("biPlanes: %d\n", bmpInfoHeader.biPlanes);
    printf("biBitCount: %d\n", bmpInfoHeader.biBitCount);
    printf("biCompression: %d\n", bmpInfoHeader.biCompression);
    printf("biSizeImage: %d\n", bmpInfoHeader.biSizeImage);
    printf("biXPelsPerMeter: %d\n", bmpInfoHeader.biXPelsPerMeter);
    printf("biYPelsPerMeter: %d\n", bmpInfoHeader.biYPelsPerMeter);
    printf("biClrUsed: %d\n", bmpInfoHeader.biClrUsed);
    printf("biClrImportant: %d\n", bmpInfoHeader.biClrImportant);
}

void showBmpRgbQuad(RGBQUAD rgbQuad)
{
    printf("BMP RGB Quad\n");
    printf("biWhite: %d\n", rgbQuad.rgbWhite);
    printf("biBlack: %d\n", rgbQuad.rgbBlack);
}
