#ifndef __I_TEST_CONTROLLER_H__
#define __I_TEST_CONTROLLER_H__

#include <variant>

#include "nador/utils/Types.h"
#include "nador/test/ITest.h"
#include "nador/test/CameraWindow.h"

namespace nador
{
    class IRenderer;

    class ITestController
    {
    public:
        using ToggleGameCallback_t = std::function<void(bool)>;
        using pair_t               = std::pair<std::string, std::function<void()>>;
        using ButtonsList_t        = std::vector<pair_t>;

        using AdditionalWindows_t = std::variant<CameraWindow>;
        using WindowsList_t       = std::vector<AdditionalWindows_t>;

        using ToggleDebugTextCb_t = std::function<void()>;

        virtual ~ITestController() = default;

        template <typename T, typename... Args>
        void AddTest(const std::string& name, Args&&... args)
        {
            _tests.push_back(std::make_pair(name, [this, args...]() { _currentTest = std::make_unique<T>(args...); }));
        }

        void AddCustomButton(const std::string& name, std::function<void()> callback)
        {
            _customButtons.emplace_back(std::make_pair(name, callback));
        }

        /*!
         * On tick function.
         *
         * \param  The delta time between ticks.
         */
        virtual void OnTick(float_t deltaTime) = 0;

        /*!
         * Called on render.
         */
        virtual void OnRender(IRenderer* renderer) = 0;

        /*!
         * Render the imgui content.
         */
        virtual void OnDebugRender(IRenderer* renderer) = 0;

        virtual void Suspend(bool suspend) = 0;

        virtual void SetToggleDebugTextCallback(ToggleDebugTextCb_t cb) noexcept = 0;

        virtual void AddWindow(AdditionalWindows_t&& window) = 0;

    protected:
        ButtonsList_t _tests {};
        ITestUPtr     _currentTest { nullptr };

        ButtonsList_t _customButtons {};
    };
    CREATE_PTR_TYPES(ITestController);
} // namespace nador

#endif //!__I_TEST_CONTROLLER_H__