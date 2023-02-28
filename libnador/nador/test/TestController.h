#ifndef __TEST_CONTROLLER_H__
#define __TEST_CONTROLLER_H__

#include <vector>
#include <string>
#include <functional>

#include "nador/test/ITestFwd.h"
#include "nador/common/GlobalEvents.h"

namespace nador
{
	class TestController
	{
		using pair_t = std::pair<std::string, std::function<void ()>>;
		using test_list_t = std::vector<pair_t>;

	public:
		/*!
		 * TestController constructor.
		 */
		TestController();

		/*!
		 * Add new test.
		 *
		 * \param name The name of the test. (will be the button label)
		 */
		template<typename T>
		void AddTest(const std::string& name)
		{
			_tests.push_back(std::make_pair(name, 
											[this]() 
											{ 
												_currentTest.reset(new T);
											}));
		}

		/*!
		 * Get the tests.
		 *
		 * \return The test conatnier
		 */
		const test_list_t& GetTests() const
		{
			return _tests;
		}

		/*!
		 * On tick function.
		 *
		 * \param  The delta time between ticks.
		 */
		void OnTick(float_t deltaTime);

		/*!
		 * Called on render.
		 */
		void OnRender();

		/*!
		 * Render the imgui content.
		 */
		void OnDebugRender();

		void Suspend(bool suspend);

	private:
		ITestPtr	_currentTest;
		test_list_t _tests;

		onTick_listener_t			_onTickListener;
		onRender_listener_t			_onRenderListener;
		onDebugRender_listener_t	_onDebugRenderListener;
	};

	CREATE_PTR_TYPES(TestController);
}

#endif // !__TEST_CONTROLLER_H__

