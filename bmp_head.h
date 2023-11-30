#ifndef BMP_H
#define BMP_H

#pragma pack(1)

typedef struct BMP_FILE_HEADER
{
    unsigned short bfType; //����ѧO�r
    unsigned long  bfSize; //���ؤo
    unsigned short bfReserved1; //�O�d�r�A�T�w��0
    unsigned short bfReserved2; //�O�d�r�A�T�w��0
    unsigned long  bfOffBits; //�v���ɰ_�l��}
}BMPFILEHEADER;

typedef struct BMP_INFO_HEADER
{
    unsigned long  biInfoSize; //��T�Y�j�p
    long           biWidth; //�Ϲ��e��
    long           biHeigth; //�Ϲ�����
    unsigned short biPlanes; //�Ϲ��줸����
    unsigned short biBitCount; //�C�ӹϤ����줸��
    unsigned long  biCompression; //���Y����
    unsigned long  biSizeImage; //�Ϲ��j�p�A�줸�լ����
    long           biXPelsPerMeter; //���ǸѪR��
    long           biYPelsPerMeter; //�����ѪR��
    unsigned long  biClrUsed; //�ϥΦ�m��
    unsigned long  biClrImportant; //���n�C���
}BMPINFOHEADER;

typedef struct RGB_QUAD
{
    unsigned long  rgbWhite; //�զ�
    unsigned long  rgbBlack; //�¦�
}RGBQUAD;

#endif

