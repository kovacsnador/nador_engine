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

		SoundTest();

		/*!
		 * Render the imgui content.
		 */
		void OnDebugRender() override;

	private:
		void _LoadSounds();

		vec_t _sounds;
	};
}

#endif // !__SOUND_TEST_H__