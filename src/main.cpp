#include<array>
#include<cmath>
#include<iostream>
#include<string>
#include<vector>

#include<glm/glm.hpp>

// TODO
#include "PixelDraw.hpp"
#include "Raytracing.hpp"
#include "SaveUtils.hpp"
#include "Transform.hpp"

#include "SimpleGraphics/Camera.hpp"
#include "SimpleGraphics/Lights.hpp"
#include "SimpleGraphics/Materials.hpp"
#include "SimpleGraphics/Objects.hpp"
#include "SimpleGraphics/Scene.hpp"

const char* OUTPUT_FILE_PATH = "./test.ppm";

glm::vec3 whittedRayTracePixelFunc(
    const SimpleGraphics::Camera& camera, 
    const SimpleGraphics::Scene& scene,
    const int maxBounces,
    float uvX, 
    float uvY 
) {
    // TODO - May need to shift Uvs by 0.5 so rays go through center of pixels instead of corners
    // Remap uvs into [-1, 1]^2
    uvX = (uvX * 2.0f) - 1.0f;
    uvY = (uvY * 2.0f) - 1.0f;
    Raytracing::Ray viewRay = camera.viewRay(uvX, uvY);

    return SimpleGraphics::whittedRayTrace(
        viewRay, 
        scene,
        glm::vec3(0.0f, 0.0f, 0.2f),
        maxBounces
    );
}


int main() {
    int width = 400;
    int height = 300;
    int maxBounces = 1;

    // Initialize image
    std::cout << "Initialize image with shape (" << width << ", " << height << ")" << std::endl;
    glm::vec3** image;
    image = new glm::vec3*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new glm::vec3[width];
    }

    // CAMERA
    SimpleGraphics::Camera camera = SimpleGraphics::Camera(
        glm::vec3(-1.5f, 0.5f, 2.0f), 
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        60.0f,
        (float)width / (float)height
    );

    // CREATE SCENE
    SimpleGraphics::Scene scene = SimpleGraphics::Scene();

    // LIGHTS
    SimpleGraphics::PointLight pointLight1 = {
        glm::vec3(1.5f, 2.0f, 0.5f),   // Origin
        glm::vec3(0.0f, 0.0f, 1.0f),    // Attenuation
        glm::vec3(7.5f),                // Intensity
        glm::vec3(0.0f, 0.0f, 0.0f),    // Ambient
    };
    SimpleGraphics::PointLight pointLight2 = {
        glm::vec3(-2.5f, 3.0f, -0.5f),   // Origin
        glm::vec3(0.0f, 0.0f, 1.0f),     // Attenuation
        glm::vec3(10.0f),                // Intensity
        glm::vec3(0.0f, 0.0f, 0.0f),    // Ambient
    };

    scene.addLight(&pointLight1);
    scene.addLight(&pointLight2);

    // MATERIALS
    SimpleGraphics::StaticMaterial redMat = {
        glm::vec3(0.9f, 0.0f, 0.0f),    // Diffuse
        glm::vec3(0.5f),                // Specular
        glm::vec3(0.1f, 0.0f, 0.0f),    // Ambient
        glm::vec3(0.0f, 0.0f, 0.0f),    // Emission
        128.0f                           // Shiny
    };

    SimpleGraphics::CheckerboardXZ planeMat = {
        2.0f, 2.0f,         // scaleX, scaleY
        0.1f,               // ambientMultiplier
        glm::vec3(0.9f),    // oddColor
        glm::vec3(0.1f),    // evenColor
        glm::vec3(0.2f),   // Specular
        64.0f               // Shiny
    };

    int redMaterial = scene.addMaterial(&redMat);
    int planeMaterial = scene.addMaterial(&planeMat);

    // OBJECTS
    SimpleGraphics::Sphere sphere1 = { 
        0.5f,                           // Radius
        glm::vec3(0.0f, 0.0f, 0.0f),    // Center
        redMaterial,                    // Material
        Transform::scale(1.0f, 2.0f, 1.0f)
    };

    SimpleGraphics::Sphere sphere2 = { 
        0.5f,                           // Radius
        glm::vec3(0.0f, 0.0f, 0.0f),    // Center
        redMaterial,                    // Material
        Transform::translate(-0.5f, 0.5f, -0.7f) 
    };

    glm::mat4 planeTransform = Transform::translate(-2.5f, -0.5f, 1.0f);
    SimpleGraphics::Triangle tri1 = {
        glm::vec3(5.0f, 0.0f, 0.0f),
        glm::vec3(5.0f, 0.0f, -5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        planeMaterial,
        planeTransform
    };
    SimpleGraphics::Triangle tri2 = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(5.0f, 0.0f, -5.0f),
        glm::vec3(0.0f, 0.0f, -5.0f),
        planeMaterial,
        planeTransform
    };

    scene.addObject(&sphere1);
    scene.addObject(&sphere2);
    scene.addObject(&tri1);
    scene.addObject(&tri2);

    PixelDraw::PixelFunc traceFunc = [&camera, &scene, maxBounces](float uvX, float uvY) {
        return whittedRayTracePixelFunc(
            camera, 
            scene, 
            maxBounces, 
            uvX, 
            uvY
        );
    };

    PixelDraw::pixelShade(image, width, height, traceFunc);

    // Save output image
    std::string ppmString = SaveUtils::rgbToPpm(image, width, height);
    SaveUtils::writePpm(OUTPUT_FILE_PATH, ppmString);

    return 0;
}
