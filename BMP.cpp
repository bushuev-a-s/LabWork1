/*
 Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include <exception>
#include <iostream>
#include "BMP.h"
#include <fstream>
#include <algorithm>
#include <cmath>

//length of row with padding
uint32_t BmpImage::calculateRowSize() const
{
    uint32_t w = getWidth();
    return ((w * 3 + 3) / 4) * 4;
}

//the amount of zeros in the end
uint32_t BmpImage::calculatePadding() const
{
    uint32_t w = getWidth();
    return (4 - (w * 3) % 4) % 4;
}

bool BmpImage::load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }
    if (!header.read(file))
    {
        return false;
    }

    uint32_t h = getHeight();
    uint32_t rowSize = calculateRowSize();
    pixelData.assign(rowSize * h, 0);

    file.seekg(header.getDataOffset(), std::ios::beg); //going to proper position


    for (uint32_t y = 0; y < h; y++)
    {
        uint32_t rowIndex = (h - 1 - y) * rowSize;

        if (!file.read(reinterpret_cast<char*>(&pixelData[rowIndex]), rowSize))
        {
            return false;
        }
    }

    return true;
}



bool BmpImage::save(const std::string& filename) const
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }
    if (!header.write(file))
    {
        return false;
    }

    uint32_t h = getHeight();
    uint32_t rowSize = calculateRowSize();

    for (uint32_t y = 0; y < h; y++)
    {
        uint32_t rowIndex =(h - 1 - y) * rowSize;

        if (!file.write(reinterpret_cast<const char*>(&pixelData[rowIndex]), rowSize))
        {
            return false;
        }
    }
    return true;
}

void BmpImage::getPixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b) const
{
    //check boaders: if they are bigger then it's padding and pixels are zeros
    if (x >= getWidth() || y >= getHeight())
    {
        r = g = b = 0;
        return;
    }
    uint32_t rowSize = calculateRowSize();
    uint32_t index = y * rowSize + x * 3;
    b = pixelData[index]; //blue color
    g = pixelData[index+1]; //green color
    r = pixelData[index+2]; //red color
}

void BmpImage::setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    //check boaders: if they are bigger we do nithing
    if (x >= getWidth() || y >= getHeight())
    {
        return;
    }

    uint32_t rowSize = calculateRowSize();
    uint32_t index = y * rowSize + x * 3;
    pixelData[index] = b;     //blue color
    pixelData[index+1] = g;   //green color
    pixelData[index+2] = r;   //red color
}

//Clockwise rotation
std::unique_ptr<Image> BmpImage::rotateCW() const
{
    uint32_t oldW = getWidth();
    uint32_t oldH = getHeight();

    auto newImg = std::make_unique<BmpImage>(); //empty copy of picture
    newImg->header = header;
    newImg->header.updateForRotation(oldH, oldW); //change width and height

    uint32_t newRowSize = newImg->calculateRowSize();
    newImg->pixelData.assign(newRowSize * oldW, 0);

    for (uint32_t y = 0; y < oldH; y++)
    {
        for (uint32_t x = 0; x < oldW; x++)
        {
            uint8_t r, g, b;
            getPixel(x, y, r, g, b);

            uint32_t newX = oldH - 1 - y;
            uint32_t newY = x;
            newImg->setPixel(newX, newY, r, g, b);
        }
    }
    return newImg;
}

//Counter Clockwise rotation
std::unique_ptr<Image> BmpImage::rotateCCW() const
{
    uint32_t oldW = getWidth();
    uint32_t oldH = getHeight();

    auto newImg = std::make_unique<BmpImage>(); //empty copy of picture
    newImg->header = header;
    newImg->header.updateForRotation(oldH, oldW); //change width and height

    uint32_t newRowSize = newImg->calculateRowSize();
    newImg->pixelData.assign(newRowSize * oldW, 0);

    for (uint32_t y = 0; y < oldH; y++)
    {
        for (uint32_t x = 0; x < oldW; x++)
        {
            uint8_t r, g, b;
            getPixel(x, y, r, g, b);

            uint32_t newX = y;
            uint32_t newY = oldW - 1 - x;
            newImg->setPixel(newX, newY, r, g, b);
        }
    }
    return newImg;
}


//Gaussian Blur
void BmpImage::gaussianBlur()
{
    uint32_t w = getWidth();
    uint32_t h = getHeight();

    if (w < 3 || h < 3)
    {
        std::cout << "picture is too small for 3x3 kernel\n";
        return;
    }

    //gaussian kernel
    static const float kernel[3][3] =
    {
        {1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };

    std::vector<uint8_t> newData = pixelData;
    uint32_t rowSize = calculateRowSize();

    for (uint32_t y = 0; y < h; y++)
    {
        for (uint32_t x = 0; x < w; x++)
        {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {

                    int nx = static_cast<int>(x) + dx;
                    int ny = static_cast<int>(y) + dy;

                    if (nx < 0) nx = 0;
                    if (nx >= static_cast<int>(w)) nx = w - 1;
                    if (ny < 0) ny = 0;
                    if (ny >= static_cast<int>(h)) ny = h - 1;

                    uint8_t r, g, b;
                    getPixel(nx, ny, r, g, b);
                    float weight = kernel[dy + 1][dx + 1];
                    sumR += r * weight;
                    sumG += g * weight;
                    sumB += b * weight;
                }
            }
            newData[y * rowSize + x * 3]     = static_cast<uint8_t>(std::clamp(sumB, 0.0f, 255.0f));
            newData[y * rowSize + x * 3 + 1] = static_cast<uint8_t>(std::clamp(sumG, 0.0f, 255.0f));
            newData[y * rowSize + x * 3 + 2] = static_cast<uint8_t>(std::clamp(sumR, 0.0f, 255.0f));
        }
    }
    pixelData = std::move(newData);
}


uint32_t BmpImage::calculateRequiredMemory(uint32_t width, uint32_t height)
{
    uint32_t rowSize = ((width * 3 + 3) / 4) * 4;
    return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + rowSize * height;
}
