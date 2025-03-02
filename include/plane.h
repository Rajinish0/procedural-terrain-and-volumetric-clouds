#ifndef PLANE_H
#define PLANE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"


class Plane{

public: 
    unsigned int VAO, VBO;

    Plane();

    void draw(Shader& shader, unsigned int textureId, unsigned int depthTextureId);

};

#endif