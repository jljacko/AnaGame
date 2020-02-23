#include "RotateAnimation.h"
#include <TString.h>
#include "TControl.h"

TString rotationAnimationString(L"Simple-Transition");

RotateAnimation::RotateAnimation(animation_phase ap): Animation(ap)
{
	rotateStart = 0.0f;
	rotateEnd = 360.0f;
	clockwise = true;
	degrees = true;
}

bool RotateAnimation::Update(float progress)
{
	if(!control.Get())
		return false;

	if (millisecondLength && millisecondStoryLength)
		progress = progress * (static_cast<float>(this->millisecondLength) / static_cast<float>(this->millisecondStoryLength));

	if (degrees)
		control->RotateDegrees(GetFloatBetween(rotateStart, rotateEnd, progress));
	else
		control->RotateRadians(GetFloatBetween(rotateStart, rotateEnd, progress));
	return true;
}

void RotateAnimation::SetAnimationValue(float value, animation_value_type type)
{
	switch (type)
	{
	case animation_value_type_radian:
		degrees = false;
		break;
	case animation_value_type_rotate_start:
		rotateStart = value;
		break;
	case animation_value_type_rotate_end:
		rotateEnd = value;
		break;
	case animation_value_type_direction:
		if (value == 0.0f)
		{
			if (rotateEnd > rotateStart)
			{
				float temp = rotateEnd;
				rotateEnd = rotateStart;
				rotateStart = temp;
			}
			clockwise = false;
		}
	}
}

void RotateAnimation::SetComponent(TrecComPointer<ID2D1Brush> comp)
{
}

void RotateAnimation::Prepare()
{
}

TString RotateAnimation::GetAnimationType()
{
	return rotationAnimationString;
}

void RotateAnimation::SetControl(TrecPointer<TControl> con)
{
	control = con;
}
