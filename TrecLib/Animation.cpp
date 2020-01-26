#include "Animation.h"

Animation::Animation(animation_phase ap)
{
	phase = ap;
	millisecondRefresh = 0;
	done = false;
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
