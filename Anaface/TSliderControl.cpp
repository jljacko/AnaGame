#include "TSliderControl.h"



TSliderControl::TSliderControl(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> styles) : TControl(rt,styles,false)
{
}


TSliderControl::~TSliderControl()
{
}

UCHAR * TSliderControl::GetAnaGameType()
{
	return nullptr;
}
