#include "nador/ui/UiAligner.h"

namespace nador
{
	UiAligner::UiAligner(EAligner aligner, int32_t offset)
	: _alignerType(aligner)
	, _offset(offset)
	{
	}

	void UiAligner::SetAligner(EAligner aligner)
	{
		_alignerType = aligner;
	}

	void UiAligner::SetOffset(const glm::ivec2& offset)
	{
		const glm::ivec2* pOffset = &offset;

		if (_alignerType == EAligner::VERTICAL)
		{
			if (offset.y < _minOffset.y)
			{
				pOffset = &_minOffset;
			}
			else if (offset.y > _maxOffset.y)
			{
				pOffset = &_maxOffset;
			}
		}
		else if(_alignerType == EAligner::HORIZONTAL)
		{
			if (offset.x < _minOffset.x)
			{
				pOffset = &_minOffset;
			}
			else if (offset.x > _maxOffset.x)
			{
				pOffset = &_maxOffset;
			}
		}

		_offset = *pOffset;
		Update();
	}

	void UiAligner::SetMinOffset(const glm::ivec2& minOffset)
	{
		_minOffset = minOffset;
		SetOffset(_offset);
	}

	void UiAligner::SetMaxOffset(const glm::ivec2& maxOffset)
	{
		_maxOffset = maxOffset;
		SetOffset(_offset);
	}

	const glm::ivec2& UiAligner::GetMinOffset() const noexcept
	{
		return _minOffset;
	}

	const glm::ivec2& UiAligner::GetMaxOffset() const noexcept
	{
		return _maxOffset;
	}

	const glm::ivec2& UiAligner::GetOffset() const noexcept
	{
		return _offset;
	}

	void UiAligner::SetDiff(int32_t diff)
	{
		_diff = diff;
		Update();
	}

	void UiAligner::SetAlignment(UiAlignment alignment)
	{
		_alignment = alignment;
		Update();
	}

	void UiAligner::AddElement(IUiElement* element)
	{
		if (std::find(_elements.begin(), _elements.end(), element) == _elements.end())
		{
			_elements.push_back(element);
		}
		Update();
	}

	void UiAligner::RemoveElement(IUiElement* element)
	{
		auto it = std::find(_elements.begin(), _elements.end(), element);
		if (it != _elements.end())
		{
			_elements.erase(it);
		}
		Update();
	}

	void UiAligner::Update()
	{
		glm::ivec2 offset = _offset;
		for (auto& it : _elements)
		{
			it->SetAlignment(_alignment);
			it->SetOffset(offset);

			if (_alignerType == EAligner::VERTICAL)
			{
				offset += glm::ivec2(0, it->GetSize().y + _diff);
			}
			else if (_alignerType == EAligner::HORIZONTAL)
			{
				offset += glm::ivec2(it->GetSize().x + _diff, 0);
			}
		}
	}

	glm::ivec2 UiAligner::CalculateMaxOffset() const
	{
		glm::ivec2 offset{ 0, 0 };
		for (const auto& it : _elements)
		{
			if (it == _elements.back())
			{
				break;
			}

			if (_alignerType == EAligner::VERTICAL)
			{
				offset += glm::ivec2(0, it->GetSize().y + _diff);
			}
			else if (_alignerType == EAligner::HORIZONTAL)
			{
				offset += glm::ivec2(it->GetSize().x + _diff, 0);
			}
		}
		return offset;
	}

	glm::vec2 UiAligner::CalculateOffsetPercentage() const
	{
		glm::vec2 offset = glm::abs(_offset);
		glm::vec2 maxOffset = CalculateMaxOffset();
		return offset / maxOffset;
	}
}