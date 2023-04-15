#ifndef __TEST_CONTROLLER_H__
#define __TEST_CONTROLLER_H__

#include <vector>
#include <string>
#include <functional>

#include "nador/common/GlobalEvents.h"
#include "nador/test/ITestController.h"

namespace nador
{
    class IAtlasController;
    class IFontController;
    class IUiApp;
    class IInputController;

    class TestController : public ITestController
    {
    public:
        /*!
         * TestController constructor.
         */
        TestController();

        /*!
         * Get the tests.
         *
         * \return The test conatnier
         */
        const test_list_t& GetTests() const override { return _tests; }

        /*!
         * On tick function.
         *
         * \param  The delta time between ticks.
         */
        void OnTick(float_t deltaTime) override;

        /*!
         * Called on render.
         */
        void OnRender(IRenderer* renderer) override;

        /*!
         * Render the imgui content.
         */
        void OnDebugRender(IRenderer* renderer) override;

        void Suspend(bool suspend) override;

        void SetToggleDebugTextCallback(ToggleDebugTextCb_t cb) noexcept override;

    private:
        void _DebugRenderStartMenu();

        onTick_listener_t        _onTickListener;
        onRender_listener_t      _onRenderListener;
        onDebugRender_listener_t _onDebugRenderListener;

        ToggleDebugTextCb_t _toggleDebugTxtCb { nullptr };
    };
    CREATE_PTR_TYPES(TestController);

} // namespace nador

#endif // !__TEST_CONTROLLER_H__
