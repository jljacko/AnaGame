#pragma once
#include <TObject.h>
#include "TColor.h"
#include "Structure2D.h"
class _VIDEO_GRAPHICS TGradientStop : public TObject
{
public:
	TGradientStop(const TColor& color, float position);
	TGradientStop(const TGradientStop& stop);
	TGradientStop();

	void SetColor(const TColor& color);
	void SetPosition(float position);
	void SetGradient(const GRADIENT_STOP_2D& stop);

	TColor GetColor()const;
	float GetPosition()const;
	GRADIENT_STOP_2D GetGradient()const;
protected:
	GRADIENT_STOP_2D gradient;
};

