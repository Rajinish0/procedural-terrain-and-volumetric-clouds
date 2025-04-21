#ifndef AUDIO_MGR_H
#define AUDIO_MGR_H

#include "AL/al.h"
#include "AL/alc.h"
#include "sndfile.h"
#include <map>
#include <tuple>
#include <string>
#include <vector>
#include <interfaces/audio_manager_interface.h>

typedef std::tuple<ALuint*, ALuint*, bool> _buf_source_loop;

/*
    SNDFILE has some serious problems loading mp3 files
    so this only works with .wav for now
*/

class AudioManager : public IAudioManager
{
public:
    AudioManager();
    ~AudioManager();

    bool isReady;
    void play2D(const std::string &filename, bool loop = false) override;
    void update() override;

private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    bool initOpenAL();
    std::vector<_buf_source_loop> currentPlaying;
};


#endif
