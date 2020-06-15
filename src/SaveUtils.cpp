#include<fstream>
#include<iostream>
#include<sstream>
#include<string>

#include<glm/glm.hpp>

#include <FreeImage.h>

// TODO
#include "../SaveUtils.hpp"


namespace SaveUtils {

/** 
 * Save image from 2D array of glm::vec3 to PNG.
 * http://graphics.stanford.edu/courses/cs148-10-summer/docs/UsingFreeImage.pdf
 */
void savePng(
    glm::dvec3** rgbImage,
    const int width,
    const int height,
    const char* savePath 
) {
    // 24 bits per pixel = 8 bits * 3 channels
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
    if (!bitmap) {
        throw std::runtime_error("Failed to allocate FreeImage bitmap");
    }
    RGBQUAD color;
    
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            glm::dvec3 rgb = glm::clamp(rgbImage[i][j], 0.0, 1.0);
            color.rgbRed = int(rgb[0] * 255.0);
            color.rgbGreen = int(rgb[1] * 255.0);
            color.rgbBlue = int(rgb[2] * 255.0);
            // rgbImage needs vertical flip to have origin at bottom left
            FreeImage_SetPixelColor(bitmap, j, height - 1 - i, &color);
            
        }
    }
    if (FreeImage_Save(FIF_PNG, bitmap, savePath, 0)) {
        std::cout << "SaveUtils::savePng() success to " << savePath << std::endl;
    }
}


void savePpm(
    glm::dvec3** rgbImage,
    const int width,
    const int height,
    const char* savePath 
) {
    const std::string ppmString = rgbToPpm(rgbImage, width, height);
    writePpm(savePath, ppmString);
}


/** 
 * Convert RGB array defined as 2D array of glm::vec3 to PPM string.
 * http://netpbm.sourceforge.net/doc/ppm.html
 * http://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf
 * @param rgbImage - 2D array of vec3 RGB
 * @param width - Image width (rgbImage axis 1)
 * @param height - Image height (rgbImage axis 0)
 */
std::string rgbToPpm(
    glm::dvec3** rgbImage, 
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
            glm::dvec3 rgb = glm::clamp(rgbImage[i][j], 0.0, 1.0);
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
    std::cout << "SaveUtils::writePpm() success to " << outPath << std::endl;
}

}  // namespace SaveUtils
