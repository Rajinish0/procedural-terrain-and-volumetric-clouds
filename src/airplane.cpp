#include "airplane.h"

Airplane::Airplane(const std::string& path,
                   const std::string& airDropPath, 
                   const std::string& aircraftSound,
                   const std::string& packetSound,
                   std::shared_ptr<AudioManager> audioManager
                  )
    : Model(path),
      RigidBody(1.0f, glm::vec3(0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::mat3(1.0f)*3.0f),
      audioMgr(audioManager), dropPackage(false), packetSound(packetSound)
    {
        airDropModel = std::make_shared<Model>(airDropPath);
        // audioMgr     = std::make_unique<AudioManager>();
        applyForce(glm::vec3(0.0f, 0.0, -10.0f));
        audioMgr->play2D(aircraftSound, true);
    }

void Airplane::update(float dt) {
    if (window) updateParameters();
    if (camera) updateCameraFeatures();
    auto invRot = glm::inverse(orient);
    glm::vec3 localLinVel = invRot * linearVel;
    glm::vec3 localAngVel = invRot * angularVel;

    float angleOfAttack = std::atan2(-localLinVel.y, localLinVel.z);
    float angleOfYaw = std::atan2(localLinVel.x, localLinVel.z);
    float speed = glm::length(localLinVel);
    glm::vec3 rightVector = orient * glm::vec3(1, 0, 0);
    float currentRoll = std::atan2(rightVector.y, std::fabs(rightVector.x));
    // std::cout << currentRoll << std::endl; 

    glm::vec3 frontVector = orient * glm::vec3(0, 0, -1);
    float currentPitch  = std::atan2(frontVector.y, glm::length(glm::vec2(frontVector.x, frontVector.z)));
    // std::cout << currentPitch << std::endl;
    // // Calculate self-stabilizing roll force
    float stabilizingRoll = 0.0f;
    if (std::fabs(currentRoll) > 0.1){
        stabilizingRoll = -currentRoll * 0.1f;  // Increases with speed
        // stabilizingRoll *= (1.0f - std::fabs(currentRoll) * 0.1f);  // Damp as roll increases
    }

    float stabilizingPitch = 0.0f;
    if (std::fabs(currentPitch) > 0.1)
        stabilizingPitch = -currentPitch * 0.05f;  // Increases with speed

    // Apply thrust
    applyRelativeForce(glm::vec3(0.0f, 0.0f, -1.0f) * throttle * maxThrottle);
    // Natural yaw induced by roll (when you roll, the lift vector tilts, causing turning)
    float inducedYaw = currentRoll * inducedYawFactor;      
    float inducedPitch = -std::fabs(roll) * .1f;
    
    // std::cout << angleOfAttack << std::endl;
    // Control forces
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
        coolDown = 3.0f;
        audioMgr->play2D(packetSound);
        // std::cout << "DROPPING PACKAGE " << std::endl;
    }

    for (Packet& pkt : packets)
        pkt.update(dt);
    // audioMgr->update();
    
    if (coolDown > -0.5f)
        coolDown -= dt;
    
    resetParameters();
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

void Airplane::updateCameraFeatures(){
    glm::vec3 direc = glm::normalize(getDirection());
    camera->position = this->pos - direc * 10.0f;
    float yawDeg = glm::degrees(std::atan2(direc.z, direc.x));
    float pitchDeg = glm::degrees(std::atan2(direc.y, glm::length(glm::vec2(direc.x, direc.z))));
    camera->setYaw(yawDeg);
    camera->setPitch(pitchDeg);
    camera->updateDirection();
}

void Airplane::addConfigParamsToImgui(){
    ImGui::SliderFloat("rollFactor", &rollFactor, 0.0f, 15.0f);
    ImGui::SliderFloat("stabilizingRollFactor", &stabilizingRollFactor, 0.0f, 1.0f);
    ImGui::SliderFloat("pitchSpeed", &pitchSpeed, 0.0f, 50.0f);
    ImGui::SliderFloat("yawSpeed", &yawSpeed, 0.0f, 50.0f);
    ImGui::SliderFloat("rollSpeed", &rollSpeed, 0.0f, 50.0f);
    ImGui::SliderFloat("maxThrottle", &maxThrottle, 0.0f, 100.0f);
    ImGui::SliderFloat("inducedYawFactor", &inducedYawFactor, 0.0f, 0.5f);   
}

void Airplane::updateParameters(){
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

void Airplane::resetParameters(){
    throttle = 0.0f;
    roll     = 0.0f;
    pitch    = 0.0f;
    yaw      = 0.0f;
    dropPackage = false;
}

