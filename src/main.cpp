#include<array>
#include<cmath>
#include<functional>
#include<iostream>
#include<string>
#include<vector>

#include<glm/glm.hpp>

#include "../include/Raytracing.h"
#include "../include/save_utils.h"
#include "../include/SimpleLights.h"
#include "../include/SimpleMaterial.h"
#include "../include/SimpleObjects.h"
#include "../include/SimpleScene.h"
#include "../include/Transform.h"

const char* OUTPUT_FILE_PATH = "./test.ppm";

// Want to support function pointer for basic functions operating on (uvX, uvY)
// Also want to support lambdas capturing e.g. Camera
typedef std::function<glm::vec3 (float, float)> PixelFunc;


typedef struct {
    glm::vec3 eye;
    glm::vec3 f;    // Forward
    glm::vec3 u;    // Up
    glm::vec3 r;    // Right
    float fovXMultiplier;
    float fovYMultiplier;
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

Camera makeCamera(
    const glm::vec3 eye, 
    const glm::vec3 lookAt, 
    const glm::vec3 up,
    // TODO - separate fovX and fovY
    float fovDegrees,
    float aspectRatio
) {
    glm::vec3 f = glm::normalize(lookAt - eye);
    glm::vec3 r = glm::normalize(glm::cross(f, up));
    glm::vec3 u = glm::cross(r, f);
    float fovYMultiplier = tan(glm::radians(fovDegrees / 2.0f));
    // Each pixel should be square, wo fov = (fovY / height) * width
    // Then (fovX / fovY) = width / height
    float fovXMultiplier = fovYMultiplier * aspectRatio;
    Camera camera = { eye, f, u, r, fovXMultiplier, fovYMultiplier };
    return camera;
}

/** 
 * @param camera - Camera 
 * @param fovDegrees
 * @param uvX - X coordinate of image in [-1, 1]
 * @param uvY - Y coordinate of image in [-1, 1]
 */
Raytracing::Ray cameraViewRay(const Camera& camera, const float uvX, const float uvY) {
    glm::vec3 imagePlaneCenter = camera.eye + camera.f;
    glm::vec3 imagePlaneIntersection = imagePlaneCenter
        + (camera.r * uvX * camera.fovXMultiplier)
        + (camera.u * uvY * camera.fovYMultiplier);
    glm::vec3 direction = glm::normalize(imagePlaneIntersection - camera.eye);
    Raytracing::Ray ray = { camera.eye, direction };
    return ray;
}

glm::vec3 whittedRayTracePixelFunc(
    const Camera& camera, 
    const SimpleScene::Scene& scene,
    float uvX, 
    float uvY 
) {
    // TODO - May need to shift Uvs by 0.5 so rays go through center of pixels instead of corners
    // Remap uvs into [-1, 1]^2
    uvX = (uvX * 2.0f) - 1.0f;
    uvY = (uvY * 2.0f) - 1.0f;
    Raytracing::Ray viewRay = cameraViewRay(camera, uvX, uvY);

    return SimpleScene::whittedRayTrace(
        viewRay, 
        scene,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );
}


int main() {
    int width = 400;
    int height = 300;

    // Initialize image
    std::cout << "Initialize image with shape (" << width << ", " << height << ")" << std::endl;
    glm::vec3** image;
    image = new glm::vec3*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new glm::vec3[width];
    }

    // CAMERA
    Camera camera = makeCamera(
        glm::vec3(0.0f, 0.0f, 1.0f), 
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        60.0f,
        (float)width / (float)height
    );

    // CREATE SCENE
    SimpleScene::Scene scene = SimpleScene::Scene();

    // LIGHTS
    SimpleLights::PointLight pointLight1 = {
        glm::vec3(1.5f, 2.0f, 0.5f),   // Origin
        glm::vec3(1.0f, 0.0f, 0.0f),    // Attenuation
        glm::vec3(0.6f),                // Intensity
        glm::vec3(0.0f, 0.0f, 0.0f),    // Ambient
    };
    SimpleLights::PointLight pointLight2 = {
        glm::vec3(-2.5f, 3.0f, -0.5f),   // Origin
        glm::vec3(1.0f, 0.0f, 0.0f),    // Attenuation
        glm::vec3(.8f, .8f, .8f),      // Intensity
        glm::vec3(0.0f, 0.0f, 0.0f),    // Ambient
    };

    scene.addLight(&pointLight1);
    scene.addLight(&pointLight2);

    // MATERIALS
    SimpleMaterial::Material material = {
        glm::vec3(1.0f, 0.0f, 0.0f),    // Diffuse
        glm::vec3(1.0f, 1.0f, 1.0f),    // Specular
        glm::vec3(0.1f, 0.0f, 0.0f),    // Ambient
        glm::vec3(0.0f, 0.0f, 0.0f),    // Emission
        128.0f                           // Shiny
    };

    int redMaterial = scene.addMaterial(&material);

    // OBJECTS
    SimpleObjects::Sphere sphere1 = { 
        0.5f,                           // Radius
        glm::vec3(0.0f, 0.0f, 0.0f),    // Center
        redMaterial,                    // Material
        Transform::translate(0.5f, 0.5f, -3.0f) * Transform::scale(1.0f, 2.0f, 1.0f)
    };
    SimpleObjects::Sphere sphere2 = { 
        0.5f,                           // Radius
        glm::vec3(0.0f, 0.0f, 0.0f),    // Center
        redMaterial,                    // Material
        Transform::translate(-0.5f, 1.0f, -2.0f) 
    };
    scene.addObject(&sphere1);
    scene.addObject(&sphere2);

    PixelFunc traceFunc = [&camera, &scene](float uvX, float uvY) {
        return whittedRayTracePixelFunc(camera, scene, uvX, uvY);
    };

    pixelShade(image, width, height, traceFunc);

    // Save output image
    std::string ppmString = SaveUtils::rgbToPpm(image, width, height);
    SaveUtils::writePpm(OUTPUT_FILE_PATH, ppmString);

    return 0;
}
