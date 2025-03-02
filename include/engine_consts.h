#ifndef RENGINE_CONSTS_H
#define RENGINE_CONSTS_H

namespace REngine{
    constexpr unsigned int WIDTH       = 800U;
    constexpr unsigned int HEIGHT      = 600U;
    constexpr float MAX_TERRAIN_HEIGHT = 128.0f;
    constexpr float DENSITY_THRESHOLD  = .434003f;
    constexpr float SCALE 		       = .595005f;
    // constexpr float SCALE              = 2.604f;
    constexpr float WEATHER_SCALE      = .0001f;
    constexpr float HIGHER_SCALE 	   =  20.0f;
    constexpr float SIGMA 		       = 1.178f;
    constexpr float HG			       = 0.119f;
    constexpr float WIND_SPEED         = 10.0f;
    constexpr float MAX_THROTTLE       = 69.0f;
    constexpr float ROLL_SPEED         = 43.0f;
    constexpr float YAW_SPEED          = 43.0f;
    constexpr float PITCH_SPEED        = 20.0f;
    constexpr float INDUCED_YAW_FACTOR = 0.035f;
    constexpr float STABILIZING_ROLL   = 0.25f;
    constexpr float FAR_VIEW           = 500.0f;
    constexpr float NEAR_VIEW          = 0.1f;
};

#endif