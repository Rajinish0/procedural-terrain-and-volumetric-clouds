#ifndef RENGINE_CONSTS_H
#define RENGINE_CONSTS_H


#define X_AXIS glm::vec3(1.0f, 0.0f, 0.0f)
#define Y_AXIS glm::vec3(0.0f, 1.0f, 0.0f)
#define Z_AXIS glm::vec3(0.0f, 0.0f, 1.0f)

namespace REngine{
    constexpr unsigned int WIDTH       = 800U;
    constexpr unsigned int HEIGHT      = 600U;
    constexpr float MAX_TERRAIN_HEIGHT = 128.0f;
    constexpr float DENSITY_THRESHOLD  = .534003f;
    constexpr float SCALE 		       = .595005f;
    // constexpr float SCALE              = 2.604f;
    constexpr float WEATHER_SCALE      = .0001f;
    constexpr float HIGHER_SCALE 	   =  20.0f;
    constexpr float SIGMA 		       = 1.178f;
    constexpr float HG			       = 0.119f;
    constexpr float WIND_SPEED         = 10.0f;
    constexpr float MAX_THROTTLE       = 198.0f;
    constexpr float ROLL_SPEED         = 32.175f;
    constexpr float YAW_SPEED          = 32.175f;
    constexpr float PITCH_SPEED        = 20.0f;
    constexpr float INDUCED_YAW_FACTOR = 0.055f;
    constexpr float STABILIZING_ROLL   = 0.5f;
    constexpr float ROLL_FACTOR        = 0.75f;
    constexpr float FAR_VIEW           = 500.0f;
    constexpr float NEAR_VIEW          = 0.1f;
};

#endif