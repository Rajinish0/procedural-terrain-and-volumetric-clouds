#include <gtest/gtest.h>
#include <memory>
#include "airplane.h"
#include <iostream>
#include <filesystem>
#include "mock_audio_manager.h"
#include "empty_context.h"

class AirplaneTest : public EmptyContext{
protected:
    std::shared_ptr<MockAudioManager> mockAudio;
    std::shared_ptr<IAudioManager> mockAudioInterface;

    void SetUp() override {
        EmptyContext::SetUp();
        mockAudio = std::make_shared<MockAudioManager>();
        mockAudioInterface = mockAudio;
    }

    Airplane createAirplane() {
        AirplaneParameters params {
            "../models/plane/a22.obj",
            "../models/air_drop/air_drop.obj",
            "../audio/aircraft.wav",
            "../audio/parachute.wav",
            mockAudio
        };
        return Airplane(params);

    }
};

TEST_F(AirplaneTest, InitializesWithCorrectDefaults) {
    EXPECT_CALL(*mockAudio, play2D("../audio/aircraft.wav", true)).Times(1);
    Airplane airplane = createAirplane();
    EXPECT_EQ(airplane.getTotalForce().z < 0.0f, true);
}

TEST_F(AirplaneTest, DropPackageAddsPacket) {
    EXPECT_CALL(*mockAudio, play2D("../audio/aircraft.wav", true)).Times(1);
    EXPECT_CALL(*mockAudio, play2D("../audio/parachute.wav", false)).Times(1);
    Airplane airplane = createAirplane();

    airplane.bindOnDrop([]() {});
    airplane.update(1.0f); // no packet dropped yet

    EXPECT_EQ(airplane.getPackets().size(), 0);

    // Simulate drop
    airplane.dropPackage = true;
    airplane.update(1.0f); // should drop

    EXPECT_EQ(airplane.getPackets().size(), 1);
    // EXPECT_TRUE(mockAudio->wasPlayed("packet.wav"));
}

TEST_F(AirplaneTest, CooldownPreventsRapidDrop) {
    Airplane airplane = createAirplane();
    airplane.bindOnDrop([]() {});

    airplane.dropPackage = true;
    airplane.update(1.0f);
    size_t countAfterDrop = airplane.getPackets().size();
    EXPECT_EQ(countAfterDrop, 1);

    airplane.dropPackage = true;
    airplane.update(0.1f);
    EXPECT_EQ(airplane.getPackets().size(), 0);
}
