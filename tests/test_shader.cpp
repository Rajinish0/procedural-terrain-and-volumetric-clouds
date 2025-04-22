#include <gtest/gtest.h>
#include <shader.h>
#include <fstream>
#include "empty_context.h"
#include <cstdio>

class ShaderTest : public EmptyContext {};

TEST_F(ShaderTest, LoadSourceReadsCorrectFileContent) {
    std::string filename = "../resources/test_shader.glsl";
    std::string content = "void main() {}";
    std::ofstream file(filename);
    file << content;
    file.close();

    std::string result = Shader::loadSource(filename);
    EXPECT_EQ(result, content);

    std::remove(filename.c_str());
}

TEST_F(ShaderTest, CompileShaderFailsOnInvalidCode) {
    std::string badCode = "this is not valid GLSL";
    GLuint shaderId = Shader::compileShader(GL_VERTEX_SHADER, badCode);

    GLint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    EXPECT_EQ(success, GL_FALSE);

    glDeleteShader(shaderId);
}

TEST_F(ShaderTest, ShaderProgramCompilesAndLinks) {
    Shader shader("../resources/test_vertex1.glsl", "../resources/test_frag1.glsl");

    shader.use(); // Should not crash

    EXPECT_EQ(shader.getLoc("nonexistent_uniform"), -1);
}


TEST_F(ShaderTest, WrongFileThrowsError)
{
    EXPECT_THROW
    (
    {
        Shader shader("badFile", "badFile");
    }, 
    std::runtime_error
    );
}


TEST_F(ShaderTest, GeometryShaderCompilesSuccessfully) {
    EXPECT_NO_THROW({
        Shader shader(
            "../resources/test_vertex1.glsl", 
            "../resources/test_frag1.glsl", 
            "../resources/test_geom.glsl");
    });
}

TEST_F(ShaderTest, SetUniformValues) {
    EXPECT_NO_THROW({
        Shader shader("../resources//test_vertex1.glsl", 
                      "../resources//test_frag_with_uniforms.glsl");
        shader.use();
        shader.setFloat("testFloat", 1.0f);
        shader.setInt("testInt", 42);
        shader.setBool("testBool", true);
        shader.setVec2("testVec2", glm::vec2(1.0f, 2.0f));
        shader.setVec3("testVec3", glm::vec3(1.0f, 2.0f, 3.0f));
        shader.setVec4("testVec4", glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
    });
}

TEST_F(ShaderTest, DestructorReleasesProgram) {
    GLuint progId = 0;
    {
        Shader shader("../resources/test_vertex1.glsl", 
                      "../resources/test_frag1.glsl");
        progId = shader.getId(); 
        EXPECT_TRUE(glIsProgram(progId));
    }
    
    EXPECT_FALSE(glIsProgram(progId));
}
