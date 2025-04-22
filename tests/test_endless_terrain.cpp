#include <endless_terrain.h>
#include "empty_context.h"

class EndlessTerrainTest : public EmptyContext {};


TEST(TerrainGenerationTest, ChunkDataHasCorrectSizeAndValues) {
    int size = 16;
    auto chunk = generateChunkData(size, glm::vec2(0.0f));

    // Check correct number of values
    ASSERT_EQ(chunk.heights_and_normals.size(), size * size);

    for (auto& hn : chunk.heights_and_normals) {
        float h = hn.x;
        ASSERT_GE(h, -1.0f);
        ASSERT_LE(h, 1.0f);

        // Normal values should be in [0,1] range due to normalization and mapping
        ASSERT_GE(hn.y, 0.0f);
        ASSERT_LE(hn.y, 1.0f);
        ASSERT_GE(hn.z, 0.0f);
        ASSERT_LE(hn.z, 1.0f);
        ASSERT_GE(hn.w, 0.0f);
        ASSERT_LE(hn.w, 1.0f);
    }
}

TEST_F(EndlessTerrainTest, HeightMapWrapperReceivesDataCorrectly) {
    HeightMapWrapper wrapper(16, glm::vec2(0.0f));

    // Wait briefly for async task to complete
    for (int i = 0; i < 100; ++i) {
        wrapper.update();
        if (wrapper.ready()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_TRUE(wrapper.ready());
}


TEST(TerrainGenerationTest, AdjacentChunksAlignHorizontally) {
    int size = 241;
    float offset = size - 1;
    
    auto leftChunk = generateChunkData(size, glm::vec2(99.0f*offset, offset));
    auto rightChunk = generateChunkData(size, glm::vec2(100.0f*offset, offset));

    for (int i = 0; i < size; ++i) {
        glm::vec4 left = leftChunk.heights_and_normals[funcs::flatten(i, size - 1, size)];
        glm::vec4 right = rightChunk.heights_and_normals[funcs::flatten(i, 0, size)];
        ASSERT_NEAR(left.x, right.x, 0.01f) << "Mismatch at row " << i << " (height)";
        ASSERT_NEAR(left.y, right.y, 0.05f) << "Mismatch at row " << i << " (normal x)";
        ASSERT_NEAR(left.z, right.z, 0.05f) << "Mismatch at row " << i << " (normal y)";
        ASSERT_NEAR(left.w, right.w, 0.05f) << "Mismatch at row " << i << " (normal z)";
    }
}

TEST(TerrainGenerationTest, AdjacentChunksAlignVertically) {
    int size = 241;
    float offset = size - 1;
    
    auto topChunk = generateChunkData(size, glm::vec2(0.0f, -99.0f*offset));
    auto bottomChunk = generateChunkData(size, glm::vec2(0.0f, -100.0f*offset));

    for (int j = 0; j < size; ++j) {
        glm::vec4 top = topChunk.heights_and_normals[funcs::flatten(size - 1, j, size)];
        glm::vec4 bottom = bottomChunk.heights_and_normals[funcs::flatten(0, j, size)];

        ASSERT_NEAR(top.x, bottom.x, 0.01f) << "Mismatch at column " << j << " (height)";
        ASSERT_NEAR(top.y, bottom.y, 0.05f) << "Mismatch at column " << j << " (normal x)";
        ASSERT_NEAR(top.z, bottom.z, 0.05f) << "Mismatch at column " << j << " (normal y)";
        ASSERT_NEAR(top.w, bottom.w, 0.05f) << "Mismatch at column " << j << " (normal z)";
    }
}

