#ifndef PACKET_HEADER
#define PACKET_HEADER

#include "model.h"
#include "rigid_body.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "camera.h"
#include "window.h"
#include "engine_consts.h"
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <random> // For randomness


class Packet : public RigidBody {
public:
    Packet(std::shared_ptr<Model> model, const glm::vec3& pos, 
           const glm::vec3& initVel = {0.0f, 0.0f, 0.0f}, float gravity = 3.0f)
        :RigidBody(1.0, pos, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::mat3(1.0f), initVel),
         pkt_model(model),
         gravity(gravity)
         {
            this->linearDamp = 1.0f;
            this->angularDamp = 0.3f;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
    
            glm::vec3 randomTorque = glm::vec3(dist(gen), dist(gen), dist(gen));
            applyTorque(randomTorque);
         }

    void update(float dt){
        applyForce(glm::vec3(0.0f, -gravity, 0.0f));
        RigidBody::update(dt);
    }

    void draw(Shader& shader){
        glm::mat4 model(1.0f);
        model = glm::translate(model, this->pos);
        model *= glm::toMat4(orient);
        model = glm::scale(model, glm::vec3(0.5f));
        shader.use();
        shader.setMatrix("model", model);
        pkt_model->draw(shader);
    }
    
private:
    float gravity;
    std::shared_ptr<Model> pkt_model;

};

#endif