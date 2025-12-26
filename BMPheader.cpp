/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include "BMPheader.h"
#include <vector>

BmpHeader::BmpHeader(uint32_t width, uint32_t height)
{
    BmpFile.bfType = 0x4D42;
    BmpFile.bfReserved1 = 0;
    BmpFile.bfReserved2 = 0;
    BmpFile.bfOffset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BmpInfo.biSize = 40;
    BmpInfo.biWidth = static_cast<int32_t>(width);
    BmpInfo.biHeight = static_cast<int32_t>(height);
    BmpInfo.biPlanes = 1;
    BmpInfo.biBitCount = 24;
    BmpInfo.biCompression = 0;
    BmpInfo.biXPelsPerMeter = 0;
    BmpInfo.biYPelsPerMeter = 0;
    BmpInfo.biClrUsed = 0;
    BmpInfo.biClrImportant = 0;

    uint32_t rowSize = ((width * 3 + 3) / 4) * 4;
    BmpInfo.biSizeImage = rowSize * height;
    BmpFile.bfSize = BmpFile.bfOffset + BmpInfo.biSizeImage;
}

bool BmpHeader::read(std::ifstream& file)
{
    if (!file.read(reinterpret_cast<char*>(&BmpFile), sizeof(BmpFile)))
        return false;

    if (BmpFile.bfType != 0x4D42)
        return false;

    if (!file.read(reinterpret_cast<char*>(&BmpInfo), sizeof(BmpInfo)))
        return false;

    uint32_t headerEnd = sizeof(BITMAPFILEHEADER) + BmpInfo.biSize;
    if (BmpFile.bfOffset < headerEnd)
        return false;

    uint32_t extraBytes = BmpFile.bfOffset - headerEnd;
    if (extraBytes > 0)
        file.seekg(extraBytes, std::ios::cur);

    return true;
}

bool BmpHeader::write(std::ofstream& file) const
{
    if (!file.write(reinterpret_cast<const char*>(&BmpFile), sizeof(BmpFile)))
        return false;

    if (!file.write(reinterpret_cast<const char*>(&BmpInfo), sizeof(BmpInfo)))
        return false;

    uint32_t written = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    if (BmpFile.bfOffset < written)
        return false;

    uint32_t padding = BmpFile.bfOffset - written;
    if (padding > 0)
    {
        std::vector<char> zeros(padding, 0);
        if (!file.write(zeros.data(), zeros.size()))
            return false;
    }

    return true;
}

void BmpHeader::updateForRotation(uint32_t newW, uint32_t newH)
{
    setWidth(newW);
    setHeight(newH);
    uint32_t rowSize = ((newW * 3 + 3) / 4) * 4;
    setImageSize(rowSize * newH);
    BmpFile.bfSize = BmpFile.bfOffset + getImageSize();
}
