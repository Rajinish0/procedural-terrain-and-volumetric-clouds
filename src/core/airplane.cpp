#include "airplane.h"

Airplane::Airplane(const AirplaneParameters &params)
    : Model(params.modelPath),
      RigidBody(1.0f, glm::vec3(0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::mat3(1.0f)*3.0f),
      audioMgr(params.audioManager), dropPackage(false), packetSound(params.packetSound)
    {
        this->linearDamp = 0.5f;
        this->angularDamp = 0.3f;
        airDropModel = std::make_shared<Model>(params.airDropPath);
        applyForce(glm::vec3(0.0f, 0.0, -1.0f));
        if (audioMgr) audioMgr->play2D(params.aircraftSound, true);
    }

void Airplane::update(float dt) 
{
    if (window) updateProperties();
    if (camera) updateCameraFeatures();
    auto invRot = glm::inverse(orient);
    glm::vec3 localLinVel = invRot * linearVel;
    glm::vec3 localAngVel = invRot * angularVel;

    // glm::vec3 drag = -dragCoeff * glm::length2(linearVel) * linearVel;
    // applyForce(drag);

    // float angleOfAttack = std::atan2(-localLinVel.y, localLinVel.z);
    // float angleOfYaw = std::atan2(localLinVel.x, localLinVel.z);
    float speed = glm::length(localLinVel);
    glm::vec3 rightVector = orient * glm::vec3(1, 0, 0);
    float currentRoll = std::atan2(rightVector.y, std::fabs(rightVector.x));

    glm::vec3 frontVector = orient * glm::vec3(0, 0, -1);
    float currentPitch  = std::atan2(frontVector.y, glm::length(glm::vec2(frontVector.x, frontVector.z)));
    float stabilizingRoll = 0.0f;
    if (std::fabs(currentRoll) > 0.1){
        stabilizingRoll = -currentRoll * 0.1f;
    }

    float stabilizingPitch = 0.0f;
    if (std::fabs(currentPitch) > 0.1)
        stabilizingPitch = -currentPitch * 0.05f;

    applyRelativeForce(glm::vec3(0.0f, 0.0f, -1.0f) * throttle * maxThrottle);
    float inducedYaw = currentRoll * inducedYawFactor;      
    float inducedPitch = -std::fabs(roll) * .1f;
    
    applyRelativeTorque(glm::vec3(
        (pitch*(1-std::fabs(currentPitch)) + stabilizingPitch + inducedPitch) * pitchSpeed,
        (yaw + inducedYaw) * yawSpeed,      // Yaw (direct + induced)
        (roll*.75+stabilizingRoll*stabilizingRollFactor) * rollSpeed                      // Roll
    ));

    RigidBody::update(dt);

    packets.erase(std::remove_if(packets.begin(), packets.end(),
    [](const Packet& p) { return p.getPos().y < -1.0f; }),
    packets.end());

    if (dropPackage && coolDown <= 0.0f){
        packets.emplace_back(
            airDropModel, this->pos + glm::vec3(0.0f, -2.0f, 0.0f),
            this->linearVel
        );
        coolDown = MAX_COOL_DOWN;
        if (audioMgr) audioMgr->play2D(packetSound);

        onDrop();
    }

    for (Packet& pkt : packets)
        pkt.update(dt);
    
    if (coolDown > MIN_COOL_DOWN)
        coolDown -= dt;
    
    resetProperties();
}

void Airplane::draw(Shader& shader) {
    glm::mat4 rot = glm::toMat4(orient);
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 localRot = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 localScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    shader.setMatrix("model", trans * rot * localRot * localScale);
    Model::draw(shader);

    for (Packet& pkt : packets)
        pkt.draw(shader);
}

void Airplane::mount(Camera *camera){
    this->camera = camera;
}

void Airplane::attach(Window *window){
    this->window = window;
}

// void Airplane::updateCameraFeatures(){
//     glm::vec3 direc = glm::normalize(getDirection());
//     camera->position = this->pos - direc * 10.0f;
//     float yawDeg = glm::degrees(std::atan2(direc.z, direc.x));
//     float pitchDeg = glm::degrees(std::atan2(direc.y, glm::length(glm::vec2(direc.x, direc.z))));
//     camera->setYaw(yawDeg);
//     camera->setPitch(pitchDeg);
//     camera->updateDirection();
// }

void Airplane::updateCameraFeatures() {
    glm::vec3 targetPosition = this->pos - glm::normalize(getDirection()) * 10.0f;

    //TO DO: the smooth factor should be FPS dependent.
    float smoothFactor = 0.1f;
    // camera->position = glm::mix(camera->position, targetPosition, smoothFactor);
    camera->position = targetPosition;

    glm::vec3 direction = glm::normalize(getDirection());
    // std::cout << direction.z << ' ' << direction.x << std::endl;
    float targetYaw = glm::degrees(std::atan2(direction.z, direction.x));
    /* 
    conventionally, this is kept same if x > 0
    but the way i have coded my camera it starts looking at yaw = -90
    in the negative z direction (because it's easier for me to remember with the negative sign)
    so if i make this 270 here instead, there would be a bad jump at the start
    */
    targetYaw = (targetYaw < 0) ? targetYaw : targetYaw - 360.0f;
    float targetPitch = glm::degrees(std::atan2(direction.y, glm::length(glm::vec2(direction.x, direction.z))));

    // std::cout << "TARGET YAW: " << targetYaw << std::endl;

    camera->setYaw(targetYaw);
    camera->setPitch(targetPitch);

    /*TO DO: figure out how to lerp angles 
             e.g. how would this work?
             lerp(-358, -1, theta)
    */
    // camera->setYaw(glm::mix(camera->yaw, std::fmod(targetYaw-360.0f, 360.0f) , smoothFactor));
    // camera->setPitch(glm::mix(camera->pitch, targetPitch, smoothFactor));

    camera->updateDirection();
}


void Airplane::addConfigParamsToImgui(){
    ImGui::SliderFloat("rollFactor", &rollFactor, 0.0f, 15.0f);
    ImGui::SliderFloat("stabilizingRollFactor", &stabilizingRollFactor, 0.0f, 1.0f);
    ImGui::SliderFloat("pitchSpeed", &pitchSpeed, 0.0f, 50.0f);
    ImGui::SliderFloat("yawSpeed", &yawSpeed, 0.0f, 50.0f);
    ImGui::SliderFloat("rollSpeed", &rollSpeed, 0.0f, 50.0f);
    ImGui::SliderFloat("maxThrottle", &maxThrottle, 0.0f, 5000.0f);
    ImGui::SliderFloat("inducedYawFactor", &inducedYawFactor, 0.0f, 0.5f);   
    ImGui::SliderFloat("Drag Coefficient", &dragCoeff, 0.0f, 10.0f);
}

void Airplane::updateProperties(){
    if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS)
        throttle = 0.1;
    if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS)
        throttle = -0.1;
    if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS || 
        glfwGetKey(window->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        roll = 0.1f;
    if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS || 
        glfwGetKey(window->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        roll = -0.1f;        
    if (glfwGetKey(window->window, GLFW_KEY_E) == GLFW_PRESS)
        yaw = -0.1f;
    if (glfwGetKey(window->window, GLFW_KEY_Q) == GLFW_PRESS)
        yaw =  0.1f;
    if (glfwGetKey(window->window, GLFW_KEY_UP) == GLFW_PRESS)
        pitch = -0.1f;
    if (glfwGetKey(window->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch = 0.1f;
    if (glfwGetKey(window->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        dropPackage = true;
}

void Airplane::resetProperties(){
    throttle = 0.0f;
    roll     = 0.0f;
    pitch    = 0.0f;
    yaw      = 0.0f;
    dropPackage = false;
}

void Airplane::bindOnDrop(std::function<void()> func){
    onDrop = func;
}

const std::vector<Packet> &Airplane::getPackets() 
{
    return packets;
}
