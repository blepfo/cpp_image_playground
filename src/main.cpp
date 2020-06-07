#include<array>
#include<cmath>
#include<functional>
#include<iostream>
#include<string>

#include<glm/glm.hpp>

#include "../include/save_utils.h"
#include "../include/Intersectables.h"
#include "../include/Raytracing.h"
#include "../include/SimpleLights.h"

const char* OUTPUT_FILE_PATH = "./test.ppm";

// Want to support function pointer for basic functions operating on (uvX, uvY)
// Also want to support lambdas capturing e.g. Camera
typedef std::function<glm::vec3 (float, float)> PixelFunc;


typedef struct {
    glm::vec3 eye;
    glm::vec3 f;    // Forward
    glm::vec3 u;    // Up
    glm::vec3 r;    // Right
    glm::vec3 imagePlaneCenter;
} Camera;

/* Simple Material */

typedef struct {
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    glm::vec3 emission;
} SimpleMaterial;


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

Camera makeCamera(
    glm::vec3 eye, 
    glm::vec3 lookAt, 
    glm::vec3 up,
    float fovDegrees
) {
    glm::vec3 f = glm::normalize(eye - lookAt);
    glm::vec3 r = glm::normalize(glm::cross(up, f));
    glm::vec3 u = glm::cross(f, r);
    float fovMultiplier = 1.0f / tan(glm::radians(fovDegrees / 2.0f));
    glm::vec3 imagePlaneCenter = eye + (f * fovMultiplier);
    Camera camera = { eye, f, u, r, imagePlaneCenter };
    return camera;
}

/** 
 * @param camera - Camera 
 * @param fovDegrees
 * @param uvX - X coordinate of image in [-1, 1]
 * @param uvY - Y coordinate of image in [-1, 1]
 */
Raytracing::Ray cameraViewRay(Camera camera, float uvX, float uvY) {
    glm::vec3 imagePlaneIntersection = camera.imagePlaneCenter
        + (camera.r * uvX)
        + (camera.u * uvY);
    glm::vec3 direction = glm::normalize(camera.eye - imagePlaneIntersection);
    Raytracing::Ray ray = { camera.eye, direction };
    return ray;
}

// TODO - Accept Scene containing objects + lights
glm::vec3 whittedRayTrace(Camera camera, float uvX, float uvY) {
    // Remap uvs into [-1, 1]^2
    uvX = (uvX * 2.0f) - 1.0f;
    uvY = (uvY * 2.0f) - 1.0f;
    Raytracing::Ray viewRay = cameraViewRay(camera, uvX, uvY);

    // DEFINE GEOMETRY
    Intersectables::Sphere sphere = { 0.5f, glm::vec3(0.0f, 0.0f, 0.0f) };

    // DEFINE LIGHTING
    SimpleLights::PointLight pointLight = {
        glm::vec3(1.5f, 2.0f, -0.5f),   // Origin
        glm::vec3(1.0f, 0.0f, 0.0f),    // Attenuation
        glm::vec3(1.0f, 1.0f, 1.0f),    // Intensity
        glm::vec3(0.0f, 0.0f, 0.0f),    // Ambient
    };

    // Intersect scene
    // TODO 
    // intersect(ray, scene)
    Raytracing::HitInfo sphereHit = sphere.intersect(viewRay);

    if (sphereHit.t > 0) {
        // Compute lighting at intersection
        glm::vec3 illumination = pointLight.illuminate(sphereHit, camera.eye);
        return illumination;
    } else {
        // Background color
        return glm::vec3(0.0f, 0.0f, 1.0f);
    }


    // Shadow Ray

    // Reflection Ray

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

    Camera camera = makeCamera(
        glm::vec3(0.0f, 0.0f, 1.0f), 
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        60.0f
    );

    PixelFunc traceFunc = [camera](float uvX, float uvY) {
        return whittedRayTrace(camera, uvX, uvY);
    };

    pixelShade(image, width, height, traceFunc);

    // Save output image
    std::string ppmString = SaveUtils::rgbToPpm(image, width, height);
    SaveUtils::writePpm(OUTPUT_FILE_PATH, ppmString);

    return 0;
}
