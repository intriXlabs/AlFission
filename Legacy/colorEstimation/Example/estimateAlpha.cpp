#include "../Include/colorEstimation.cpp"
#include <iostream>
#include <iomanip>

int main() {
    colorEstimation ce;
    
    // Estimate alpha of red text on white background
    uint8_t alpha = ce.estimateAlpha(255, 0, 0, 255, 255, 255);
    std::cout << "Red on white alpha: " << (int)alpha << "/255" << std::endl;
    
    // Estimate alpha of black text on white background
    alpha = ce.estimateAlpha(0, 0, 0, 255, 255, 255);
    std::cout << "Black on white alpha: " << (int)alpha << "/255" << std::endl;
    
    // Estimate alpha of similar colors
    alpha = ce.estimateAlpha(200, 200, 200, 210, 210, 210);
    std::cout << "Similar colors alpha: " << (int)alpha << "/255" << std::endl;
    
    // Estimate alpha of identical colors
    alpha = ce.estimateAlpha(128, 128, 128, 128, 128, 128);
    std::cout << "Identical colors alpha: " << (int)alpha << "/255" << std::endl;
    
    return 0;
}