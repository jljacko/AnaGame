#include "ColorAnimation.h"
#include <TString.h>
#include <atltrace.h>

TString colorAnimationString(L"Simple-Color");

ColorAnimation::ColorAnimation(animation_phase ap): Animation(ap)
{
	color1 = color2 = D2D1::ColorF(D2D1::ColorF::Azure);
}

bool ColorAnimation::Update(float progress)
{
	if (!brush.Get())
		return false;
	if(millisecondLength && millisecondStoryLength)
		progress = progress * (static_cast<float>(this->millisecondLength) / static_cast<float>(this->millisecondStoryLength));

	D2D1_COLOR_F curColor{
		GetFloatBetween(color1.r, color2.r, progress),
		GetFloatBetween(color1.g, color2.g, progress),
		GetFloatBetween(color1.b, color2.b, progress),
		GetFloatBetween(color1.a, color2.a, progress)
	};

	try
	{
		reinterpret_cast<ID2D1SolidColorBrush*>(brush.Get())->SetColor(curColor);
	}
	catch (...)
	{
		ATLTRACE(L"Exception Caught Attempting to cast Brush. Was it not a SolidColor Brush?");
	}
	return true;
}

void ColorAnimation::SetAnimationValue(float value, animation_value_type type)
{
	if (value > 1.0f)
	{
		UINT iValue = static_cast<UINT>(value);

		value /= static_cast<float>(iValue + 1);
	}
	switch (type)
	{
	case animation_value_type_alpha:
		color1.a = value;
		break;
	case animation_value_type_blue:
		color1.b = value;
		break;
	case animation_value_type_green:
		color1.g = value;
		break;
	case animation_value_type_red:
		color1.r = value;
		break;
	case animation_value_type_alpha2:
		color2.a = value;
		break;
	case animation_value_type_blue2:
		color2.b = value;
		break;
	case animation_value_type_green2:
		color2.g = value;
		break;
	case animation_value_type_red2:
		color2.r = value;
		break;
	}
}

void ColorAnimation::SetComponent(TrecComPointer<ID2D1Brush> comp)
{
	brush = comp;
}

void ColorAnimation::Prepare()
{
	if (done && reverse)
	{
		D2D1_COLOR_F tempColor = color1;
		color1 = color2;
		color2 = tempColor;
	}
	done = false;
}

TString ColorAnimation::GetAnimationType()
{
	return colorAnimationString;
}


