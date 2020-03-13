#pragma once
#include <Animation.h>
#include <TBitmapBrush.h>


class GifAnimation : public Animation
{
public:
	GifAnimation(animation_phase ap);

	virtual void Prepare();
	virtual bool Update(float progress);
	virtual void SetAnimationValue(float value, animation_value_type type);

	virtual void SetComponent(TrecPointer<TBrush> comp);
	virtual void SetControl(TrecPointer<TControl> con);

	virtual TString GetAnimationType();

protected:
	TrecSubPointer<TBrush, TBitmapBrush> bitBrush;
	UINT frameCount;
};

