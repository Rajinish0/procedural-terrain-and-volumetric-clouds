#ifndef MOCK_AUDIO_MANAGER_H
#define MOCK_AUDIO_MANAGER_H

#include <interfaces/audio_manager_interface.h>
#include <gmock/gmock.h>
#include <string>

class MockAudioManager : public IAudioManager {
public:
    // Syntax: MOCK_METHOD(return_type, method_name, (argument_types), (modifiers));
    MOCK_METHOD(void, play2D, (const std::string& filename, bool loop), (override));
    MOCK_METHOD(void, update, (), (override));
};

#endif