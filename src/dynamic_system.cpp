#include "dynamic_system.h"

DynamicSystem::DynamicSystem(Camera& camera, Shader& shader, Shader& normalShader, Shader& airplaneShader,
    Airplane& airplane, float maxHeight, glm::vec3 sunDirection)
        :camera(camera), shader(shader), normalShader(normalShader), airplaneShader(airplaneShader),
        airplane(airplane), terrain(camera)
        {
            shader.use();
            shader.setFloat("maxHeight", maxHeight);
            shader.setVec3("sunDirection", sunDirection);

            airplaneShader.use();
            airplaneShader.setVec3("sunDirection", sunDirection);

            normalShader.use();
            normalShader.setFloat("maxHeight", maxHeight);
        }

void DynamicSystem::update(float dt){
        airplane.update(dt);
}


// #ifdef DRAW_WATER
// DynamicSystem::DynamicSystem(Camera& camera, Shader& shader, Shader& airplaneShader, Shader& waterShader,
//     Airplane& airplane, float maxHeight, glm::vec3 sunDirection, float waterHeight, float moveFac)
//     :camera(camera), shader(shader), airplaneShader(airplaneShader),
//     airplane(airplane), terrain(camera), fb(), fb2(), waterShader(waterShader),
//     waterHeight(waterHeight)
//     {
//         shader.use();
//         shader.setFloat("maxHeight", maxHeight);
//         shader.setVec3("sunDirection", sunDirection);

//         airplaneShader.use();
//         airplaneShader.setVec3("sunDirection", sunDirection);

//         waterShader.use();
//         waterShader.setInt("t1",  0);
//         waterShader.setInt("t2",  1);
//         waterShader.setInt("dudv",2);
//         waterShader.setFloat("moveFac", moveFac);
//     }

// #endif

void DynamicSystem::draw(){

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CLIP_DISTANCE0);

    shader.use();
    shader.setMatrix("view", camera.getView());
    terrain.draw(shader);

    #ifdef DRAW_NORMALS
    normalShader.use();
    normalShader.setMatrix("view", camera.getView());
    terrain.draw(normalShader);
    #endif


    airplaneShader.use();
    airplaneShader.setMatrix("view", camera.getView());
    airplane.draw(airplaneShader);
}