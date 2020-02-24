#pragma once
#include <TObject.h>
#include "TGradientStop.h"
#include <TDataArray.h>
class TGradientStopCollection :	public TObject
{
public:
	TGradientStopCollection();
	TGradientStopCollection(const TGradientStopCollection& col);

	UINT AddGradient(const TGradientStop& newGradient);
	UINT GetGradientCount()const;
	
	bool IsValid(UINT index)const;
	TGradientStop GetGradientStopAt(UINT index)const;

	bool SetGradientAt(const TGradientStop& gradient, UINT index);
	bool SetColorAt(const TColor& color, UINT index);
	bool SetPositionAt(float position, UINT index);

	void Empty();

	TDataArray<GRADIENT_STOP_2D> GetRawCollection()const;
protected:
	TDataArray<TGradientStop> gradients;
};

