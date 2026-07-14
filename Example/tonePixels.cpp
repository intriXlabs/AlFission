#include "../Include/colorEstimation.cpp"
#include <iostream>
#include <vector>
#include <iomanip>

void printColors(const std::vector<colorEstimation::RGB>& colors, const std::string& label) {
    std::cout << label << ":" << std::endl;
    for (size_t i = 0; i < colors.size(); i++) {
        std::cout << "  [" << i << "] RGB(" 
                  << std::setw(3) << (int)colors[i].r << ", "
                  << std::setw(3) << (int)colors[i].g << ", "
                  << std::setw(3) << (int)colors[i].b << ")";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

int main() {
    colorEstimation ce;
    
    // Test pixels: a mix of different colors
    std::vector<colorEstimation::RGB> testPixels = {
        {255, 0, 0},     // Pure red
        {0, 255, 0},     // Pure green
        {0, 0, 255},     // Pure blue
        {255, 255, 0},   // Yellow
        {128, 128, 128}, // Gray
        {255, 200, 150}, // Skin tone
        {50, 180, 220},  // Sky blue
        {200, 100, 50}   // Orange-brown
    };
    
    printColors(testPixels, "Original");
    
    // Apply WARM tone at 50% strength
    auto warm = ce.tonePixels(testPixels, colorEstimation::Tone::WARM, 50.0f);
    printColors(warm, "WARM (50%)");
    
    // Apply COLD tone at 75% strength
    auto cold = ce.tonePixels(testPixels, colorEstimation::Tone::COLD, 75.0f);
    printColors(cold, "COLD (75%)");
    
    // Apply GOLDEN tone at 80% strength
    auto golden = ce.tonePixels(testPixels, colorEstimation::Tone::GOLDEN, 80.0f);
    printColors(golden, "GOLDEN (80%)");
    
    // Apply NOIR tone at 100% strength
    auto noir = ce.tonePixels(testPixels, colorEstimation::Tone::NOIR, 100.0f);
    printColors(noir, "NOIR (100%)");
    
    // Apply NEON tone at 60% strength
    auto neon = ce.tonePixels(testPixels, colorEstimation::Tone::NEON, 60.0f);
    printColors(neon, "NEON (60%)");
    
    // Apply VINTAGE tone at 70% strength
    auto vintage = ce.tonePixels(testPixels, colorEstimation::Tone::VINTAGE, 70.0f);
    printColors(vintage, "VINTAGE (70%)");
    
    // Apply CYBER tone at 90% strength
    auto cyber = ce.tonePixels(testPixels, colorEstimation::Tone::CYBER, 90.0f);
    printColors(cyber, "CYBER (90%)");
    
    // Demonstrate strength range
    std::vector<colorEstimation::RGB> singlePixel = {{100, 150, 200}};
    std::cout << "Strength demonstration on RGB(100, 150, 200):" << std::endl;
    for (int strength = 0; strength <= 100; strength += 25) {
        auto result = ce.tonePixels(singlePixel, colorEstimation::Tone::SUNSET, (float)strength);
        std::cout << "  " << strength << "%: RGB(" 
                  << (int)result[0].r << ", " 
                  << (int)result[0].g << ", " 
                  << (int)result[0].b << ")" << std::endl;
    }
    
    return 0;
}