#ifndef PLANE_H
#define PLANE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"




class Plane{

private:
    static bool initialized;

public: 
    static unsigned int VAO, VBO;


    /*
    THIS GUY'S ONLY CONCCERN IS DRAWING A PLANE, WITH A GIVEN TEXTURE
    ALL OTHER TEXTURES REQUIRED MUST BE MANUALLY BOUND TO 
    GL_TEXTURE0 + i (where i >= 1) AND MUSH ALSO BE MANUALLY 
    SET BY THE SHADER TO PREDIFNED UNIFORM VARIABLES BEFOREHAND

    TO DO:
    This is bad design, i need to change it to a singleton class
    and I just destroy it at the end of the prog
    */
    Plane();

    void draw(Shader& shader, unsigned int textureId);

};

#endif