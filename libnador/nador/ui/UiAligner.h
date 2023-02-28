#ifndef __NADOR_UI_ALIGNER_H__
#define __NADOR_UI_ALIGNER_H__

#include <vector>

#include "nador/ui/IUiElement.h"

namespace nador
{
	enum class EAligner
	{
		VERTICAL,
		HORIZONTAL,
	};

	class UiAligner
	{
	public:
		UiAligner() = default;
		UiAligner(EAligner aligner, int32_t offset);

		void SetAligner(EAligner aligner);

		void SetOffset(const glm::ivec2& offset);
		void SetMinOffset(const glm::ivec2& minOffset);
		void SetMaxOffset(const glm::ivec2& maxOffset);
		
		void SetDiff(int32_t diff);
		void SetAlignment(UiAlignment alignment);

		const glm::ivec2& GetOffset() const noexcept;
		const glm::ivec2& GetMinOffset() const noexcept;
		const glm::ivec2& GetMaxOffset() const noexcept;

		void AddElement(IUiElement* element);
		void RemoveElement(IUiElement* element);

		glm::ivec2 CalculateMaxOffset() const;
		glm::vec2 CalculateOffsetPercentage() const;

		void Update();

	private:
		EAligner	_alignerType { EAligner::VERTICAL };
		glm::ivec2	_offset{ 0, 0 };
		glm::ivec2	_minOffset{ -9999, -9999 };
		glm::ivec2	_maxOffset{ 9999, 9999 };
		int32_t		_diff{ 0 };

		UiAlignment _alignment;

		std::vector<IUiElement*> _elements;
	};
}

#endif //!__NADOR_UI_ALIGNER_H__

