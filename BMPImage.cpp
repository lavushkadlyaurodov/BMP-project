#include <exception>
#include <stdexcept>
#include "BMPImage.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint>


BMPImage::BMPImage() : width(0), height(0), bitcount(0), rowsize(0) {}
BMPImage::~BMPImage() {}

bool BMPImage::load(const std::string& filename){
    std::ifstream in(filename, std::ios::binary);

    if (!in){
        std::cerr << "Error: Cannot open file\n";
        return false;
    }


    fileHeader.resize(14);
    in.read(reinterpret_cast<char*>(fileHeader.data()), 14);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M'){
        std::cerr << "Error: Not a BMP file\n";
        return false;
    }

    infoHeader.resize(40);
    in.read(reinterpret_cast<char*>(infoHeader.data()), 40);

    width = *reinterpret_cast<int*>(&infoHeader[4]);
    height = *reinterpret_cast<int*>(&infoHeader[8]);
    bitcount = *reinterpret_cast<uint16_t*>(&infoHeader[14]);

    if (bitcount != 24 && bitcount != 32) {
        std::cerr << "Error: Only 24 or 32-bit BMP supported\n";
        return false;
    }

    rowsize = ((width * bitcount + 31) / 32) * 4;
    int datasize = rowsize * height;
    data.resize(datasize);

    in.seekg(*reinterpret_cast<int*>(&fileHeader[10]), std::ios::beg);
    in.read(reinterpret_cast<char*>(data.data()), datasize);
    in.close();

    return true;
}


void BMPImage::display() const {
    if (height > 0) {
        // стандартный порядок BMP, снизу вверх
        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                int i = getIndex(x, y);
                unsigned char b = data[i];
                unsigned char g = data[i + 1];
                unsigned char r = data[i + 2];

                if (r == 0 && g == 0 && b == 0)
                    std::cout << "#";
                else if (r == 255 && g == 255 && b == 255)
                    std::cout << " ";
                else
                    std::cout << "?";
            }
            std::cout << "\n";
        }
    } else {
        // высота отрицательная, читаем сверху вниз
        for (int y = 0; y < -height; ++y) {
            for (int x = 0; x < width; ++x) {
                int i = getIndex(x, y);
                unsigned char b = data[i];
                unsigned char g = data[i + 1];
                unsigned char r = data[i + 2];

                if (r == 0 && g == 0 && b == 0)
                    std::cout << "#";
                else if (r == 255 && g == 255 && b == 255)
                    std::cout << " ";
                else
                    std::cout << "?";
            }
            std::cout << "\n";
        }
    }
}


int BMPImage::getIndex(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::out_of_range("Coordinates out of image bounds");
    return y * rowsize + x * (bitcount / 8);
}


void BMPImage::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    int i = getIndex(x, y);
    data[i]     = b;
    data[i + 1] = g;
    data[i + 2] = r;
    if (bitcount == 32)
        data[i + 3] = 255;
}

void BMPImage::drawline(int x1, int y1, int x2, int y2) {
    // Алгоритм Брезенхема для пикселей
    int dx = std::abs(x2 - x1); // дельта x
    int dy = -std::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1; 
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy; // функция ошибки

    while (true) {
        setPixel(x1, y1, 0, 0, 0); // черный
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; } // если не превышает вдвое ошибку, то добавляем к ошибке еще дельту, а к иксу еще +-1 сдвиг
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

bool BMPImage::save(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Error: Cannot write file.\n";
        return false;
    }

    out.write(reinterpret_cast<char*>(fileHeader.data()), fileHeader.size());
    out.write(reinterpret_cast<char*>(infoHeader.data()), infoHeader.size());
    out.write(reinterpret_cast<char*>(data.data()), data.size());
    out.close();

    return true;
}