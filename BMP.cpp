/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include "BMP.h"
#include <vector>
#include <utility>

void Bmp::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    file.read(reinterpret_cast<char*>(&_fileheader), sizeof(_fileheader));
    file.read(reinterpret_cast<char*>(&_infoheader), sizeof(_infoheader));

    int rowLength = (_infoheader.biWidth * _infoheader.biBitCount / 8 + 3) & ~3;
    int totalSize = rowLength * abs(_infoheader.biHeight);
    pixels.resize(totalSize);
    file.seekg(fileHeader.bfOffBits);
    file.read(reinterpret_cast<char*>(pixels.data()), totalSize);
}

void Bmp::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(&_fileheader), sizeof(_fileheader));
    file.write(reinterpret_cast<const char*>(&_infoheader), sizeof(_infoheader));

    int rowLength = (_infoheader.biWidth * 3 + 3) & ~3;
    int paddingSize = rowLength - _infoheader.biWidth * 3;
    for (int y = 0; y < abs(_infoheader.biHeight); ++y) {
        file.write(reinterpret_cast<const char*>(pixels.data() + y * _infoheader.biWidth), _infoheader.biWidth * 3);
        char padding[paddingSize]{};
        file.write(padding, paddingSize);
    }
}

void Bmp::rotateRight() {
    std::vector<uint8_t> temp(_pixels.size());
    for (int y = 0; y < _infoheader.biHeight; ++y) {
        for (int x = 0; x < _infoheader.biWidth; ++x) {
            int oldIndex = (y * _infoheader.biWidth + x) * 3;
            int newY = x;
            int newX = _infoheader.biHeight - 1 - y;
            int newIndex = (newY * _infoheader.biWidth + newX) * 3;

            temp[newIndex] = _pixels[oldIndex];
            temp[newIndex + 1] = _pixels[oldIndex + 1];
            temp[newIndex + 2] = _pixels[oldIndex + 2];
        }
    }

    std::swap(_pixels,temp);
    std::swap(_infoheader.biWidth,_infoheader.biHeight);
    std::swap(_infoheader.biXPelsPerMeter,_infoheader.biYPelsPerMeter);
}

void Bmp::rotateLeft() {
    std::vector<uint8_t> temp(_pixels.size());
    for (int y = 0; y < _infoheader.biHeight; ++y) {
        for (int x = 0; x < _infoheader.biWidth; ++x) {
            int oldIndex = (y * _infoheader.biWidth + x) * 3;
            int newY = _infoheader.biWidth - 1 - x;
            int newX = y;
            int newIndex = (newY * _infoheader.biHeight + newX) * 3;

            temp[newIndex] = _pixels[oldIndex];
            temp[newIndex + 1] = _pixels[oldIndex + 1];
            temp[newIndex + 2] = _pixels[oldIndex + 2];
        }
    }

    std::swap(_pixels,temp);
    std::swap(_infoheader.biWidth,_infoheader.biHeight);
    std::swap(_infoheader.biXPelsPerMeter,_infoheader.biYPelsPerMeter);
}

void Bmp::applyGaussianFilter() {
    std::vector<uint8_t> temp(_pixels.size());
    float kernel[3][3] = {{0.0625, 0.125, 0.0625}, {0.125, 0.25, 0.125}, {0.0625, 0.125, 0.0625}};
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float sumR = 0, sumG = 0, sumB = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int nx = x + kx;
                    int ny = y + ky;
                    int index = (ny * width + nx) * 3; // RGB

                    sumR += pixels[index] * kernel[ky + 1][kx + 1];
                    sumG += pixels[index + 1] * kernel[ky + 1][kx + 1];
                    sumB += pixels[index + 2] * kernel[ky + 1][kx + 1];
                }
            }
            int idx = (y * width + x) * 3;
            filteredPixels[idx] = static_cast<uint8_t>(sumR);
            filteredPixels[idx + 1] = static_cast<uint8_t>(sumG);
            filteredPixels[idx + 2] = static_cast<uint8_t>(sumB);
        }
    }
    std::swap(_pixels,temp);
}
