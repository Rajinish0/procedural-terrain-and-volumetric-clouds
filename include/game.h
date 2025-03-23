#ifndef _GAME_H
#define _GAME_H

#include "airplane.h"
#include "textrender.h"
#include <memory>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "g_types.h"
#include "sprite_renderer.h"
#include "camera.h"
#include "dynamic_system.h"
#include "cloud_system.h"

namespace game {
    struct GameParameters{
        TextRenderer &textRenderer;
        Airplane &airplane;
        Camera &camera;
        DynamicSystem &terrainSystem;
        CloudSystem &cloudSystem;
        SpriteRenderer &spriteRenderer;


        //change these directly if you want to 
        glm::vec3 coordsTextColor = {0.23921f, 0.415686f, 0.44705f};
        glm::vec3 tCoordsTextColor = {0.83921f, 0.415686f, 0.44705f};
        float minRange = -500.0f;
        float maxRange =  500.0f;
        float distThresholdSqr = 100.0f * 100.0f; // the circle in which package can be dropped
        std::string arrowTexture    = "textures/left-arrow.png";
        std::string circleTexture   = "textures/circle.png";
        std::string targetTexture   = "textures/blue_circle.png";

        GameParameters(TextRenderer &textRenderer, Airplane &airplane, Camera &camera,
                       DynamicSystem &terrainSystem, CloudSystem &cloudSystem, 
                       SpriteRenderer &spriteRenderer)
            :textRenderer(textRenderer), airplane(airplane), camera(camera),
             terrainSystem(terrainSystem), cloudSystem(cloudSystem),
             spriteRenderer(spriteRenderer)
            {}

    };

    class Game{
    public:
        Game(std::shared_ptr<GameParameters> params);

        void update(float dt);
        void draw(Shader &textShader, Shader &screenShader, Shader &spriteShader);

        void addConfigParamsToImgui();

        void onDrop();

        ~Game();

    private:
    /*
    why not add every field of params here?
    because if i want to add some other field to params at a later stage
    i only want to add it to one place (in the above struct) and just access
    it from this params field
    */
        std::shared_ptr<GameParameters> params;
        glm::vec2 targetCoords;
        bool dropped = false;
        Sprite arrowSprite;
        Sprite circleSprite;
        Sprite targetSprite;
        FrameBuffer fbo;
    };
};

#endif