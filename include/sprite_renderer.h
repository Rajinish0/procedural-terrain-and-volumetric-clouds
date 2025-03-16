#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <string>
#include "plane.h"
#include "funcs.h"


class SpriteRenderer{
public:    
    SpriteRenderer(std::string fname){
        id = funcs::TextureFromFile(fname);
    }

    void draw(Shader& shader) const{
        if (id){

        }
    }

private:
    unsigned int id;
    Plane plane;
};


#endif