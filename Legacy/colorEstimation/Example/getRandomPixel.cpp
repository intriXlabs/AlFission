#include "../Include/colorEstimation.cpp"
#include <iostream>
#include <iomanip>

int main() {
    colorEstimation ce;
    
    std::cout << "Generating 10 random pixels:" << std::endl;
    std::cout << "------------------------" << std::endl;
    
    for (int i = 0; i < 10; i++) {
        auto pixel = ce.getRandomPixel();
        std::cout << "Pixel " << i+1 << ": RGB(" 
                  << std::setw(3) << (int)pixel.r << ", "
                  << std::setw(3) << (int)pixel.g << ", "
                  << std::setw(3) << (int)pixel.b << ")  Hex: #"
                  << std::hex << std::setfill('0')
                  << std::setw(2) << (int)pixel.r
                  << std::setw(2) << (int)pixel.g
                  << std::setw(2) << (int)pixel.b
                  << std::dec << std::setfill(' ') << std::endl;
    }
    
    return 0;
}