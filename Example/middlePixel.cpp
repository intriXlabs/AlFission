#include "../Include/colorEstimation.cpp"
#include <iostream>

int main() {
    colorEstimation ce;
    
    // Middle between black and white
    auto mid1 = ce.middlePixel({0, 0, 0}, {255, 255, 255});
    std::cout << "Black + White = RGB(" 
              << (int)mid1.r << ", " << (int)mid1.g << ", " << (int)mid1.b << ")" << std::endl;
    
    // Middle between red and blue
    auto mid2 = ce.middlePixel({255, 0, 0}, {0, 0, 255});
    std::cout << "Red + Blue = RGB(" 
              << (int)mid2.r << ", " << (int)mid2.g << ", " << (int)mid2.b << ")" << std::endl;
    
    // Middle between two greens
    auto mid3 = ce.middlePixel({0, 100, 0}, {0, 200, 0});
    std::cout << "Dark green + Light green = RGB(" 
              << (int)mid3.r << ", " << (int)mid3.g << ", " << (int)mid3.b << ")" << std::endl;
    
    // Middle of identical colors
    auto mid4 = ce.middlePixel({128, 64, 32}, {128, 64, 32});
    std::cout << "Same colors = RGB(" 
              << (int)mid4.r << ", " << (int)mid4.g << ", " << (int)mid4.b << ")" << std::endl;
    
    return 0;
}