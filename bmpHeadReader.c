#define _CRT_SECURE_NO_WARNINGS

#include "bmpHeadReader.h"

void showBmpFileHead(BMPFILEHEADER bmpFileHeader)
{
    printf("BMP����ѧO�r�G%x\n", bmpFileHeader.bfType);
    printf("���j�p�G%ld\n", bmpFileHeader.bfSize);
    printf("�Ϲ���ڦ�m�G%lx\n", bmpFileHeader.bfOffBits);
}

void showBmpFileInfo(BMPINFOHEADER bmpInfoHeader)
{
    printf("BMP�H���Y\n");
    printf("��T�Y�j�p�G%d\n", bmpInfoHeader.biInfoSize);
    printf("�I�}�ϼe�סG%d\n", bmpInfoHeader.biWidth);
    printf("�I�}�ϰ��סG%d\n", bmpInfoHeader.biHeigth);
    printf("�Ϲ����줸���ơG%d\n", bmpInfoHeader.biPlanes);
    printf("�C�ӹϤ����줸�ơG%d\n", bmpInfoHeader.biBitCount);
    printf("���Y�覡�G%d\n", bmpInfoHeader.biCompression);
    printf("�Ϲ����j�p�G%d\n", bmpInfoHeader.biSizeImage);
    printf("���Ǥ�V�ѪR�סG%d\n", bmpInfoHeader.biXPelsPerMeter);
    printf("������V�ѪR�סG%d\n", bmpInfoHeader.biYPelsPerMeter);
    printf("�ϥΪ��C��ơG%d\n", bmpInfoHeader.biClrUsed);
    printf("���n�C��ơG%d\n", bmpInfoHeader.biClrImportant);
}
