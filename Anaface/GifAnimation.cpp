#include "GifAnimation.h"

TString gifAnimationString(L"Gif");

GifAnimation::GifAnimation(animation_phase ap): Animation(ap)
{
	frameCount = 1;
}

void GifAnimation::Prepare()
{

}

bool GifAnimation::Update(float progress)
{
	if(!bitBrush.Get())
	return false;

	bitBrush->NextFrame();

	return true;
}

void GifAnimation::SetAnimationValue(float value, animation_value_type type)
{
}

void GifAnimation::SetComponent(TrecPointer<TBrush> comp)
{
	bitBrush = TrecPointerKey::GetTrecSubPointerFromTrec<TBrush, TBitmapBrush>(comp);
}

void GifAnimation::SetControl(TrecPointer<TControl> con)
{
	
}

TString GifAnimation::GetAnimationType()
{
	return gifAnimationString;
}
