#pragma once
#include "TObject.h"
#include "TrecReference.h"
#include "TControlComponent.h"

typedef enum animation_phase
{
	animation_pase_beginning,
	animation_phase_end,
	animation_phase_middle,
	animation_phase_persistant
}animation_phase;


typedef enum animation_value_type
{
	animation_value_type_width,
	animation_value_type_hight,
	animation_value_type_top_location,
	animation_value_type_left_location,
	animation_value_type_right_location,
	animation_value_type_bottom_location,
	animation_value_type_red,
	animation_value_type_green,
	animation_value_type_blue,
	animation_value_type_alpha,
	animation_value_type_degree,
	animation_value_type_rotate_start,
	animation_value_type_rotate_end,
	animation_value_type_radian,
	animation_value_type_direction,
	animation_value_type_frame,
	animation_value_type_red2,
	animation_value_type_green2,
	animation_value_type_blue2,
	animation_value_type_alpha2,
	animation_value_type_special
}animation_value_type;

class _TREC_LIB_DLL Animation :
	public TObject
{
public:
	Animation(animation_phase ap);

	animation_phase GetAnimationPhase();
	UINT GetMillisecondRefresh();
	void SetMillisecondRefresh(UINT mr);
	UINT GetMillisecondLength();
	void SetMillisecondLength(UINT ml);
	UINT GetMillisecondStoryLength();
	void SetMillisecindStoryLength(UINT msl);
	void SetReverse(bool rev);
	bool GetReverse();
	virtual void Prepare() = 0;
	virtual bool Update(float progress) = 0;
	virtual void SetAnimationValue(float value, animation_value_type type) = 0;

	virtual void SetComponent(TrecComPointer<ID2D1Brush> comp) = 0;

	virtual TString GetAnimationType() = 0;

protected:
	UINT millisecondRefresh;
	UINT millisecondLength;
	UINT millisecondStoryLength;
	animation_phase phase;
	bool done, reverse;
};

float _TREC_LIB_DLL GetFloatBetween(float value1, float value2, float progress);