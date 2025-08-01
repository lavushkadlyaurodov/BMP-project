#pragma once
#include <string>
#include <vector>
#include <exception>

class BMPImage{
public:
    BMPImage();
    ~BMPImage();

    bool load(const std::string& filename);
    void display() const;
    void drawline(int x1, int y1, int x2, int y2);
    bool save(const std::string& filename);
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width, height;
    int bitcount;
    int rowsize;
    std::vector<unsigned char> data;
    std::vector<unsigned char> fileHeader; //файловый заголовок BMP
    std::vector<unsigned char> infoHeader; // байты BMP

    int getIndex(int x, int y) const;
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);

};
