#include "TColor.h"

TColor::TColor()
{
	SetColor(t_color::Black);
}

TColor::TColor(t_color::Enum en)
{
	SetColor(en);
}

TColor::TColor(float r, float g, float b)
{
	SetColor(r, g, b);
}

TColor::TColor(float r, float g, float b, float a)
{
	SetColor(r, g, b, a);
}

TColor::TColor(const TColor& c)
{
	SetColor(c);
}

TColor::TColor(const color_struct& color)
{
	this->color = color;
}

void TColor::SetColor(t_color::Enum en)
{
	color = t_color(en);
}

void TColor::SetColor(float r, float g, float b)
{
	color = t_color(r, g, b);
}

void TColor::SetColor(float r, float g, float b, float a)
{
	color = t_color(r, g, b, a);
}

void TColor::SetColor(const TColor& co)
{
	color = co.GetColor();
}

float TColor::GetRed()const
{
	return color.r;
}

float TColor::GetBlue()const
{
	return color.b;
}

float TColor::GetGreen()const
{
	return color.g;
}

float TColor::GetOpacity()const
{
	return color.a;
}

color_struct TColor::GetColor()const
{
	return color;
}
