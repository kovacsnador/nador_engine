#include <regex>

#include <AL/al.h>
#include <AL/alc.h>

#include "dr_libs/dr_wav.h"
#include "dr_libs/dr_mp3.h"

#include "nador/system/IFileController.h"
#include "nador/log/Log.h"
#include "nador/sound/open_al/OpenALSoundController.h"
#include "nador/common/ThreadPool.h"
#include "nador/utils/Utils.h"

// OpenAL error checking
#define OpenAL_ErrorCheck(message)                                                                                                                   \
    {                                                                                                                                                \
        ALenum error = alGetError();                                                                                                                 \
        if (error != AL_NO_ERROR)                                                                                                                    \
        {                                                                                                                                            \
            ENGINE_FATAL("OpenAL Error: %d with call for %s", error, #message);                                                                      \
        }                                                                                                                                            \
    }

#define alec(FUNCTION_CALL)                                                                                                                          \
    FUNCTION_CALL;                                                                                                                                   \
    OpenAL_ErrorCheck(FUNCTION_CALL)

namespace nador
{
    OpenAlSoundContoller::OpenAlSoundContoller(const IFileControllerPtr fileCtrl)
    : _fileCtrl(fileCtrl)
    {
        NADOR_ASSERT(_fileCtrl);

        // Create device
        const ALCchar* defaultDeviceString = alcGetString(/*device*/ nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);

        _pDevice = alcOpenDevice(defaultDeviceString);
        if (!_pDevice)
        {
            ENGINE_FATAL("Failed to get the default device for OpenAL.");
            return;
        }
        ENGINE_DEBUG("OpenAL Device: %s", alcGetString(_pDevice, ALC_DEVICE_SPECIFIER));

        // Create context
        _pContext = alcCreateContext(_pDevice, /*attrlist*/ nullptr);

        // Get the max source number
        ALCint size;
        alcGetIntegerv(_pDevice, ALC_ATTRIBUTES_SIZE, 1, &size);
        std::vector<ALCint> attrs(size);
        alcGetIntegerv(_pDevice, ALC_ALL_ATTRIBUTES, size, &attrs[0]);
        for (auto it : attrs)
        {
            if (it == ALC_MONO_SOURCES)
            {
                _maxSoundSource = it;
            }
        }

        ENGINE_DEBUG("Max mono source: %d", _maxSoundSource);

        // Activate this context so that OpenAL state modifications are applied to the context
        if (!alcMakeContextCurrent(_pContext))
        {
            ENGINE_FATAL("Failed to make the OpenAL context the current context.");
            return;
        }
        OpenAL_ErrorCheck("Make context current.");

        // Create a listener in 3d space (ie the player); (there always exists as listener, you just configure data on it)
        alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
        alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
        ALfloat forwardAndUpVectors[] = { /*forward = */ 1.f, 0.f, 0.f,
                                          /* up = */ 0.f,     1.f, 0.f };
        alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));

        ENGINE_DEBUG("OpenAlSoundControl initialized.");
    }

    OpenAlSoundContoller::~OpenAlSoundContoller()
    {
        Wait();

        {
            std::scoped_lock<std::mutex> lock(_mtx);
            _sounds.clear();
        }

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(_pContext);
        alcCloseDevice(_pDevice);

        ENGINE_DEBUG("OpenAlSoundControl destroyed.");
    }

    void OpenAlSoundContoller::TickBegin()
    {
        // remove sounds if not playing any more.
        _currentSoundSources.remove_if([](const ISoundSourcePtr& s) { return s == nullptr || s->GetState() != ESoundSourceState::PLAYING; });

        // remove pending sound loads
        std::remove_if(_pendingSoundLoading.begin(), _pendingSoundLoading.end(), [this](const auto& it) { return nador::utils::isReadyFuture(it.obj); });
    }

    bool OpenAlSoundContoller::LoadSound(const char* filePath, uint32_t soundId)
    {
        auto threadPool = GetThreadPool();

        auto addSound = [this](std::string filePath, uint32_t soundId) {
            bool isMp3 = std::regex_match(filePath, std::regex("(.*)\\.mp3$"));
            bool isWav = std::regex_match(filePath, std::regex("(.*)\\.wav$"));

            if (!isMp3 && !isWav)
            {
                ENGINE_WARNING("Sound file extension not supported: %s", filePath.c_str());
                return false;
            }

            auto soundFile = _fileCtrl->Read(filePath);

            SoundPtr soundPtr  = std::make_shared<Sound>();
            soundPtr->soundId  = soundId;
            soundPtr->fileName = filePath;

            int16_t* pcmData = nullptr;

            if (isWav)
            {
                pcmData = drwav_open_memory_and_read_pcm_frames_s16(
                    soundFile->Begin(), soundFile->GetSize(), &soundPtr->channels, &soundPtr->sampleRate, &soundPtr->totalPcmFrameCount, nullptr);
            }
            else if (isMp3)
            {
                drmp3_config mp3Config;
                pcmData = drmp3_open_memory_and_read_pcm_frames_s16(
                    soundFile->Begin(), soundFile->GetSize(), &mp3Config, &soundPtr->totalPcmFrameCount, nullptr);

                soundPtr->channels   = mp3Config.channels;
                soundPtr->sampleRate = mp3Config.sampleRate;
            }

            if (pcmData == nullptr)
            {
                ENGINE_WARNING("Failed to load audio file: %s", filePath.c_str());
                return false;
            }

            std::scoped_lock<std::mutex> lock(_mtx);

            alec(alGenBuffers(1, &soundPtr->bufferId));
            alec(alBufferData(soundPtr->bufferId,
                              soundPtr->channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
                              pcmData,
                              (ALsizei)soundPtr->getPcmDataSize(),
                              soundPtr->sampleRate));

            // delete pcmData
            drwav_free(pcmData, nullptr);

            ENGINE_DEBUG("Sound added from file %s with id %d", filePath.c_str(), soundId);

            auto [_, inserted] = _sounds.insert_or_assign(soundId, soundPtr);

            return inserted; // on "inserted" true on "assign" false
        };

        auto future = threadPool->Enqueue(addSound, ETaskPriority::HIGH, filePath, soundId);
        _pendingSoundLoading.emplace_back(std::move(future));
        return true;
    }

    ISoundSourceUPtr OpenAlSoundContoller::CreateSoundSource(uint32_t soundId)
    {
        std::scoped_lock<std::mutex> lock(_mtx);

        auto iter = _sounds.find(soundId);

        if (iter == _sounds.end())
        {
            ENGINE_WARNING("Sound %d not found", soundId);
            return nullptr;
        }

        uint32_t sourceId;
        alec(alGenSources(1, &sourceId));
        // alec(alSource3f(stereoSource, AL_POSITION, 0.f, 0.f, 1.f)); //NOTE: this does not work like mono sound positions!
        // alec(alSource3f(stereoSource, AL_VELOCITY, 0.f, 0.f, 0.f));
        alec(alSourcef(sourceId, AL_PITCH, 1.f));
        alec(alSourcef(sourceId, AL_GAIN, 1.f));
        alec(alSourcei(sourceId, AL_LOOPING, AL_FALSE));
        alec(alSourcei(sourceId, AL_BUFFER, iter->second->bufferId));

        return std::make_unique<SoundSourceImpl>(soundId, sourceId, &_onStopAllSoundEvent);
    }

    void OpenAlSoundContoller::PlaySound(uint32_t soundId)
    {
        auto soundSource = CreateSoundSource(soundId);
        if (soundSource)
        {
            soundSource->Play();
            _currentSoundSources.push_back(std::move(soundSource));
        }
    }

    void OpenAlSoundContoller::StopAllSound()
    {
        _onStopAllSoundEvent();
    }

    void OpenAlSoundContoller::Wait()
    {
        _pendingSoundLoading.clear();
    }

    sound_data_list_t OpenAlSoundContoller::GetAllSoundData() const
    {
        std::scoped_lock<std::mutex> lock(_mtx);

        sound_data_list_t allSoundData;

        for (auto& it : _sounds)
        {
            allSoundData.emplace_back(it.second->fileName, it.second->soundId);
        }

        return allSoundData;
    }

    OpenAlSoundContoller::Sound::~Sound()
    {
        alec(alDeleteBuffers(1, &bufferId));
    }

    OpenAlSoundContoller::SoundSourceImpl::SoundSourceImpl(uint32_t soundId, uint32_t sourceId, onStopAllSound_event_t* event)
    : onStopAllSound_listener_t(event, std::bind(&OpenAlSoundContoller::SoundSourceImpl::Stop, this))
    , soundId(soundId)
    , sourceId(sourceId)
    {
    }

    OpenAlSoundContoller::SoundSourceImpl::~SoundSourceImpl()
    {
        alec(alDeleteSources(1, &sourceId));
    }

    void OpenAlSoundContoller::SoundSourceImpl::Play() const
    {
        alec(alSourcePlay(sourceId));
    }

    void OpenAlSoundContoller::SoundSourceImpl::Pause() const
    {
        alec(alSourcePause(sourceId));
    }

    void OpenAlSoundContoller::SoundSourceImpl::Rewind() const
    {
        alec(alSourceRewind(sourceId));
    }

    void OpenAlSoundContoller::SoundSourceImpl::Stop() const
    {
        alec(alSourceStop(sourceId));
    }

    ESoundSourceState OpenAlSoundContoller::SoundSourceImpl::GetState() const
    {
        ALenum state;
        alec(alGetSourcei(sourceId, AL_SOURCE_STATE, &state));

        switch (state)
        {
        case AL_PLAYING:
            return ESoundSourceState::PLAYING;
        case AL_INITIAL:
            return ESoundSourceState::INITIAL;
        case AL_PAUSED:
            return ESoundSourceState::PAUSED;
        case AL_STOPPED:
            return ESoundSourceState::STOPPED;
        default:
            return ESoundSourceState::NONE;
        }
    }

    void OpenAlSoundContoller::SoundSourceImpl::SetLooping(bool looping)
    {
        alec(alSourcei(sourceId, AL_LOOPING, looping));
    }

    void OpenAlSoundContoller::SoundSourceImpl::SetPosition(const glm::vec3& position)
    {
        alec(alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z));
    }

    glm::vec3 OpenAlSoundContoller::SoundSourceImpl::GetPosition() const
    {
        glm::vec3 position;
        alec(alGetSource3f(sourceId, AL_POSITION, &position.x, &position.y, &position.z));
        return position;
    }

    bool OpenAlSoundContoller::SoundSourceImpl::IsLooping() const
    {
        int32_t isLooping = 0;
        alec(alGetSourcei(sourceId, AL_LOOPING, &isLooping));
        return isLooping;
    }

    uint32_t OpenAlSoundContoller::SoundSourceImpl::GetSoundId() const
    {
        return soundId;
    }

    uint32_t OpenAlSoundContoller::SoundSourceImpl::GetSourceId() const
    {
        return sourceId;
    }
} // namespace nador