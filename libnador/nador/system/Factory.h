#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <memory>

#include "nador/system/IFactory.h"
#include "nador/common/AppConfig.h"

namespace nador
{
	class Factory : public IFactory
	{
	public:
		/*!
		 * Factroy constructor.
		 *
		 * \param config		The app config struct.
		 */
		Factory(const AppConfig& config);
		
		/*!
		 * Factroy destructor.
		 */
		virtual ~Factory();

		/*!
		 * Gets the operation system specific window.
		 * 
		 * \return The current window interface 
		 */
		IWindow* GetWindow() const override;

		/*!
		 * Gets the operation system specific window.
		 *
		 * \return The current window interface
		 */
		const IWindow* GetWindowConst() const override;

		/*!
		 * Gets the video interface.
		 *
		 * \return The current video interface
		 */
		const IVideo* GetVideo() const override;

		/*!
		 * Gets the file controller.
		 *
		 * \return	The file controller interface.
		 */
		IFileController* GetFileController() const override;

		/*!
		 * Gets the input controller.
		 *
		 * \return	The input controller interface.
		 */
		IInputController* GetInputController() const override;

		/*!
		 * Gets the sound controller.
		 *
		 * \return	The sound controller interface.
		 */
		ISoundController* GetSoundController() const override;

	private:
		/*!
		 * Initialize the window to draw.
		 */
		void _InitWindow(const AppConfig& config);
		
		/*!
		 * Initialize the video interface.
		 */
		void _InitVideo();

		/*!
		 * Initialize the file controller interface.
		 */
		void _InitFileController(const std::string_view& rootFilePath);

		/*!
		 * Initialize the sound controller interface.
		 */
		void _InitSoundController();

		/*!
		 * Initialize the input controller interface.
		 */
		void _InitInputController();

		IVideo*				_video;
		IWindow*			_window;
		IFileController*	_fileController;
		IInputController*	_inputController;
		ISoundController*	_soundController;
	};

	CREATE_PTR_TYPES(Factory);
}

#endif // !__FACTORY_H__

