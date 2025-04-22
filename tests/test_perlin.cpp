#include <gtest/gtest.h>
#include <perlin.h>


constexpr float EPSILON = 1e-4;
constexpr size_t OCTAVES = 4;
constexpr float FREQ_MULT = 2.0f;
constexpr float AMPL_MULT = 0.5f;
Perlin2d perlin(256, 4);

TEST(PerlinTest, NoiseIsSame)
{
    float noise1 = perlin.noise(0.1f, 0.1f);

    float noise2 = perlin.noise(0.1f, 0.1f);

    EXPECT_NEAR(noise1, noise2, EPSILON);
}

TEST(PerlinTest, NoiseIsDistinct)
{
    float noise1 = perlin.noise(0.1f, 0.1f);

    float noise2 = perlin.noise(0.1f, 0.2f);

    EXPECT_GE(std::fabs(noise1 - noise2), EPSILON);
}

TEST(PerlinTest, FBMisCorrect)
{
    float out = 0.0f,
          x = 0.1f, 
          y = 0.1f;

    float a = 1, p = 1, max = 0.0f;

    for (int i =0; i < OCTAVES; ++i)
    {
        out += p * perlin.noise(x * a, y * a);
        p *= 0.5f;
        a *= 2.0f;
        max += p;
    }

    float fbm = std::fmax(0.0f, out/max);

    EXPECT_NEAR(
        perlin.perlin(x, y),
        fbm,
        EPSILON
    );
}
