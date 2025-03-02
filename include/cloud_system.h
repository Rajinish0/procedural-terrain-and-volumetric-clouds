#ifndef CLOUD_SYSTEM_H
#define CLOUD_SYSTEM_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "funcs.h"
#include "compute_shader.h"
#include "shader.h"
#include "engine_consts.h"
#include "framebuffer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class CloudSystem{

public:
    CloudSystem(const std::string& high_texture_path, 
                const std::string& detail_texture, 
                const std::string& weather_texture, 
                Camera& camera,
                ComputeShader& computeShader,
                glm::vec3 boundingRectDims,
                float nearPlane,
                float farPlane,
                float cloudHeight,
                glm::mat4 proj,
                glm::vec3 sunDirection,
                FrameBuffer& bgFBO,
                float densityThreshold = REngine::DENSITY_THRESHOLD,
                float scale            = REngine::SCALE,
                float weatherScale     = REngine::WEATHER_SCALE,
                float higherScale      = REngine::HIGHER_SCALE,
                float sigma            = REngine::SIGMA,
                float hg               = REngine::HG
                );

    void update(FrameBuffer& fbo);

    void draw(Shader& shader);

    void addConfigParamsToImgui();

private:
    unsigned int compT;
    Camera& camera;
    ComputeShader& computeShader;
    float cloudHeight;
    float densityThreshold,
          scale, weatherScale,
          higherScale, sigma, hg;
    FrameBuffer& fbo;
    GLuint weatherTextureId,
           detailTextureId,
           highTextureId;
    Plane plane;

    void setUpComputeTexture();
};

#endif