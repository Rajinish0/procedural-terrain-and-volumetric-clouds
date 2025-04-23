#include <gtest/gtest.h>
#include <framebuffer.h>
#include "empty_context.h"

// Test suite
class FrameBufferTest : public EmptyContext {};
    
// Test 1: Check if framebuffer initializes without errors
TEST_F(FrameBufferTest, InitializesCorrectly) {
    FrameBuffer fb;
    EXPECT_NE(fb.FBO, 0);
    EXPECT_NE(fb.textureId, 0);
    EXPECT_NE(fb.depthTextureId, 0);
}

// Test 2: Framebuffer should be complete after construction
TEST_F(FrameBufferTest, FramebufferCompleteness) {
    FrameBuffer fb;
    fb.Bind();
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    EXPECT_EQ(status, GL_FRAMEBUFFER_COMPLETE);
    fb.unBind();
}

// Test 3: Clear color should be settable
TEST_F(FrameBufferTest, ClearColorChange) {
    FrameBuffer fb;
    glm::vec4 newColor(0.5f, 0.2f, 0.7f, 1.0f);
    fb.setClearColor(newColor);

    // Bind and force clear to test if OpenGL accepted the value
    fb.Bind();

    GLfloat clearColor[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    
    EXPECT_FLOAT_EQ(clearColor[0], newColor.r);
    EXPECT_FLOAT_EQ(clearColor[1], newColor.g);
    EXPECT_FLOAT_EQ(clearColor[2], newColor.b);
    EXPECT_FLOAT_EQ(clearColor[3], newColor.a);

    fb.unBind();
}