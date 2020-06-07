#include<array>
#include<cmath>
#include<iostream>
#include<string>

#include<glm/glm.hpp>

#include "../include/save_utils.h"

const char* OUTPUT_FILE_PATH = "./test.ppm";

typedef glm::vec3 (*PixelFunc)(float, float);

typedef struct {
    glm::vec3 origin;
    glm::vec3 direction;
} Ray;

typedef struct {
    glm::vec3 eye;
    glm::vec3 f;
    glm::vec3 u;
    glm::vec3 r;
} Camera;

/**
 * Apply a function to each pixel in an input image.
 * PixelFunc is passed coordinates in [0, 1] with the origin
 * at bottom left of the image.
 * @param image - 2D array of glm::vec3 representing image RGB
 * @param width - Image width (axis 1 of image)
 * @param height - Image height (axis 0 of image)
 * @param f - Function (uvX, uvY) -> pixelRgb
 */
void pixelShade(
    glm::vec3 **image, 
    const int width, 
    const int height, 
    const PixelFunc f
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

glm::vec3 sphereSdf(float x, float y) {
    x = (x * 2.0f) - 1.0f;
    y = (y * 2.0f) - 1.0f;
    float c = sqrt((x*x) + (y*y)) - 0.5f;
    return glm::vec3(c, c, c);
}

int main() {
    int width = 50;
    int height = 50;

    // Initialize image
    std::cout << "Initialize image with shape (" << width << ", " << height << ")" << std::endl;
    glm::vec3** image;
    image = new glm::vec3*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new glm::vec3[width];
    }

    pixelShade(image, width, height, sphereSdf);

    // Save output image
    std::string ppmString = SaveUtils::rgbToPpm(image, width, height);
    SaveUtils::writePpm(OUTPUT_FILE_PATH, ppmString);

    return 0;
}
