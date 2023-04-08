#ifndef __FONT_TEST_H__
#define __FONT_TEST_H__

#include <map>
#include <string>

#include "nador/test/ITest.h"
#include "nador/common/GlobalEvents.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"
#include "nador/video/font/FontController.h"

namespace nador
{
	class IFontController;

	class FontTest : public ITest
	{
	public:
		/*!
		 * FontTest constructor.
		 */
		FontTest(const IFontController* fontCtrl);

		/*!
		 * Called on render.
		 */
		void OnRender(IRenderer* renderer) override;

		/*!
		 * Called on imgui render.
		 */
		void OnDebugRender(IRenderer* /*renderer*/) override;

	private:
		/*!
		 * Loads the font.
		 *
		 * \param fontId	The id of the font.
		 * \param fontSize  The size of the font.
		 */
		void _LoadFont(uint32_t fontId, uint32_t fontSize);

		const IFontController* _fontCtrl;

		FontMaterial	_fontMaterial;
		RenderData		_renderData;
		glm::vec2		_position;
		glm::vec3		_scale;
		glm::vec4		_color;

		std::unique_ptr<char[]> _inputTextBuffer;
	};
}

#endif // !__FONT_TEST_H__