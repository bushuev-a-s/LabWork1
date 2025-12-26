/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include <gtest/gtest.h>
#include "BMP.h"
#include <filesystem>
#include <fstream>

// extra function for test bmp file
void createTestBmp(const std::string& filename, uint32_t width, uint32_t height)
{
    std::ofstream file(filename, std::ios::binary);
    BITMAPFILEHEADER BmpFile;
    BITMAPINFOHEADER BmpInfo;

    uint32_t rowSize = ((width * 3 + 3) / 4) * 4;
    uint32_t imageSize = rowSize * height;

    BmpFile.bfType = 0x4D42;
    BmpFile.bfSize = 54 + imageSize;
    BmpFile.bfOffset = 54;

    BmpInfo.biSize = 40;
    BmpInfo.biWidth = width;
    BmpInfo.biHeight = height;
    BmpInfo.biPlanes = 1;
    BmpInfo.biBitCount = 24;
    BmpInfo.biCompression = 0;
    BmpInfo.biSizeImage = imageSize;

    file.write(reinterpret_cast<char*>(&BmpFile), 14);
    file.write(reinterpret_cast<char*>(&BmpInfo), 40);

    std::vector<uint8_t> pixels(imageSize, 0);

    for (uint32_t y = 0; y < height; ++y)
    {
        uint32_t bmpY = height - 1 - y;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint32_t index = bmpY * rowSize + x * 3;
            pixels[index + 0] = 0; // B
            pixels[index + 1] = 0; // G
            pixels[index + 2] = uint8_t((x * 255) / width); // R
        }
    }

    file.write(reinterpret_cast<char*>(pixels.data()), imageSize);
}


TEST(BmpImageTest, MemoryCalculation)
{
    uint32_t width = 100;
    uint32_t height = 100;

    uint32_t expectedMemory = BmpImage::calculateRequiredMemory(width, height);
    uint32_t rowSize = ((width * 3 + 3) / 4) * 4;
    uint32_t expected = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + rowSize * height;

    EXPECT_EQ(expectedMemory, expected);
}

TEST(BmpImageTest, LoadSave)
{
    const std::string testFile = "test.bmp";
    const std::string savedFile = "saved.bmp";

    createTestBmp(testFile, 10, 10);

    BmpImage image;
    EXPECT_TRUE(image.load(testFile));
    EXPECT_EQ(image.getWidth(), 10);
    EXPECT_EQ(image.getHeight(), 10);
    EXPECT_TRUE(image.save(savedFile));

    std::ifstream saved(savedFile, std::ios::binary | std::ios::ate);
    EXPECT_TRUE(saved.is_open());
    saved.close();

    std::filesystem::remove(testFile);
    std::filesystem::remove(savedFile);
}

TEST(BmpImageTest, RotateClockwise)
{
    const std::string testFile = "rotate_test.bmp";
    createTestBmp(testFile, 4, 3);

    BmpImage image;
    image.load(testFile);

    auto rotated = image.rotateCW();
    EXPECT_EQ(rotated->getWidth(), 3);
    EXPECT_EQ(rotated->getHeight(), 4);

    EXPECT_TRUE(rotated->save("rotated_cw.bmp"));

    std::filesystem::remove(testFile);
    std::filesystem::remove("rotated_cw.bmp");
}

TEST(BmpImageTest, RotateCounterClockwise)
{
    const std::string testFile = "rotate_ccw_test.bmp";
    createTestBmp(testFile, 3, 4);

    BmpImage image;
    image.load(testFile);

    auto rotated = image.rotateCCW();
    EXPECT_EQ(rotated->getWidth(), 4);
    EXPECT_EQ(rotated->getHeight(), 3);

    auto doubleRotated = rotated->rotateCCW();
    auto quadRotated = doubleRotated->rotateCCW();
    auto originalSize = quadRotated->rotateCCW();

    EXPECT_EQ(originalSize->getWidth(), 3);
    EXPECT_EQ(originalSize->getHeight(), 4);

    std::filesystem::remove(testFile);
}

TEST(BmpImageTest, GaussianBlur)
{
    const std::string testFile = "blur_test.bmp";
    createTestBmp(testFile, 5, 5);

    BmpImage image;
    image.load(testFile);

    uint8_t r1, g1, b1;
    image.getPixel(2, 2, r1, g1, b1);

    image.gaussianBlur();

    uint8_t r2, g2, b2;
    image.getPixel(2, 2, r2, g2, b2);

    EXPECT_TRUE(image.save("blurred.bmp"));
    std::filesystem::remove(testFile);
    std::filesystem::remove("blurred.bmp");
}

TEST(BmpImageTest, PixelAccess)
{
    BmpImage image;
    image = BmpImage();
    SUCCEED();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
