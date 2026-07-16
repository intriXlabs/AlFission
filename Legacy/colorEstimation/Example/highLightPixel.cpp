#include "../Include/colorEstimation.cpp"
#include <iostream>
#include <vector>

int main() {
    colorEstimation ce;
    
    // Start with a mid-gray pixel in history
    std::vector<colorEstimation::RGB> history = {{128, 128, 128}};
    
    // Darken to 25%
    auto dark = ce.highlightPixel(history, {128, 128, 128}, 25.0f);
    std::cout << "25% (darkened): RGB(" 
              << (int)dark.r << ", " << (int)dark.g << ", " << (int)dark.b << ")" << std::endl;
    
    // Original (50%)
    history = {{128, 128, 128}};
    auto mid = ce.highlightPixel(history, {128, 128, 128}, 50.0f);
    std::cout << "50% (original): RGB(" 
              << (int)mid.r << ", " << (int)mid.g << ", " << (int)mid.b << ")" << std::endl;
    
    // Lighten to 75%
    history = {{128, 128, 128}};
    auto light = ce.highlightPixel(history, {128, 128, 128}, 75.0f);
    std::cout << "75% (lightened): RGB(" 
              << (int)light.r << ", " << (int)light.g << ", " << (int)light.b << ")" << std::endl;
    
    // Full brightness variations
    history = {{100, 50, 200}};
    auto fullDark = ce.highlightPixel(history, {100, 50, 200}, 0.0f);
    std::cout << "0% (black): RGB(" 
              << (int)fullDark.r << ", " << (int)fullDark.g << ", " << (int)fullDark.b << ")" << std::endl;
    
    history = {{100, 50, 200}};
    auto fullLight = ce.highlightPixel(history, {100, 50, 200}, 100.0f);
    std::cout << "100% (white): RGB(" 
              << (int)fullLight.r << ", " << (int)fullLight.g << ", " << (int)fullLight.b << ")" << std::endl;
    
    return 0;
}