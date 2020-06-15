#pragma once

#include<string>
#include<glm/glm.hpp>

namespace SaveUtils {

/**
 * Save image from 2D array of glm::dvec3 to PNG
 */
void savePng(glm::dvec3** rgbImage, const int width, const int height, const char* savePath);


/**
 * Save image from 2D array of glm::dvec3 to PPM
 */
void savePpm(glm::dvec3** rgbImage, const int width, const int height, const char* savePath);

std::string rgbToPpm(glm::dvec3** rgbImage, const int width, const int height);
void writePpm(const char* outPath, std::string ppmString);

} // namespace SaveUtils
