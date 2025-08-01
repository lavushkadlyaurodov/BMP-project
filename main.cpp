#include "BMPImage.h"
#include <iostream>
#include <string>
#include <exception>
#include <limits>

int main() {
    BMPImage image;
    std::string inputFile, outputFile;

    while (true) {
        std::cout << ">> Enter input BMP file name: ";
        if (!std::getline(std::cin, inputFile)) {
            std::cerr << "Input error. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (image.load(inputFile)) {
            break;
        } else {
            std::cerr << "Error: Failed to load BMP file '" << inputFile << "'. Please try again.\n";
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    std::cout << "\nOriginal image:\n";
    image.display();

    int w = image.getWidth();
    int h = image.getHeight();
    image.drawline(0, 0, w - 1, h - 1);        
    image.drawline(0, h - 1, w - 1, 0);        

    std::cout << "\nImage with X drawn:\n";
    image.display();

    std::cout << ">> Enter output BMP file name: ";
    std::getline(std::cin, outputFile);

    if (!image.save(outputFile)) {
        std::cerr << "Error: Failed to save file '" << outputFile << "'\n";
        return 1;
    }

    std::cout << "Saved to " << outputFile << "\n";
    return 0;
}
