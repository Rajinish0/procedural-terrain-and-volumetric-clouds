#ifndef AIRPLANE_H
#define AIRPLANE_H

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
#include "packet.h"
#include "audio_manager.h"
#include <memory>
#include <functional>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <interfaces/audio_manager_interface.h>


struct AirplaneParameters
{
    const std::string &modelPath;
    const std::string &airDropPath;
    const std::string &aircraftSound;
    const std::string &packetSound;

    std::shared_ptr<IAudioManager> audioManager;
};


class Airplane : public Model, public RigidBody {
public:
    Airplane(const AirplaneParameters &parameters);

    void update(float dt);

    void draw(Shader& shader);

    void mount(Camera *camera);

    void attach(Window *window);
    void updateCameraFeatures();

    void addConfigParamsToImgui();

    float rollFactor = REngine::ROLL_FACTOR,
          stabilizingRollFactor = REngine::STABILIZING_ROLL,
          pitchSpeed = REngine::PITCH_SPEED,
          yawSpeed = REngine::YAW_SPEED,
          rollSpeed = REngine::ROLL_SPEED,
          maxThrottle =  REngine::MAX_THROTTLE,
          inducedYawFactor = REngine::INDUCED_YAW_FACTOR,
          dragCoeff = 0.1f;

    void bindOnDrop(std::function<void()> func);

    const std::vector<Packet> &getPackets();

    bool dropPackage;

private:
    Camera *camera = nullptr;
    Window *window = nullptr;
    std::shared_ptr<Model> airDropModel;
    std::shared_ptr<IAudioManager> audioMgr;

    std::vector<Packet> packets;
    float coolDown = -0.1f;
    const float MAX_COOL_DOWN = 3.0f;
    const float MIN_COOL_DOWN = -0.5F;
    std::string packetSound;

    std::function<void()> onDrop = [](){};
    
    float throttle =0.0f,
          pitch    =0.0f, 
          roll     =0.0f, 
          yaw      =0.0f;

    void updateProperties();

    void resetProperties();
};

#endif