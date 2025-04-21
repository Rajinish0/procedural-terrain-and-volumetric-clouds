#include <gtest/gtest.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class EmptyContext : public ::testing::Test {
    protected:
        GLFWwindow* window;
        
        virtual void SetUp() override {
            ASSERT_TRUE(glfwInit()) << "Failed to initialize GLFW";
            
            // Configure GLFW for hidden window
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            
            // Create hidden window
            window = glfwCreateWindow(1, 1, "Test Window", NULL, NULL);
            ASSERT_NE(window, nullptr) << "Failed to create GLFW window";
            
            // Make OpenGL context current
            glfwMakeContextCurrent(window);
            
            // Initialize GLAD
            ASSERT_TRUE(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
                << "Failed to initialize GLAD";
        }
        
        virtual void TearDown() override 
        {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    };