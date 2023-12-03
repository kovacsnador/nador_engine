#ifndef __NADOR_DEMO_ASSETS_H__
#define __NADOR_DEMO_ASSETS_H__

#include <cstdint>

namespace demo
{
    enum Fonts : uint32_t
    {
        FREE_SANS = 1,
        SUPER_MARIO_BROS_3
    };

    enum FontSize : uint32_t
    {
        SMALL  = 32,
        MEDIUM = 64,
        LARGE  = 128,
    };

    enum Sound : uint32_t
    {
        TEST_SOUND_1 = 1,
        TEST_SOUND_2,

        MARIO_THEME,
        SMB_1_UP,
        SMB_BOWSERFALLS,
        SMB_BOWSERFIRE,
        SMB_BREAKBLOCK,
        SMB_BUMP,
        SMB_COIN,
        SMB_FIREBALL,
        SMB_FIREWORKS,
        SMB_FLAGPOLE,
        SMB_GAMEOVER,
        SMB_JUMP_SMALL,
        SMB_JUMP_SUPER,
        SMB_KICK,
        SMB_MARIODIE,
        SMB_PAUSE,
        SMB_PIPE,
        SMB_POWERUP_APPEARS,
        SMB_POWERUP,
        SMB_STAGE_CLEAR,
        SMB_STOMP,
        SMB_VINE,
        SMB_WARNING,
        SMB_WORLD_CLEAR
    };
} // namespace demo

#endif //!__NADOR_DEMO_ASSETS_H__