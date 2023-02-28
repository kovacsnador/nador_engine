#include "nador/test/TestCommon.h"

namespace nador
{
	UiAlignment ToAligner(uint32_t horizontal, uint32_t vertical)
	{
		EHorizontalAlignment eHorizontal = EHorizontalAlignment::LEFT;
		EVerticalAlignment eVertical = EVerticalAlignment::BOTTOM;

		switch (horizontal)
		{
		case 0:
			eHorizontal = EHorizontalAlignment::LEFT;
			break;
		case 1:
			eHorizontal = EHorizontalAlignment::RIGHT;
			break;
		case 2:
			eHorizontal = EHorizontalAlignment::CENTER;
			break;
		case 3:
			eHorizontal = EHorizontalAlignment::STRETCH;
			break;
		}

		switch (vertical)
		{
		case 0:
			eVertical = EVerticalAlignment::TOP;
			break;
		case 1:
			eVertical = EVerticalAlignment::BOTTOM;
			break;
		case 2:
			eVertical = EVerticalAlignment::CENTER;
			break;
		case 3:
			eVertical = EVerticalAlignment::STRETCH;
			break;
		}

		return { eHorizontal, eVertical };
	}
}