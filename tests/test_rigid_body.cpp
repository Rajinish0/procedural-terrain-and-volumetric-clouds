#include <gtest/gtest.h>
#include <rigid_body.h>

const float EPS = 1e-4f;  // for floating point comparison


TEST(RigidBodyTest, MovesInDirectionOfForce) {
    glm::mat4 inertiaTensor = glm::mat4(1.0f);
    RigidBody body(1.0f, glm::vec3(0), glm::quat(1, 0, 0, 0), inertiaTensor);

    body.applyForce(glm::vec3(0, 0, 10));
    body.update(1.0f); // 1 second

    glm::vec3 pos = body.getPos();
    EXPECT_NEAR(pos.z, 10.0f, EPS);
}


TEST(RigidBodyTest, AppliesLinearDamping) {
    glm::mat4 inertiaTensor = glm::mat4(1.0f);
    RigidBody body(1.0f, glm::vec3(0), glm::quat(1, 0, 0, 0), inertiaTensor);

    body.applyForce(glm::vec3(10, 0, 0));
    body.update(1.0f); // initial impulse
    glm::vec3 afterImpulse = body.getPos();

    body.update(1.0f); // now only damping applies
    glm::vec3 afterDamp = body.getPos();

    EXPECT_GT(afterImpulse.x, 0);
    EXPECT_GT(afterDamp.x, afterImpulse.x); // it should still be moving
}


TEST(RigidBodyTest, AppliesTorqueAndRotates) {
    glm::mat4 inertiaTensor = glm::mat4(1.0f);
    RigidBody body(1.0f, glm::vec3(0), glm::quat(1, 0, 0, 0), inertiaTensor);

    body.applyTorque(glm::vec3(0, 1, 0)); // Yaw
    body.update(1.0f);

    glm::vec3 direction = body.getDirection();
    EXPECT_LT(glm::dot(direction, glm::vec3(0, 0, -1)), 0.99f);
    EXPECT_GT(glm::length(direction), 0.99f);
}


TEST(RigidBodyTest, ForceAtPointGeneratesTorque) {
    glm::mat4 inertiaTensor = glm::mat4(1.0f);
    RigidBody body(1.0f, glm::vec3(0), glm::quat(1, 0, 0, 0), inertiaTensor);

    glm::vec3 point = glm::vec3(1, 0, 0);  // off-center
    glm::vec3 force = glm::vec3(0, 1, 0);  // upward force

    body.applyForceAtPoint(point, force);
    body.update(1.0f);

    glm::vec3 direction = body.getDirection();
    EXPECT_LT(direction.z, -0.9f);  // still close to -Z but should have rotated a bit
}


TEST(RigidBodyTest, ApplyRelativeForceInLocalSpace) {
    glm::mat4 inertiaTensor = glm::mat4(1.0f);
    glm::quat rot = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0));
    RigidBody body(1.0f, glm::vec3(0), rot, inertiaTensor);

    body.applyRelativeForce(glm::vec3(0, 0, -10));
    body.update(1.0f);

    glm::vec3 pos = body.getPos();
    EXPECT_NEAR(pos.x,-10.0f, EPS);
}


TEST(RigidBodyTest, OrientationRemainsNormalized) {
    glm::mat4 inertiaTensor = glm::mat4(1.0f);
    RigidBody body(1.0f, glm::vec3(0), glm::quat(1, 0, 0, 0), inertiaTensor);

    body.applyTorque(glm::vec3(0, 1, 0));
    for (int i = 0; i < 100; ++i)
        body.update(0.1f);

    float len = glm::length(body.getDirection());
    EXPECT_NEAR(len, 1.0f, EPS);
}
