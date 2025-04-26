#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "debug.h"

/* 
This is not fully general yet, see the thesis:
the full equation for torque is T = Ia + w x Iw
so a = inv(I)(T - w x Iw) would be the actual equation.
*/

class RigidBody{
public:
    RigidBody (float mass, glm::vec3 pos, glm::quat orient, glm::mat4 inertiaTensor, 
               glm::vec3 linearVel = {0.0f, 0.0f, 0.0f}, glm::vec3 angularVel = {0.0f, 0.0f, 0.0f},
               float linearDamp = 0.99f, float angularDamp = 0.99f)
        :mass(mass), pos(pos), orient(orient), linearVel(linearVel), angularVel(angularVel), 
         inverseInertiaTensor(glm::inverse(inertiaTensor)), totalForce(0.0f), totalTorque(0.0f),
         linearDamp(linearDamp), angularDamp(angularDamp)
         {
        
         }
    
    void update(float dt){
        glm::vec3 linearAcc = totalForce / mass;
        linearVel += linearAcc * dt;
        pos += linearVel * dt;

        glm::mat3 worldInverseInertia = glm::toMat3(orient) * inverseInertiaTensor * glm::transpose(glm::toMat3(orient));                
        glm::vec3 angularAcc = worldInverseInertia * totalTorque;
        angularVel += angularAcc * dt;
        orient += 0.5f * glm::quat(0.0f, angularVel * dt) * orient;
        orient = glm::normalize(orient);

        angularVel *= std::pow(angularDamp, dt);
        linearVel *= std::pow(linearDamp, dt);

        resetExternalForces();
    }

    void applyForce(const glm::vec3& force){
        totalForce += force;
    }

    // relative to the current orientation
    void applyRelativeForce(const glm::vec3& localForce){
        totalForce += orient * localForce;
    }

    void applyRelativeTorque(const glm::vec3& localTorque){
        totalTorque += orient * localTorque;
    }

    //assumes point is inside the body(in local space), im not gonna do bound checking.
    void applyForceAtPoint(glm::vec3 point, glm::vec3 force){
        glm::vec3 dirVec = transformToGlobal(point) - pos;
        totalTorque += glm::cross(dirVec, force);
        totalForce += force;
    }

    void applyTorque(glm::vec3 torque){
        totalTorque += torque;
    }

    glm::vec3 getPos() const{
        return pos;
    }

    void setPos(glm::vec3 pos){
        this->pos = pos;
    }

    glm::vec3 getDirection() const{
        return orient * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    glm::vec3 getLocalLinVel() const {
        return glm::inverse(orient) * linearVel;
    }

    glm::vec3 getLinearVel() const {
        return linearVel;
    }

    glm::quat getOrient() const {
        return orient;
    }

    float getMass() const {
        return mass;
    }

    glm::vec3 getAngularVel() const {
        return angularVel;
    }

    glm::vec3 getTotalForce() const {
        return totalForce;
    }

    glm::vec3 getTotalTorque() const {
        return totalTorque;
    }

    float linearDamp, 
          angularDamp;

protected:
    float mass;
    glm::vec3 pos;
    glm::quat orient;
    glm::vec3 linearVel;
    glm::vec3 angularVel;
    glm::mat3 inverseInertiaTensor;
    glm::vec3 totalForce;
    glm::vec3 totalTorque;


    void resetExternalForces(){
        totalForce = glm::vec3(0.0f);
        totalTorque = glm::vec3(0.0f);
    }

    glm::vec3 transformToGlobal(glm::vec3 point){
        return glm::mat3(orient) * point + pos; 
    }

};

#endif