#include<fstream>
#include<iostream>
#include<sstream>
#include<string>

#include<glm/glm.hpp>

#include "../include/save_utils.h"


namespace SaveUtils {

/** 
 * Convert RGB array defined as 2D array of glm::vec3 to PPM string.
 * @param rgbImage - 2D array of vec3 RGB
 * @param width - Image width (rgbImage axis 1)
 * @param height - Image height (rgbImage axis 0)
 */
std::string rgbToPpm(
    glm::vec3** rgbImage, 
    const int width, 
    const int height
) {
    std::stringstream ppmStream;
    // PPM Header
    ppmStream 
        // P3 == numbers are written in ASCII
        << "P3\n"
        // Image dims
        << width << " " << height << "\n"
        // Max color val 
        << 255 << "\n";
    // Write RGB values
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            glm::vec3 rgb = glm::clamp(rgbImage[i][j], 0.0f, 1.0f);
            int r = int(rgb[0] * 255.99);
            int g = int(rgb[1] * 255.99);
            int b = int(rgb[2] * 255.99);
            ppmStream << r << " " << g << " " << b << " ";
        }
        ppmStream << "\n";
    }
    return ppmStream.str();
}

void writePpm(const char* outPath, std::string ppmString) {
    std::ofstream outFile(outPath);
    outFile << ppmString;
    outFile.close();
    std::cout << "writePpm success to " << outPath << std::endl;
}

}  // namespace SaveUtils
