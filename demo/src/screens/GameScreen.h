#ifndef __NADOR_DEMO_GAME_SCREEN_H__
#define __NADOR_DEMO_GAME_SCREEN_H__

#include "nador/common/GlobalListener.h"
#include "nador/ui/UiTextLabel.h"
#include "nador/ui/UiScene.h"
#include "nador/ui/UiAligner.h"

#include "gameplay/World.h"

namespace demo
{
    class GameScreen : protected nador::GlobalListener
    {
        /*struct InGameUi : public nador::UiScene
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
        };*/

    public:
        GameScreen(World world)
        : _world(std::move(world))
        {
        }

        void Start();
        void Stop();

        void OnTick(float_t) override;
        void OnRender(nador::IRenderer* renderer) override;

    private:
        World _world;

    };
} // namespace demo

#endif //!__NADOR_DEMO_GAME_SCREEN_H__