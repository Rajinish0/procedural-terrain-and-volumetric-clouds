#ifndef AUDIO_MANAGER_ITRFC_H
#define AUDIO_MANAGER_ITRFC_H

#include <iostream>

class IAudioManager 
{
public:
    IAudioManager() = default;
    virtual ~IAudioManager() = default;
    
    virtual void play2D(const std::string &filename, bool loop = false) = 0;
    virtual void update() = 0;
};

#endif;