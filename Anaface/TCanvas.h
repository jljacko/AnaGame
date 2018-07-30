#pragma once
#include "TControl.h"



class _ANAFACE_DLL TCanvas :	public TControl
{
public:
	TCanvas(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta);
	~TCanvas();

	bool onCreate(RECT);
	void addGeoGroup(TrecPointer<GeoGroup>);
	virtual UCHAR* GetAnaGameType()override;
private:
	TArray<GeoGroup> geometry;
};

