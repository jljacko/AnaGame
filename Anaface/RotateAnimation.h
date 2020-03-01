#pragma once
#include <Animation.h>

class TControl;

class RotateAnimation :
	public Animation
{
public:
	RotateAnimation(animation_phase);

	virtual bool Update(float progress);
	virtual void SetAnimationValue(float value, animation_value_type type);

	virtual void SetComponent(TrecPointer<TBrush> comp)override;
	virtual void Prepare();
	virtual TString GetAnimationType();

	virtual void SetControl(TrecPointer<TControl> con)override;
protected:
	TrecPointer<TControl> control;
	float rotateStart, rotateEnd;
	bool clockwise;
	bool degrees;

};

