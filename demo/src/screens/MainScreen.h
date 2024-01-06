#ifndef __NADOR_DEMO_MAIN_SCREEN_H__
#define __NADOR_DEMO_MAIN_SCREEN_H__

#include "nador/ui/IUiApp.h"
#include "nador/ui/UiImage.h"
#include "nador/ui/UiTextLabel.h"
#include "nador/ui/UiScene.h"
#include "nador/ui/UiAligner.h"
#include "nador/video/font/IFontController.h"
#include "nador/common/GlobalEvents.h"
#include "nador/sound/ISoundController.h"

namespace demo
{
    struct MenuButton : public nador::UiScene
    {
        MenuButton(const glm::ivec2&        position,
                   const glm::ivec2&        size,
                   nador::UiAlignment       alignment,
                   const nador::FontPtr&    font,
                   std::string_view         text,
                   nador::video::EImageName imageName)
        : nador::UiScene(position, size, alignment)
        , aligner(nador::EAligner::HORIZONTAL, 0)
        , textLabel({ 0, 0 }, font, text, { nador::EHorizontalAlignment::CENTER, nador::EVerticalAlignment::CENTER })
        , image({ 0, 0 }, { 50, 50 }, imageName, { nador::EHorizontalAlignment::RIGHT, nador::EVerticalAlignment::CENTER })
        {
            AddChild(&textLabel);
            AddChild(&image);

            aligner.SetAlignment({ nador::EHorizontalAlignment::LEFT, nador::EVerticalAlignment::CENTER });
            aligner.SetDiff(20);

            aligner.AddElement(&image);
            aligner.AddElement(&textLabel);

            Select(false);
        }

        void Select(bool select) { image.Show(select); }

        nador::UiAligner   aligner;
        nador::UiTextLabel textLabel;
        nador::UiImage     image;
    };

    class MainScreen
    {
    public:
        MainScreen(nador::IUiApp* uiApp, std::shared_ptr<nador::Font> font, std::shared_ptr<nador::ISoundSource> mainMusic);
        ~MainScreen();

        MainScreen(const MainScreen& other)            = default;
        MainScreen& operator=(const MainScreen& other) = default;

        MainScreen(MainScreen&& other)            = default;
        MainScreen& operator=(MainScreen&& other) = default;

        void Start();
        void Stop();

        template<typename ListenerT>
        void RegisterListener(ListenerT& listener)
        {
            _buttonEvent->operator+=(listener);
        }

    private:
        glm::ivec2 _CalculateMarioPos(const std::shared_ptr<nador::UiTextLabel>& label) const noexcept;

        void _OnKeyPressed(nador::EKeyCode keycode);

        nador::IUiApp* _uiApp;

        std::shared_ptr<nador::UiScene> _scene;

        std::shared_ptr<nador::UiImage> _background;

        std::shared_ptr<nador::UiTextLabel> _superMarioLabel;

        std::array<std::shared_ptr<MenuButton>, 3> _menuButtons;
        size_t                                     selected { 0 };

        std::shared_ptr<MenuButton> _menuButtonStart;
        std::shared_ptr<MenuButton> _menuButtonOptions;
        std::shared_ptr<MenuButton> _menuButtonExit;

        nador::onKey_listener_t _keyReleasedListener;

        std::shared_ptr<nador::ISoundSource> _music;
        std::shared_ptr<nador::Event<size_t>> _buttonEvent;
    };

} // namespace demo

#endif //!__NADOR_DEMO_MAIN_SCREEN_H__