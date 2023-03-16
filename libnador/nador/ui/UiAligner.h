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

		void SetOffset(const glm::vec2& offset);
		void SetMinOffset(const glm::vec2& minOffset);
		void SetMaxOffset(const glm::vec2& maxOffset);
		
		void SetDiff(int32_t diff);
		void SetAlignment(UiAlignment alignment);

		const glm::vec2& GetOffset() const noexcept;
		const glm::vec2& GetMinOffset() const noexcept;
		const glm::vec2& GetMaxOffset() const noexcept;

		void AddElement(IUiElement* element);
		void RemoveElement(IUiElement* element);

		glm::vec2 CalculateMaxOffset() const;
		glm::vec2 CalculateOffsetPercentage() const;

		void Update();

	private:
		EAligner	_alignerType { EAligner::VERTICAL };
		glm::vec2	_offset{ 0, 0 };
		glm::vec2	_minOffset{ -9999, -9999 };
		glm::vec2	_maxOffset{ 9999, 9999 };
		int32_t		_diff{ 0 };

		UiAlignment _alignment;

		std::vector<IUiElement*> _elements;
	};
}

#endif //!__NADOR_UI_ALIGNER_H__

