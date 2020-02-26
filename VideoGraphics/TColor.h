#pragma once
#include <TObject.h>
#include <d2d1.h>
#include "Structure2D.h"


#define t_color D2D1::ColorF

#define color_struct D2D1_COLOR_F

class _VIDEO_GRAPHICS TColor : public TObject
{
public:
	TColor();
	TColor(t_color::Enum);
	TColor(float r, float g, float b);
	TColor(float r, float g, float b, float a);
	TColor(const TColor& c);
	TColor(const color_struct& color);

	void SetColor(t_color::Enum);
	void SetColor(float r, float g, float b);
	void SetColor(float r, float g, float b, float a);
	void SetColor(const TColor& co);

	float GetRed()const;
	float GetBlue()const;
	float GetGreen()const;
	float GetOpacity()const;

	color_struct GetColor()const;
private:
	color_struct color;
};

