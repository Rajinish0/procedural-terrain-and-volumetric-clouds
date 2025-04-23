#include <gtest/gtest.h>
#include <sprite_renderer.h>
#include <engine_consts.h>
#include <empty_context.h>


constexpr char *SPRITE_PATH = "../resources/grass.jpg";
constexpr float EPSILON = 1e-4;

class SpriteTest : public EmptyContext {};

TEST_F(SpriteTest, TestRotationMatrixIsCorrect)
{
    const float size = 2.0f;
    const float theta = glm::radians(45.0f);
    Sprite sprite{SPRITE_PATH, size};
    glm::mat4 mat1 = sprite.getRotationMatrix(theta);

    glm::mat4 mat2(1.0f);
    mat2 = glm::translate(mat2, glm::vec3(size/2.0f, size/2.0f, 0.0f));
    mat2 = glm::rotate(mat2, theta, Z_AXIS);
    mat2 = glm::translate(mat2, glm::vec3(-size/2.0f, -size/2.0f, 0.0f));

    for (int i =0; i < 4; ++i)
        for (int j =0; j < 4; ++j)
            ASSERT_NEAR(mat1[i][j], mat2[i][j], EPSILON);
}



TEST_F(SpriteTest, TestRotationMatrixIsCorrectAfterSizeChange)
{
    float size = 2.0f;
    const float theta = glm::radians(45.0f);
    Sprite sprite{SPRITE_PATH, size};

    size = 4.0f;
    sprite.setSize(size);
    
    glm::mat4 mat1 = sprite.getRotationMatrix(theta);


    glm::mat4 mat2(1.0f);
    mat2 = glm::translate(mat2, glm::vec3(size/2.0f, size/2.0f, 0.0f));
    mat2 = glm::rotate(mat2, theta, Z_AXIS);
    mat2 = glm::translate(mat2, glm::vec3(-size/2.0f, -size/2.0f, 0.0f));

    for (int i =0; i < 4; ++i)
        for (int j =0; j < 4; ++j)
            ASSERT_NEAR(mat1[i][j], mat2[i][j], EPSILON);
}