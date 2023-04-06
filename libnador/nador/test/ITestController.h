#ifndef __I_TEST_CONTROLLER_H__
#define __I_TEST_CONTROLLER_H__

#include "nador/utils/Types.h"
#include "nador/test/ITestFwd.h"

namespace nador
{
    class IRenderer;

    class ITestController
    {
    public:
        using pair_t      = std::pair<std::string, std::function<void()>>;
        using test_list_t = std::vector<pair_t>;

        using ToggleDebugTextCb_t = std::function<void()>;

        virtual ~ITestController() = default;

        template <typename T, typename... Args>
        void AddTest(const std::string& name, Args... args)
        {
            _tests.push_back(std::make_pair(name, [this, args...]() { _currentTest.reset(new T(args...)); }));
        }

        /*!
         * Get the tests.
         *
         * \return The test conatnier
         */
        virtual const test_list_t& GetTests() const = 0;

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

    protected:
        test_list_t         _tests {};
        ITestPtr            _currentTest { nullptr };
    };
    CREATE_PTR_TYPES(ITestController);
} // namespace nador

#endif //!__I_TEST_CONTROLLER_H__