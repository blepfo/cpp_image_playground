#pragma once

#include<string>
#include<glm/glm.hpp>

namespace SaveUtils {

std::string rgbToPpm(glm::dvec3** rgbImage, const int width, const int height);
void writePpm(const char* outPath, std::string ppmString);

} // namespace SaveUtils
