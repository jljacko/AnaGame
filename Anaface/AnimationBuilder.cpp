#include "AnimationBuilder.h"
#include "ColorAnimation.h"
#include "RotateAnimation.h"
#include "TransitionAnimation.h"
#include "TControl.h"

AnimationBuilder::AnimationBuilder()
{
	phase = animation_phase_middle;
}

void AnimationBuilder::SetName(const TString& name)
{
	this->name.Set(name);
}

void AnimationBuilder::SetType(const TString& type)
{
	this->type.Set(type);
}

void AnimationBuilder::SetAttribute(const TString& key, TrecPointer<TString> value)
{
	if (!key.Compare(L"Phase"))
	{
		if (value.Get())
		{
			if (!value->Compare(L"Begin"))
				phase = animation_pase_beginning;
			else if (!value->Compare(L"End"))
				phase = animation_phase_end;
			else if (!value->Compare(L"Persistent"))
				phase = animation_phase_persistant;
		}
		return;
	}
	if (value.Get())
		attributes.addEntry(key, value);
}

TString AnimationBuilder::GetName()
{
	return name;
}

TrecPointer<Animation> AnimationBuilder::Build()
{
	TrecPointer<Animation> ret;

	if (!type.CompareNoCase(TString(L"Simple-Color")))
		ret = TrecPointerKey::GetNewTrecPointerAlt<Animation, ColorAnimation>(phase);
	else if (!type.CompareNoCase(TString(L"Simple-Transition")))
		ret = TrecPointerKey::GetNewTrecPointerAlt<Animation, TransitionAnimation>(phase);
	else if (!type.CompareNoCase(TString(L"Simple-Rotation")))
		ret = TrecPointerKey::GetNewTrecPointerAlt<Animation, RotateAnimation>(phase);


	if (!ret.Get())
		return ret;

	UINT index = 0;

	TrecPointer<tEntry<TString>> entry = attributes.GetEntryAt(index++);

	while (entry.Get() && entry->object.Get())
	{
		D2D1_COLOR_F color;
		ColorFormat cFormat;
		float value = 0.0f;
		if (entry->key.Compare(L"BeginColor") && entry->object->ConvertToColor(color, cFormat))
		{
			ret->SetAnimationValue(color.a, animation_value_type_alpha);
			ret->SetAnimationValue(color.b, animation_value_type_blue);
			ret->SetAnimationValue(color.g, animation_value_type_green);
			ret->SetAnimationValue(color.r, animation_value_type_red);
		}
		else if (entry->key.Compare(L"EndColor") && entry->object->ConvertToColor(color, cFormat))
		{
			ret->SetAnimationValue(color.a, animation_value_type_alpha2);
			ret->SetAnimationValue(color.b, animation_value_type_blue2);
			ret->SetAnimationValue(color.g, animation_value_type_green2);
			ret->SetAnimationValue(color.r, animation_value_type_red2);
		}
		else if (entry->key.Compare(L"IsRadian"))
		{
			// Value could be anything, all this does is set animation to radian mode (default is degrees)
			ret->SetAnimationValue(0.0f, animation_value_type_radian);
		}
		else if (entry->key.Compare(L"BeginRotate") && !entry->object->ConvertToFloat(&value))
		{
			ret->SetAnimationValue(value, animation_value_type_rotate_start);
		}
		else if (entry->key.Compare(L"EndRotate") && !entry->object->ConvertToFloat(&value))
		{
			ret->SetAnimationValue(value, animation_value_type_rotate_end);
		}
		else if (entry->key.Compare(L"RotateDirection"))
		{
			entry->object->ConvertToFloat(&value);
			ret->SetAnimationValue(value, animation_value_type_direction);
		}
		else if (entry->key.Compare(L"Location"))
		{
			auto rect = convertStringToD2D1Rect(entry->object.Get());
			ret->SetAnimationValue(rect.bottom, animation_value_type_bottom_location);
			ret->SetAnimationValue(rect.left, animation_value_type_left_location);
			ret->SetAnimationValue(rect.top, animation_value_type_top_location);
			ret->SetAnimationValue(rect.right, animation_value_type_right_location);

		}
		else if (entry->key.Compare(L"Length"))
		{
			int length = 0;
			if (entry->object->ConvertToInt(&length))
				ret->SetMillisecondLength(length);
		}
		else if (entry->key.Compare(L"RefreshRate"))
		{
			int length = 0;
			if (entry->object->ConvertToInt(&length))
				ret->SetMillisecondRefresh(length);
		}








		entry = attributes.GetEntryAt(index++);
	}


	return ret;
}
