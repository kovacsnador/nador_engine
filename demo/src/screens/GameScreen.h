#ifndef __NADOR_DEMO_GAME_SCREEN_H__
#define __NADOR_DEMO_GAME_SCREEN_H__

#include "nador/ui/UiTextLabel.h"
#include "nador/ui/UiScene.h"
#include "nador/ui/UiAligner.h"

namespace demo
{
    /*class GameScreen
    {
        struct InGameUi : public nador::UiScene
        {
            InGameUi(const glm::ivec2&        position,
                     const glm::ivec2&        size,
                     nador::UiAlignment       alignment,
                     const nador::FontPtr&    font)
            : nador::UiScene(position, size, alignment)
            , aligner(nador::EAligner::HORIZONTAL, 0)
            {
            }

            nador::UiAligner   aligner;

            nador::UiScene label1;
            nador::UiScene label2;
            nador::UiScene label3;
            nador::UiScene label4;

            nador::UiTextLabel lifesLeft;
            nador::UiTextLabel points;

            nador::UiTextLabel coints;

            nador::UiTextLabel world;
            nador::UiTextLabel worldCount;

            nador::UiTextLabel time;
            nador::UiTextLabel timeLeft;
        };

    public:
    private:

    };*/
} // namespace demo

#endif //!__NADOR_DEMO_GAME_SCREEN_H__