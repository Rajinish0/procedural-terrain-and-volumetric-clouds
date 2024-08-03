#include "audio_manager.h"
#include <iostream>

AudioManager::AudioManager()
    :currentPlaying(), isReady(false)
{
    this->isReady = this->initOpenAL();        
}

bool AudioManager::initOpenAL(){
    this->device = alcOpenDevice(nullptr);
    if (!this->device){
        std::cerr << "Failed to open OPENAL device" << std::endl;
        return false;
    }

    this->context = alcCreateContext(this->device, nullptr);
    if (!alcMakeContextCurrent(this->context)){
        std::cerr << "Failed to set context" << std::endl;
        return false;
    }

    return true;
}

void AudioManager::play2D(std::string filename, bool loop){

    if (!this->isReady){
        std::cerr << "Can't play audio; openAL was not initialized correctly" << std::endl;
        return;
    }
    SF_INFO sfinfo;
    SNDFILE* sndfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!sndfile){
        std::cerr << "Failed to open audio file " << filename << std::endl;
        return;
    }
    //im going to assume 16 bit format for now
    ALenum format;
    if (sfinfo.channels == 1){
        format = AL_FORMAT_MONO16;
    }
    else if (sfinfo.channels == 2){
        format = AL_FORMAT_STEREO16;
    }
    else{
        std::cerr << "unsupported num of channels in file " << filename << std::endl;
        return;
    }

    std::cout << "FRAMES: " << sfinfo.frames << std::endl;
    std::cout << "samplerat: " << sfinfo.samplerate << std::endl;
    /*
    i could get the duration like this and delete it after that much time has passed
    sfinfo.frame / sfinfo.samplerate
    but what im doing rn is simpler
    */
    std::vector<ALshort> bufferData(sfinfo.frames * sfinfo.channels);
    sf_read_short(sndfile, bufferData.data(), bufferData.size());
    sf_close(sndfile);


    ALuint *buffer = new ALuint;
    alGenBuffers(1, buffer);
    alBufferData(*buffer, format, bufferData.data(), bufferData.size() * sizeof(ALshort), sfinfo.samplerate);

    ALuint *source = new ALuint;
    alGenSources(1, source);
    alSourcei(*source, AL_BUFFER, *buffer);

    currentPlaying.push_back( _buf_source_loop(buffer, source, loop) );

    alSourcePlay(*source);
}

void AudioManager::update(){
    auto it = currentPlaying.begin();
    ALint state;
    ALuint *source;
    ALuint *buffer;
    while (it != currentPlaying.end()){
        source = std::get<1>(*it);
        buffer = std::get<0>(*it);
        alGetSourcei(*source, AL_SOURCE_STATE, &state);
        if (state == AL_STOPPED && !std::get<2>(*it)){
            alSourcei(*source, AL_BUFFER, 0);
            alDeleteBuffers(1, buffer);
            alDeleteSources(1, source);
            delete source;
            delete buffer;
            it = currentPlaying.erase(it);
            continue;
        }
        else if (state == AL_STOPPED && std::get<2>(*it)){
            alSourcePlay(*source);
        }
        ++it;
    }
}

AudioManager::~AudioManager(){
    for (auto it = currentPlaying.begin(); it != currentPlaying.end(); ++it){
        ALuint* source = std::get<1>(*it);
        ALuint* buffer = std::get<0>(*it);


        ALint state;
        alGetSourcei(*source, AL_SOURCE_STATE, &state);
        if (state == AL_PLAYING) {
            alSourceStop(*source);
        }
        alSourcei(*source, AL_BUFFER, 0);
        alDeleteBuffers(1, buffer );
        alDeleteSources(1, source );

        delete buffer;
        delete source;
    }
    currentPlaying.clear();
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
