#pragma once
#include <Animation.h>

class TControl;

class TransitionAnimation :
	public Animation
{
public:
	TransitionAnimation(animation_phase);

	virtual bool Update(float progress);
	virtual void SetAnimationValue(float value, animation_value_type type);

	virtual void SetComponent(TrecPointer<TBrush> comp)override;
	virtual void Prepare();
	virtual TString GetAnimationType();

	virtual void SetControl(TrecPointer<TControl> con)override;
protected:
	TrecPointer<TControl> control;
	D2D1_RECT_F loc1, loc2;
};

