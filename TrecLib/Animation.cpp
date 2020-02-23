#include "Animation.h"

Animation::Animation(animation_phase ap)
{
	phase = ap;
	millisecondRefresh = millisecondLength = millisecondStoryLength = 0;
	done = false;
	reverse = true;
}

void Animation::SetReverse(bool rev)
{
	reverse = rev;
}

bool Animation::GetReverse()
{
	return reverse;
}

animation_phase Animation::GetAnimationPhase()
{
	return phase;
}

UINT Animation::GetMillisecondRefresh()
{
	return millisecondRefresh;
}

void Animation::SetMillisecondRefresh(UINT mr)
{
	millisecondRefresh = mr;
}

UINT Animation::GetMillisecondLength()
{
	return millisecondLength;
}

void Animation::SetMillisecondLength(UINT ml)
{
	millisecondLength = ml;
}

UINT Animation::GetMillisecondStoryLength()
{
	return millisecondStoryLength;
}

void Animation::SetMillisecindStoryLength(UINT msl)
{
	millisecondStoryLength = msl;
}


float GetFloatBetween(float value1, float value2, float progress)
{
	float diff = value2 - value1;

	return value1 + diff * progress;
}