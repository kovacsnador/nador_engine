#ifndef __OPEN_AL_SOUND_CONTROLLER_H__
#define __OPEN_AL_SOUND_CONTROLLER_H__

#include <string>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <future>
#include <list>

#include "nador/sound/ISoundController.h"
#include "nador/utils/event/Event.h"
#include "nador/utils/NonCopyable.h"
#include "nador/common/MoveableObjWrapper.h"
#include "nador/common/FutureWaiter.h"

struct ALCdevice;
struct ALCcontext;

namespace nador
{
    class IFileController;

    class OpenAlSoundContoller : public ISoundController
    {
        CREATE_EVENT_TYPE_ARG_0(onStopAllSound);

        struct Sound : private NonCopyable
        {
            ~Sound();

            uint64_t getPcmDataSize() { return totalPcmFrameCount * channels * sizeof(int16_t); }

            uint32_t    soundId  = UINT32_MAX;
            std::string fileName = "";

            uint32_t channels           = 0;
            uint32_t sampleRate         = 0;
            uint64_t totalPcmFrameCount = 0;

            uint32_t bufferId = 0;
        };
        CREATE_PTR_TYPES(Sound);

        struct SoundSourceImpl : public ISoundSource, public onStopAllSound_listener_t
        {
            SoundSourceImpl(uint32_t sourceId, uint32_t soundId, onStopAllSound_event_t& event);
            ~SoundSourceImpl();

            void Play() const override;
            void Pause() const override;
            void Rewind() const override;
            void Stop() const override;

            ESoundSourceState GetState() const override;

            void SetLooping(bool looping) override;
            void SetPosition(const glm::vec3& position) override;

            glm::vec3 GetPosition() const override;
            bool      IsLooping() const override;

            uint32_t GetSoundId() const override;
            uint32_t GetSourceId() const override;

            uint32_t soundId;
            uint32_t sourceId;
        };

        using sound_list_t       = std::unordered_map<uint32_t, SoundPtr>;
        using soundSource_list_t = std::list<ISoundSourcePtr>;

    public:
        /*!
         * OpenAlSoundController default constructor.
         */
        OpenAlSoundContoller(const IFileControllerPtr fileCtrl);

        /*!
         * OpenAlSoundController destructor.
         */
        ~OpenAlSoundContoller();

        void TickBegin() override;

        /*!
         * Add sound to the sound contoller with the given sound id.
         *
         * \param fileName    The sound file name.
         * \param soundId     The sound id.
         */
        bool LoadSound(const char* fileName, uint32_t soundId) override;

        /*!
         * Creates a sound source from the sound id.
         *
         * \param soundId     The sound id.
         *
         * \return		The new Sound source.
         */
        ISoundSourceUPtr CreateSoundSource(uint32_t soundId) override;

        /*!
         * Plays the sound by the ID.
         *
         * \param soundId     The sound id.
         */
        void PlaySound(uint32_t soundId) override;

        /*!
         * Stops all sounds.
         */
        void StopAllSound() override;

        void Wait() override;

        /*!
         * Gets all registered sound data.
         *
         * \return	All sound data.
         */
        sound_data_list_t GetAllSoundData() const override;

    private:
        const IFileControllerPtr _fileCtrl;

        ALCdevice*  _pDevice { nullptr };
        ALCcontext* _pContext { nullptr };

        sound_list_t       _sounds;
        soundSource_list_t _currentSoundSources;

        uint32_t _maxSoundSource { 32 }; // the default value

        onStopAllSound_event_t _onStopAllSoundEvent;

        mutable std::mutex _mtx;

        std::vector<MoveableObjWrapper<std::future<bool>, FutureWaiter>> _pendingSoundLoading;
    };
} // namespace nador

#endif // !__OPEN_AL_SOUND_CONTROLLER_H__
