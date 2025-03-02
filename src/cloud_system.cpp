#include "cloud_system.h"

CloudSystem::CloudSystem(const std::string& high_texture_path, 
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
    float densityThreshold ,
    float scale            ,
    float weatherScale     ,
    float higherScale      ,
    float sigma            ,
    float hg               
    )
    :camera(camera), computeShader(computeShader), cloudHeight(cloudHeight),
    densityThreshold(densityThreshold), scale(scale), weatherScale(weatherScale),
    higherScale(higherScale), sigma(sigma), hg(hg), fbo(bgFBO)
    {
    weatherTextureId = funcs::loadWeatherData(weather_texture);
    detailTextureId  = funcs::loadDetailTexture(detail_texture);
    highTextureId    = funcs::loadGeneric3dTexture(high_texture_path);
    setUpComputeTexture();

    computeShader.use();
    computeShader.setVec3("bounding_rect.dims", boundingRectDims);
    computeShader.setFloat("near", nearPlane);
    computeShader.setFloat("far", farPlane);
    computeShader.setMatrix("invProjMat", glm::inverse(proj));
    computeShader.setInt("texture_diffuse1", 1);
    computeShader.setInt("depthTexture", 2);
    computeShader.setInt("texture_clouds", 3);
    computeShader.setInt("weather_data", 4);
    computeShader.setInt("detailTexture", 5);
    computeShader.setVec3("sunDirection", sunDirection);
    }

void CloudSystem::update(FrameBuffer& fbo){
    glDisable(GL_BLEND);
    computeShader.use();
    computeShader.setMatrix("invViewMat", glm::inverse(camera.getView()));
    computeShader.setVec3("camPos", camera.position);
    computeShader.setVec3("bounding_rect.pos", glm::vec3(camera.position.x, cloudHeight, camera.position.z));
    computeShader.setVec3("offSet", glm::vec3(
        camera.position.x,
        0.0f, 
        camera.position.z + (float)glfwGetTime()*REngine::WIND_SPEED
    ));

    computeShader.setFloat("densityThreshold", densityThreshold);
    computeShader.setFloat("scale", scale);
    computeShader.setFloat("weatherScale", weatherScale);
    computeShader.setFloat("higherScale", higherScale);
    computeShader.setFloat("SIGMA", sigma);
    computeShader.setFloat("HG", hg);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, compT);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbo.textureId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fbo.depthTextureId);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_3D, highTextureId);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, weatherTextureId);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_3D, detailTextureId);
    glDispatchCompute((unsigned int)std::ceil(800.0/16.0), 
                        (unsigned int)std::ceil(600.0/16.0),
                        1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // computeShader.use();
    // computeShader.setMatrix(
    //     "invViewMat",
    //     glm::inverse(camera.getView())
    // );
    // computeShader.setVec3("camPos", camera.position);
    // computeShader.setVec3("bounding_rect.pos", glm::vec3(camera.position.x, cloudHeight, camera.position.z));
    // computeShader.setVec3("offSet", glm::vec3(
    //     camera.position.x, 
    //     0.0f,
    //     camera.position.z + (float)glfwGetTime()*REngine::WIND_SPEED
    // ));

    // glDisable(GL_BLEND);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, compT);
    // computeShader.setFloat("densityThreshold", densityThreshold);
    // computeShader.setFloat("scale", scale);
    // computeShader.setFloat("weatherScale", weatherScale);
    // computeShader.setFloat("higherScale", higherScale);
    // computeShader.setFloat("SIGMA", sigma);
    // computeShader.setFloat("HG", hg);
    // // computeShader.setVec3("sunDirection", sunDirection);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, fbo.textureId);
    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, fbo.depthTextureId);
    // glActiveTexture(GL_TEXTURE3);
    // // glBindTexture(GL_TEXTURE_3D, ntId);
    // glBindTexture(GL_TEXTURE_3D, highTextureId);
    // glActiveTexture(GL_TEXTURE4);
    // glBindTexture(GL_TEXTURE_2D, weatherTextureId);
    // // glBindTexture(GL_TEXTURE_2D, weatherDataTexure);
    // glActiveTexture(GL_TEXTURE5);
    // glBindTexture(GL_TEXTURE_3D, detailTextureId);
    // glDispatchCompute((unsigned int)std::ceil(800.0/16.0), 
    //                   (unsigned int)std::ceil(600.0/16.0),
    //                   1);
    // glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}


void CloudSystem::draw(Shader& shader){
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, compT);
    glBindVertexArray(plane.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void CloudSystem::addConfigParamsToImgui(){
    ImGui::SliderFloat("Density Threshold", &densityThreshold, 0.0f, 1.0f);
    ImGui::SliderFloat("Scale", &scale, 0.0f, 2.0f);
    ImGui::SliderFloat("HG", &hg, 0.0f, 1.0f);
    ImGui::SliderFloat("SIGMA", &sigma, 0.0f, 2.0f);
    ImGui::SliderFloat("Higher Scale", &higherScale, 0.0f, 20.0f);
    ImGui::SliderFloat("Weather Scale", &weatherScale, 0.0f, 0.005f);
    ImGui::SliderFloat("Cloud Height", &cloudHeight, 20.0f, 200.0f);
}

void CloudSystem::setUpComputeTexture(){
    glGenTextures(1, &compT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, compT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, 
                GL_FLOAT, NULL);

    glBindImageTexture(0, compT, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindTexture(GL_TEXTURE_2D, 0);
}

