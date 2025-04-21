#include <gtest/gtest.h>
#include <Camera.h>
#include <glm/glm.hpp>

constexpr float EPSILON = 1e-4f;

TEST(CameraTest, InitializesFacingNegativeZ) {
    Camera cam(0.1f);
    glm::vec3 front = cam.getFront();
    EXPECT_NEAR(front.x, 0.0f, EPSILON);
    EXPECT_NEAR(front.y, 0.0f, EPSILON);
    EXPECT_NEAR(front.z, -1.0f, EPSILON);
}

TEST(CameraTest, YawRotationAffectsDirection) {
    Camera cam(0.1f);
    cam.incYaw(90.0f);
    glm::vec3 dir = cam.getFront();
    EXPECT_NEAR(dir.x, 1.0f, EPSILON);
    EXPECT_NEAR(dir.z, 0.0f, EPSILON);
}

TEST(CameraTest, PitchRotationClampsAndAffectsDirection) {
    Camera cam(0.1f);
    cam.incPitch(100.0f);
    glm::vec3 dir = cam.getFront();
    EXPECT_LE(cam.getFront().y, 1.0f);
    EXPECT_NEAR(glm::length(dir), 1.0f, EPSILON);
}

TEST(CameraTest, MovementChangesPosition) {
    Camera cam(0.1f);
    float dt = 1.0f;

    cam.move(Camera::UP, dt);
    EXPECT_NEAR(cam.position.z, -1.0f, EPSILON);

    cam.move(Camera::DOWN, dt);
    EXPECT_NEAR(cam.position.z, 0.0f, EPSILON);

    cam.move(Camera::RIGHT, dt);
    EXPECT_GT(cam.position.x, 0.0f);
}

TEST(CameraTest, MouseInputChangesYawAndPitch) {
    Camera cam(0.5f); // higher sensitivity for easier test

    cam.handleMouse(100, 100); // firstMouse → ignored
    cam.handleMouse(110, 90);  // dx = 10, dy = 10

    EXPECT_NEAR(cam.getFront().y, glm::sin(glm::radians(5.0f)), EPSILON); // pitch ~5°
}

TEST(CameraTest, ViewMatrixIsValid) {
    Camera cam(0.1f);
    glm::mat4 view = cam.getView();

    // View matrix should not be identity initially
    glm::mat4 identity(1.0f);
    EXPECT_NE(view, identity);
}


TEST(CameraTest, PitchClampsAtEdges) {
    Camera cam(0.1f);

    cam.setPitch(90.0f); // way over limit
    cam.updateDirection();
    EXPECT_LE(cam.getFront().y, glm::sin(glm::radians(45.0f)) + EPSILON);

    cam.setPitch(-90.0f);
    cam.updateDirection();
    EXPECT_GE(cam.getFront().y, glm::sin(glm::radians(-45.0f)) - EPSILON);
}


TEST(CameraTest, YawWrapsAround) {
    Camera cam(0.1f);

    cam.setYaw(450.0f); // 450 -> 90°
    cam.updateDirection();
    glm::vec3 dir = cam.getFront();
    EXPECT_NEAR(dir.x, 0.0f, EPSILON);
    EXPECT_NEAR(dir.z, 1.0f, EPSILON);
}

TEST(CameraTest, ViewMatrixDirectionMatchesCameraFront) {
    Camera cam(0.1f);
    cam.incPitch(20.0f);
    cam.incYaw(45.0f);

    glm::vec3 forward = cam.getFront();
    glm::vec3 target = cam.position + forward;

    glm::mat4 view = cam.getView();
    glm::vec3 camForwardInView = glm::normalize(glm::vec3(glm::inverse(view)[2]));

    // Should point in same direction as -forward
    EXPECT_NEAR(glm::dot(camForwardInView, -forward), 1.0f, 0.001f);
}

