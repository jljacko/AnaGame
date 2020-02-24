#include "TGradientStop.h"

TGradientStop::TGradientStop(const TColor& color, float position)
{
	SetColor(color);
	SetPosition(position);
}

TGradientStop::TGradientStop(const TGradientStop& stop)
{
	gradient = stop.gradient;
}

TGradientStop::TGradientStop()
{
	gradient.position = 0.0f;
	gradient.color = D2D1::ColorF(D2D1::ColorF::Black);
}

void TGradientStop::SetColor(const TColor& color)
{
	gradient.color = color.GetColor();
}

void TGradientStop::SetPosition(float position)
{
	gradient.position = position;
}

void TGradientStop::SetGradient(const GRADIENT_STOP_2D& stop)
{
	gradient = stop;
}

TColor TGradientStop::GetColor()const
{
	return TColor(gradient.color);
}

float TGradientStop::GetPosition()const
{
	return gradient.position;
}

GRADIENT_STOP_2D TGradientStop::GetGradient() const
{
	return gradient;
}
