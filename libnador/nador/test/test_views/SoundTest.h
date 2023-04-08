#ifndef __SOUND_TEST_H__
#define __SOUND_TEST_H__

#include "nador/test/ITest.h"
#include "nador/sound/ISoundController.h"

namespace nador
{
	class SoundTest : public ITest
	{
	public:
		using vec_t = std::vector<std::pair<SoundData, ISoundSourcePtr>>;

		SoundTest(ISoundController* soundCtrl);

		/*!
		 * Render the imgui content.
		 */
		void OnDebugRender(IRenderer* /*renderer*/) override;

	private:
		void _LoadSounds();

		ISoundController* _soundCtrl;
		vec_t _sounds;
	};
}

#endif // !__SOUND_TEST_H__