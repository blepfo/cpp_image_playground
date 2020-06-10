#include <glm/glm.hpp>

// TODO
#include "PixelDraw.hpp"

namespace PixelDraw {

void pixelShade(
    glm::vec3 **image, 
    const int width, 
    const int height, 
    const PixelDraw::PixelFunc f
) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Remap (i, j) into unit square
            float uvX = (float)j / ((float)(width - 1));
            // We want y=0 at bottom of image
            float uvY = (float)(height - 1 - i) / ((float)(height - 1));
            image[i][j] = f(uvX, uvY);
        }
    }
}

} // PixelDraw
