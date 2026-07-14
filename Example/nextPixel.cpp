#include "../Include/colorEstimation.cpp"
#include <iostream>
#include <vector>

int main() {
    colorEstimation ce;
    
    // Example 1: Linear prediction with 2 pixels
    std::vector<colorEstimation::RGB> history1 = {
        {100, 100, 100},
        {150, 150, 150}
    };
    auto next1 = ce.nextPixel(history1);
    std::cout << "Linear (2 pixels): RGB(" 
              << (int)next1.r << ", " << (int)next1.g << ", " << (int)next1.b << ")" << std::endl;
    
    // Example 2: Acceleration prediction with 5 pixels (accelerating red)
    std::vector<colorEstimation::RGB> history2 = {
        {100, 50, 200},
        {120, 55, 195},
        {145, 62, 188},
        {175, 71, 179},
        {210, 82, 168}
    };
    auto next2 = ce.nextPixel(history2);
    std::cout << "Accelerating sequence: RGB(" 
              << (int)next2.r << ", " << (int)next2.g << ", " << (int)next2.b << ")" << std::endl;
    
    // Example 3: Decelerating green
    std::vector<colorEstimation::RGB> history3 = {
        {50, 200, 50},
        {60, 170, 60},
        {65, 150, 65},
        {68, 138, 68}
    };
    auto next3 = ce.nextPixel(history3);
    std::cout << "Decelerating sequence: RGB(" 
              << (int)next3.r << ", " << (int)next3.g << ", " << (int)next3.b << ")" << std::endl;
    
    // Example 4: Empty/edge cases
    std::vector<colorEstimation::RGB> empty;
    auto next4 = ce.nextPixel(empty);
    std::cout << "Empty history: RGB(" 
              << (int)next4.r << ", " << (int)next4.g << ", " << (int)next4.b << ")" << std::endl;
    
    return 0;
}