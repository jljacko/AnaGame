#include "ColorAnimation.h"
#include <TString.h>
#include <atltrace.h>
#include <TBrush.h>

TString colorAnimationString(L"Simple-Color");

ColorAnimation::ColorAnimation(animation_phase ap): Animation(ap)
{
	color1 = color2 = D2D1::ColorF(D2D1::ColorF::Azure);
	color1Set = color2Set = false;
}

bool ColorAnimation::Update(float progress)
{
	ATLTRACE(L"COLOR ANIMATION UPDATE called!\n");
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
	TString print;
	print.Format(L"COLOR ANIMATION R: %f G: %f, B: %f, A: %f\n", curColor.r, curColor.g, curColor.b, curColor.a);
	ATLTRACE(print.GetConstantBuffer());

	try
	{
		brush.Get()->SetColor(curColor);
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
	case animation_value_type::animation_value_type_alpha:
	case animation_value_type::animation_value_type_blue:
	case animation_value_type::animation_value_type_red:
	case animation_value_type::animation_value_type_green:
		color1Set = true;
		break;
	case animation_value_type::animation_value_type_alpha2:
	case animation_value_type::animation_value_type_blue2:
	case animation_value_type::animation_value_type_red2:
	case animation_value_type::animation_value_type_green2:
		color2Set = true;
		break;
	}


	switch (type)
	{
	case animation_value_type::animation_value_type_alpha:
		color1.a = value;
		break;
	case animation_value_type::animation_value_type_blue:
		color1.b = value;
		break;
	case animation_value_type::animation_value_type_green:
		color1.g = value;
		break;
	case animation_value_type::animation_value_type_red:
		color1.r = value;
		break;
	case animation_value_type::animation_value_type_alpha2:
		color2.a = value;
		break;
	case animation_value_type::animation_value_type_blue2:
		color2.b = value;
		break;
	case animation_value_type::animation_value_type_green2:
		color2.g = value;
		break;
	case animation_value_type::animation_value_type_red2:
		color2.r = value;
		break;
	}
}

void ColorAnimation::SetComponent(TrecPointer<TBrush> comp)
{
	brush = comp;
	if (!brush.Get())
		return;
	if (color1Set && !color2Set)
		color2 = brush->GetColor().GetColor();
	else if (!color1Set && color2Set)
		color1 = brush->GetColor().GetColor();
}

void ColorAnimation::SetControl(TrecPointer<TControl> con)
{
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


