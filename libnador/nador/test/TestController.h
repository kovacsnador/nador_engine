#ifndef __TEST_CONTROLLER_H__
#define __TEST_CONTROLLER_H__

#include <vector>
#include <string>
#include <functional>

#include "nador/test/ITestFwd.h"
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
        TestController(const IVideo*           video,
                       const IFileController*  fileCtrl,
                       const IAtlasController* atlasCtrl,
                       const IFontController*  fontCtrl,
                       IUiApp*                 uiApp,
                       const IInputController* inputCtrl);

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

    private:
        onTick_listener_t        _onTickListener;
        onRender_listener_t      _onRenderListener;
        onDebugRender_listener_t _onDebugRenderListener;

        const IVideo*          _video;
        const IFileController* _fileCtrl;
    };
    CREATE_PTR_TYPES(TestController);

} // namespace nador

#endif // !__TEST_CONTROLLER_H__
