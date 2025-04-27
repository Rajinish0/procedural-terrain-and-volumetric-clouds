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
                  Shader& airplaneShader, Shader& waterShader,
                  Airplane& airplane, float maxHeight, float waterHeight,
                  glm::vec3 sunDirection);

    void update(float dt);
    void draw(FrameBuffer& fbo);
    void addConfigParamsToImgui();

public:
    Camera& camera;
    Shader& shader;
    Shader& normalShader;
    Shader& airplaneShader;
    Shader& waterShader;
    Airplane& airplane;
    EndlessTerrain terrain;
    float waterHeight;
    FrameBuffer fb, fb2;
    Texture dudv;
    Texture normalMap;
    Plane plane;
    float moveFac;
    float moveVel = 0.015f;
    bool drawWater = false;
    bool drawNormals = false;
    bool lineMode = false;
};

#endif