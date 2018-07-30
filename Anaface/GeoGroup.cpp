#include "stdafx.h"
#include "GeoGroup.h"
#include "AnafaceUI.h"


GeoGroup::GeoGroup(ID2D1RenderTarget * rt)
{
	brush = nullptr;
	secondColor = false;
	useRadial = false;
	gradients[0].color = D2D1::ColorF(D2D1::ColorF::Black);
	gradients[0].position = 0.0f;
	gradients[1].color = D2D1::ColorF(D2D1::ColorF::Black);
	gradients[1].position = 1.0f;

	crunch = true;
}

GeoGroup::~GeoGroup()
{
	if (brush.get())
	{
		brush = nullptr;
	}
}

bool GeoGroup::onCreate(RECT r)
{
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|FillBrush"));
	if(valpoint.get())
	{
		color1 = convertStringToD2DColor(valpoint.get());
		for (int c = 0; c < children.Count();c++)
		{
			children.ElementAt(c)->setFill(true);
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|Brush2"));
	if (valpoint.get())
	{
		color2 = convertStringToD2DColor(valpoint.get());
		secondColor = true;
	}

	valpoint = attributes.retrieveEntry(TString(L"|StrokeBrush"));
	if (valpoint.get())
	{
		color1 = convertStringToD2DColor(valpoint.get());
		for (int c = 0; c < children.Count();c++)
		{
			children.ElementAt(c)->setFill(false);
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|Thickness"));
	float floaty = 0.0;
	if (valpoint.get() && !valpoint->ConvertToFloat(&floaty))
	{
		for (int c = 0;c < children.Count();c++)
		{
			children.ElementAt(c)->setThickness(floaty);
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|Gradient1"));
	if (valpoint.get() && !valpoint->ConvertToFloat(&floaty))
		gradients[0].position = floaty;

	valpoint = attributes.retrieveEntry(TString(L"|Gradient2"));
	if (valpoint.get() && !valpoint->ConvertToFloat(&floaty))
		gradients[1].position = floaty;

	valpoint = attributes.retrieveEntry(TString(L"|GraientMode"));
	if (valpoint.get() && !valpoint->Compare(L"Radial"))
		useRadial = true;


	valpoint = attributes.retrieveEntry(TString(L"|CompressGeometry"));
	if (valpoint.get() && !valpoint->Compare(L"False"))
		crunch = false;


	if(!brush.get())
		return false;

	if (!secondColor)
	{
		ID2D1SolidColorBrush* solBrush = nullptr;
		renderTarget->CreateSolidColorBrush(color1, &solBrush);
		brush = solBrush;
	}
	else
	{
		ID2D1GradientStopCollection* gsc = nullptr;
		renderTarget->CreateGradientStopCollection(gradients, 2, &gsc);
		stopColl = gsc;
		if (useRadial)
		{
			ID2D1RadialGradientBrush* radBrush = nullptr;
			renderTarget->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(r.left, r.top),
				D2D1::Point2F(r.right, r.bottom),
				r.right - r.left, r.bottom - r.top),
				stopColl.get(), &radBrush);
			brush = radBrush;
		}
		else
		{
			ID2D1LinearGradientBrush* linBrush = nullptr;
			renderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(r.left, r.top),
				D2D1::Point2F(r.right, r.bottom)),
				stopColl.get(), &linBrush);
			brush = linBrush;
		}

	}


	for (int c = 0; c < children.Count();c++)
	{
		children.ElementAt(c)->onCreate(r, crunch);
	}

	return true;
}

void GeoGroup::onDraw()
{
	if (!brush.get() || !renderTarget.get())
		return;

	for (int c = 0; c < children.Count();c++)
	{
		children.ElementAt(c)->onDraw(brush.get(), renderTarget.get());
	}
}

bool GeoGroup::addAttribute(WCHAR * attr, TrecPointer<TString> value)
{
	if(!attr || !value.get())
	return false;
	attributes.addEntry(TString(attr), value);
	return true;
}

void GeoGroup::addGeometry(TGeometry * tg)
{
	if (tg)
	{
		children.Add(new TGeometry(tg));
	}
}

