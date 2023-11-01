#ifndef __UI_APP_H__
#define __UI_APP_H__

#include "nador/ui/IUiApp.h"
#include "nador/common/GlobalListener.h"
#include "nador/system/IInputController.h"
#include "nador/video/atlas/IAtlasController.h"
#include "nador/utils/NonCopyable.h"

namespace nador
{
    class UiApp : public IUiApp, protected GlobalListener, private NonCopyable
    {
    public:
        /*!
         * UiApp default constructor.
         */
        UiApp(const IVideoPtr video, const IInputControllerPtr inputCtrl, const IAtlasControllerPtr atlasCtrl);

        /*!
         * UiApp destructor.
         */
        ~UiApp();

        /*!
         * Gets the screen size.
         *
         * \return	The screen size.
         */
        const glm::ivec2& GetScreenSize() const override;

        /*!
         * Gets the screen scale.
         *
         * \return	The screen scale.
         */
        float_t GetScreenScale() const override;

        /*!
         * Gets the screen ratio.
         *
         * \return	The screen ratio.
         */
        float_t GetAspectRatio() const override;

        quadVertices_t GetScreenVertices() const override;

        void AddElementToLayer(EUiLayer layer, IUiElement* elem) override;
        void RemoveElementFromLayer(EUiLayer layer, IUiElement* elem) override;
        void RemoveElement(IUiElement* elem) override;
        void ClearLayer(EUiLayer layer) override;

        void DebugDrawEdge(bool draw) override;
        bool IsDebugDrawEdge() const noexcept override;

        void BringToFront(IUiElement* elem) override;
        void PushToback(IUiElement* elem) override;

        bool IsInFocus(const IUiElement* elem) const noexcept override;
        void SetInFocus(const IUiElement* elem) const noexcept override;

    protected:
        void OnTick(float_t deltaTime) override;
        void OnUiRender(IRenderer* renderer) override;

        void OnKeyPressed(EKeyCode keycode) override;
        void OnKeyHolded(EKeyCode keycode) override;
        void OnKeyReleased(EKeyCode keycode) override;

        void OnCharEvent(const std::string& text) override;

        void OnMousePressed(EMouseButton eMouseButton, const glm::vec2& position) override;
        void OnMouseReleased(EMouseButton eMouseButton, const glm::vec2& position) override;

    private:
        template <typename CallbackTy>
        void _PushElementInLayers(IUiElement* elem, CallbackTy callback)
        {
            for (auto& layer : _layers)
            {
                auto& list = layer.second;

                if (std::find(list.begin(), list.end(), elem) != list.end())
                {
                    utils::Remove(list, elem);
                    callback(list, elem);
                }
            }
        }

        template <typename... Params, typename... Args>
        void _OnKeyAndMouseEventImpl(bool (IUiElement::*onEventImpl)(Params...), Args... args)
        {
            for (auto layer = _layers.rbegin(); layer != _layers.rend(); layer++)
            {
                for (auto elem = layer->second.rbegin(); elem != layer->second.rend(); elem++)
                {
                    if (((*elem)->*onEventImpl)(args...))
                    {
                        // break if already handled
                        return;
                    }
                }
            }
        }

        bool _debugDrawEdge { false };

        const IVideoPtr           _video { nullptr };
        const IInputControllerPtr _inputCtrl { nullptr };
        const IAtlasControllerPtr _atlasCtrl { nullptr };

        ui_layer_list_t          _layers;
        static const IUiElement* s_pFocusedElement;
    };

    CREATE_PTR_TYPES(UiApp);
} // namespace nador

#endif // !__UI_APP_H__
