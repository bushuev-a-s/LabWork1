/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include <exception>

#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <cstdint>
#include <memory> // use it to creat smart pointer

class Image
{
public:
    virtual ~Image() = default;

    //pure virtual methods
    virtual bool load(const std::string& filename) = 0;
    virtual bool save(const std::string& filename) const = 0;

    virtual std::unique_ptr<Image> rotateCW() const = 0; // Clockwise rotation
    virtual std::unique_ptr<Image> rotateCCW() const = 0; // Counter Clockwise rotation
    virtual void gaussianBlur() = 0;

    //virtual getters
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual uint32_t getSize() const = 0;
};

#endif
