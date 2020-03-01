#include "TransitionAnimation.h"
#include "TControl.h"

TString transitionAnimationString(L"Simple-Transition");

TransitionAnimation::TransitionAnimation(animation_phase ap): Animation(ap)
{
	ZeroMemory(&loc1, sizeof(loc1));
	ZeroMemory(&loc2, sizeof(loc2));
}

bool TransitionAnimation::Update(float progress)
{
	if (!control.Get())
		return false;
	if (millisecondLength && millisecondStoryLength)
		progress = progress * (static_cast<float>(this->millisecondLength) / static_cast<float>(this->millisecondStoryLength));

	D2D1_RECT_F curLoc{
		GetFloatBetween(loc1.left, loc2.left, progress),
		GetFloatBetween(loc1.top, loc2.top, progress),
		GetFloatBetween(loc1.right, loc2.right, progress),
		GetFloatBetween(loc1.bottom, loc2.bottom, progress)
	};

	control->Resize(curLoc);
	return true;
}

void TransitionAnimation::SetAnimationValue(float value, animation_value_type type)
{
	if (animation_phase_end == phase)
	{
		switch (type)
		{
		case animation_value_type_bottom_location:
			loc2.bottom = value;
			break;
		case animation_value_type_left_location:
			loc2.left = value;
			break;
		case animation_value_type_right_location:
			loc2.right = value;
			break;
		case animation_value_type_top_location:
			loc2.top = value;
			break;
		}
	}
	else
	{
		switch (type)
		{
		case animation_value_type_bottom_location:
			loc1.bottom = value;
			break;
		case animation_value_type_left_location:
			loc1.left = value;
			break;
		case animation_value_type_right_location:
			loc1.right = value;
			break;
		case animation_value_type_top_location:
			loc1.top = value;
			break;
		}
	}
}

void TransitionAnimation::SetComponent(TrecPointer<TBrush> comp)
{
}

void TransitionAnimation::Prepare()
{
	if (done && reverse)
	{
		D2D1_RECT_F temploc = loc1;
		loc1 = loc2;
		loc2 = temploc;
	}
	done = false;
}

TString TransitionAnimation::GetAnimationType()
{
	return transitionAnimationString;
}

void TransitionAnimation::SetControl(TrecPointer<TControl> con)
{
	control = con;
	if (!control.Get())
		return;
	if (phase == animation_phase_end)
	{
		loc1 = control->getLocation();
	}
	else
		loc2 = control->getLocation();
}
