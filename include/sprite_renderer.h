#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <string>
#include "plane.h"
#include "funcs.h"
#include "camera.h"

class Sprite{
public:    
    Sprite(std::string fname, float size = 1.0f, float rotation = 0.0f,
           bool pulsating = false);

    void setSize(float size);
    float getSize();

    void setRotation(float rot);

    void setPulsating(bool flag);

    glm::mat4 getRotationMatrix(float theta); //around center

    //SHADER proj needs to be preset
    void bind(Shader& shader);

    ~Sprite();

private:
    unsigned int id;
    // glm::mat4 localTransform;
    float size, rotation;
    bool pulsating = false;
    // Plane plane;
};



class SpriteRenderer {

public:
    enum Mode {
        TOP_LEFT,
        CENTER
    };
    SpriteRenderer();

    void initVertexData();

    void draw(Sprite& sprite, Shader& shader, glm::vec3 pos,
              float rotation = 0.0f, Mode mode = TOP_LEFT);

    ~SpriteRenderer();
    
    
private:
    bool ready =false;
    GLuint VAO, VBO;
};
    

#endif