
#include "TCanvas.h"


TCanvas::TCanvas(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta):TControl(rt,ta)
{
}


TCanvas::~TCanvas()
{
}

bool TCanvas::onCreate(RECT r, TrecPointer<TWindowEngine> d3d)
{
	for (int c = 0; c < geometry.Count();c++)
		geometry.ElementAt(c)->onCreate(r);
	return false;
}

void TCanvas::addGeoGroup(TrecPointer<GeoGroup> gg)
{
	if (gg.Get())
		geometry.Add(gg);
}

UCHAR * TCanvas::GetAnaGameType()
{
	return nullptr;
}
