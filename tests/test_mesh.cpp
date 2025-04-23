#include <gtest/gtest.h>
#include "mesh.h"
#include "empty_context.h"
#include "funcs.h"

class MeshTest : public EmptyContext {};

TEST_F(MeshTest, ThrowsOnInvalidTextureType) {    
    std::vector<Vertex> verts{
        {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)}
    };
    std::vector<unsigned int> indices = {0};
    Texture invalidTex;
    invalidTex.type = static_cast<Texture::Type>(999); // Invalid type
    invalidTex.id = 1;

    
    Mesh mesh(verts, {invalidTex}, indices);


    Shader dummyShader{"../resources/test_vertex1.glsl",
                       "../resources/test_frag1.glsl"};


    EXPECT_THROW({mesh.draw(dummyShader);}, InvalidTexture);
}


TEST_F(MeshTest, MoveConstructorTransfersResources) {
    Mesh mesh1 = funcs::genPlane2(10, 1);
    GLuint originalVAO = mesh1.VAO;

    Mesh mesh2 = std::move(mesh1);

    EXPECT_EQ(mesh1.VAO, 0);
    EXPECT_EQ(mesh2.VAO, originalVAO);
}
