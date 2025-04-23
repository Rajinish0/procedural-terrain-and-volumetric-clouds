#include <gtest/gtest.h>
#include <textrender.h>
#include "empty_context.h"


class TextRendererTest : public EmptyContext {
protected:
	TextRenderer textRenderer{"../resources/arial.ttf"};
	Shader shader{"../resources/test_vertex1.glsl",
				  "../resources/test_frag1.glsl"};
};

TEST_F(TextRendererTest, CustomFontConstructor) {
	EXPECT_THROW({ TextRenderer renderer("fonts/custom.ttf"); },
				   RenderTextException);
}

TEST_F(TextRendererTest, AddCharacterToStorage) {
		int result = textRenderer.addCharacterToStorage('A');
		EXPECT_EQ(result, 0) << "Character should be added successfully";
}

TEST_F(TextRendererTest, RenderTextModes) {
    textRenderer.renderText(shader, "Test", 0.0f, 0.0f, 1.0f, glm::vec3(1.0f), TextRenderer::TOP_LEFT);
    textRenderer.renderText(shader, "Test", 0.0f, 0.0f, 1.0f, glm::vec3(1.0f), TextRenderer::CENTER);
    textRenderer.renderText(shader, "Test", 0.0f, 0.0f, 1.0f, glm::vec3(1.0f), TextRenderer::BOTTOM_LEFT);
    textRenderer.renderText(shader, "Test", 0.0f, 0.0f, 1.0f, glm::vec3(1.0f), TextRenderer::TOP_RIGHT);
    textRenderer.renderText(shader, "Test", 0.0f, 0.0f, 1.0f, glm::vec3(1.0f), TextRenderer::BOTTOM_RIGHT);
}

TEST_F(TextRendererTest, RenderLongText) 
{
        std::string longText = "This is a very long text that should test the renderer's ability to handle multiple characters in sequence";
        textRenderer.renderText(shader, longText, 0.0f, 0.0f, 1.0f, glm::vec3(1.0f), TextRenderer::CENTER);
}