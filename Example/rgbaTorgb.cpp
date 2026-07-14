#include "../Include/colorEstimation.cpp"
#include <iostream>

int main() {
    colorEstimation ce;
    
    // Semi-transparent red on white background
    auto result = ce.rgbaToRgb(255, 0, 0, 128, 255, 255, 255);
    std::cout << "50% red on white: RGB(" 
              << (int)result.r << ", " << (int)result.g << ", " << (int)result.b << ")" << std::endl;
    
    // Fully opaque red on white
    result = ce.rgbaToRgb(255, 0, 0, 255, 255, 255, 255);
    std::cout << "100% red on white: RGB(" 
              << (int)result.r << ", " << (int)result.g << ", " << (int)result.b << ")" << std::endl;
    
    // Fully transparent on black
    result = ce.rgbaToRgb(0, 255, 0, 0, 0, 0, 0);
    std::cout << "0% green on black: RGB(" 
              << (int)result.r << ", " << (int)result.g << ", " << (int)result.b << ")" << std::endl;
    
    // Semi-transparent blue on red background
    result = ce.rgbaToRgb(0, 0, 255, 64, 255, 0, 0);
    std::cout << "25% blue on red: RGB(" 
              << (int)result.r << ", " << (int)result.g << ", " << (int)result.b << ")" << std::endl;
    
    return 0;
}