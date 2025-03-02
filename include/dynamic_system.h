#ifndef DYNAMIC_SYSTEM_H
#define DYNAMIC_SYSTEM_H
#include "camera.h"
#include "endless_terrain.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "airplane.h"
#include "framebuffer.h"

class DynamicSystem{
public:
    DynamicSystem(Camera& camera, Shader& shader, Shader& normalShader, 
                  Shader& airplaneShader,
                  Airplane& airplane, float maxHeight, glm::vec3 sunDirection);

    // #ifdef DRAW_WATER
    // DynamicSystem(Camera& camera, Shader& shader, Shader& airplaneShader, Shader& waterShader,
    //               Airplane& airplane, float maxHeight, glm::vec3 sunDirection, 
    //               float waterHeight, float moveFac);
    // #endif

    void update(float dt);

    void draw();

public:
    Camera& camera;
    Shader& shader;
    Shader& normalShader;
    Shader& airplaneShader;
    Airplane& airplane;
    EndlessTerrain terrain;
    // #ifdef DRAW_WATER
    // FrameBuffer fb, fb2;
    // Shader& waterShader;
    // float waterHeight;
    // Plane plane;
    // #endif
};

#endif