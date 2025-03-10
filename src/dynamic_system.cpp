#include "dynamic_system.h"

DynamicSystem::DynamicSystem(Camera& camera, Shader& shader, Shader& normalShader, Shader& airplaneShader,
    Shader& waterShader, Airplane& airplane, float maxHeight, 
    float waterHeight, glm::vec3 sunDirection)
        :camera(camera), shader(shader), normalShader(normalShader), airplaneShader(airplaneShader),
        waterShader(waterShader), airplane(airplane), terrain(camera), waterHeight(waterHeight), 
        dudv("textures/dudv.png", Texture::DIFFUSE, GL_REPEAT, GL_REPEAT), moveFac(0.0f)
        {
            shader.use();
            shader.setFloat("maxHeight", maxHeight);
            shader.setVec3("sunDirection", sunDirection);

            airplaneShader.use();
            airplaneShader.setVec3("sunDirection", sunDirection);

            normalShader.use();
            normalShader.setFloat("maxHeight", maxHeight);

            waterShader.use();
            waterShader.setInt("t1", 0);
            waterShader.setInt("t2", 1);
            waterShader.setInt("dudv", 2);
        }

void DynamicSystem::update(float dt){
        airplane.update(dt);
        moveFac += moveVel * dt;
        moveFac -= (int)moveFac;
}


void DynamicSystem::draw(FrameBuffer& fbo){

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CLIP_DISTANCE0);
    // float offSet = 2*(camera.position.y - waterHeight);
    // camera.position = glm::vec3(camera.position.x, 
    //                          camera.position.y - offSet,
    //                          camera.position.z);//camera.position.y -= offSet;
    // camera.pitch = -camera.pitch;
    // camera.updateDirection();
    // shader.setMatrix("view", camera.getView());
    // shader.setVec4("planeNorm", glm::vec4(0.0f, 1.0f, 0.0f, -waterHeight));
    // fb.Bind();
    // terrain.draw(shader);
    // fb.unBind();

    // camera.position = glm::vec3(camera.position.x, 
    //                          camera.position.y + offSet,
    //                          camera.position.z);
    // camera.pitch = -camera.pitch;
    // camera.updateDirection();
    // shader.setMatrix("view", camera.getView());
    // shader.setVec4("planeNorm", glm::vec4(0.0f,-1.0f, 0.0f, waterHeight));
    // fb2.Bind();
    // terrain.draw(shader);
    // fb2.unBind();



    fbo.Bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CLIP_DISTANCE0);

    // waterShader.use();
    // waterShader.setMatrix("view", camera.getView());
    // glm::mat4 m =glm::translate(glm::mat4(1.0f), glm::vec3(camera.position.x, waterHeight, camera.position.z));
    // glm::mat4 m2 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // glm::mat4 m3 =glm::scale(glm::mat4(1.0f), glm::vec3((50.0f*20.0f)/2.0f));
    // waterShader.setMatrix("model", m*m2*m3);

    // waterShader.setFloat("moveFac", moveFac);
    // waterShader.setVec3("camPos", camera.position);
    // waterShader.setVec3("planeNorm", glm::vec3(0.0f, 1.0f, 0.0f));

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, fb.textureId);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, fb2.textureId);

    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, dudv.id);

    // glBindVertexArray(plane.VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    // glBindVertexArray(0);

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

    fbo.unBind();
}