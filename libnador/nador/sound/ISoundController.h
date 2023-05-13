#ifndef __ISOUND_CONTROLLER_H__
#define __ISOUND_CONTROLLER_H__

#include <functional>

#include "nador/utils/Types.h"

namespace nador
{
	struct SoundData
	{
		SoundData(std::string fileName, uint32_t soundId)
		: fileName(std::move(fileName))
		, soundId(soundId)
		{
		}

		std::string fileName;
		uint32_t soundId;
	};

	enum class ESoundSourceState
	{
		NONE,
		INITIAL,
		PLAYING,
		PAUSED,
		STOPPED,
	};

	struct ISoundSource
	{
		virtual ~ISoundSource() = default;

		virtual void Play() const = 0;
		virtual void Pause() const = 0;
		virtual void Rewind() const = 0;
		virtual void Stop() const = 0;

		virtual ESoundSourceState GetState() const = 0;

		virtual void SetLooping(bool looping) = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;

		virtual glm::vec3 GetPosition() const = 0;
		virtual bool IsLooping() const = 0;

		virtual uint32_t GetSoundId() const = 0;
		virtual uint32_t GetSourceId() const = 0;
	};
	CREATE_PTR_TYPES(ISoundSource);


	using sound_data_list_t = std::vector<SoundData>;

	class ISoundController
	{
	public:
		/*!
		 * ISoundController destructor.
		 */
		virtual ~ISoundController() = default;

		virtual void TickBegin() = 0;

		/*!
		 * Add sound to the sound contoller with the given sound id.
		 *
		 * \param fileName    The sound file name.
		 * \param soundId     The sound id.
		 */
		virtual bool AddSound(const char* fileName, uint32_t soundId) = 0;

		/*!
		 * Creates a sound source from the sound id.
		 *
		 * \param soundId     The sound id.
		 * 
		 * \return		The new Sound source id.
		 */
		virtual ISoundSourceUPtr CreateSoundSource(uint32_t soundId) = 0;

		/*!
		 * Plays the sound by the id.
		 *
		 * \param soundId   The sound id.
		 */
		virtual void PlaySound(uint32_t soundId) = 0;

		/*!
		 * Stops all sounds.
		 */
		virtual void StopAllSound() = 0;

		/*!
		 * Gets all registered sound data.
		 * 
		 * \return	All sound data.
		 */
		virtual sound_data_list_t GetAllSoundData() const = 0;
	};
	CREATE_PTR_TYPES(ISoundController);
}

#endif // !__ISOUND_CONTROLLER_H__
