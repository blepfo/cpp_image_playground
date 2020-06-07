#ifndef _SAVE_UTILS_H
#define _SAVE_UTILS_H

#include<string>
#include<glm/glm.hpp>

namespace SaveUtils {

std::string rgbToPpm(glm::vec3** rgbImage, const int width, const int height);
void writePpm(const char* outPath, std::string ppmString);

} // namespace SaveUtils

#endif
