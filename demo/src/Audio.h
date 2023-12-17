#ifndef __NADOR_DEMO_AUDIO_H__
#define __NADOR_DEMO_AUDIO_H__

#include "Assets.h"
#include "nador/sound/ISoundController.h"

namespace demo
{
    class Audio
    {
    public:
        static void Init(nador::ISoundController* soundCtrl)
        {
            _soundCtrl = soundCtrl;
        }

        static void Play(Sound soundId)
        {
            if(_soundCtrl)
            {
                _soundCtrl->PlaySound(soundId);
            }
        }

    private:
        static inline nador::ISoundController* _soundCtrl{nullptr};
    };

} // namespace demo


#endif //!__NADOR_DEMO_AUDIO_H__