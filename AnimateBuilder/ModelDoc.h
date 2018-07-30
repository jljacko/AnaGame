#pragma once
#include "stdafx.h"
// ModelDoc document

#include <ArenaModel.h>
#include <TArena.h>
#include <TrecComPointer.h>
#include <d2d1.h>
#include <d2d1_1.h>

class ModelDoc : public CDocument
{
	DECLARE_DYNCREATE(ModelDoc)

public:
	ModelDoc();
	virtual ~ModelDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void OnDraw(CDC* pDC);
	//void Initialize(std::vector<DirectX::XMFLOAT3>& x3, std::vector<int>& ind, DirectX::XMFLOAT4 color);

protected:
	virtual BOOL OnNewDocument();
	TrecPointer<ArenaModel> modelPointer;
	TrecComPointer<ID2D1Bitmap1> bit;
	TrecPointer<ArenaEngine> enginePointer;

	TrecComPointer<ID2D1Device> device;
	TrecComPointer<ID2D1DeviceContext> contextDevice;
	TrecComPointer<ID2D1GdiInteropRenderTarget> gdiRender;
	TrecComPointer<ID2D1RenderTarget> regRenderTarget;
	TrecComPointer<ID2D1Factory1> fact;
	//static int counter = 0; 

	TrecPointer<TControl> pointer;
	TrecPointer<TArena> arenaControl;
	bool firstDraw;

	HWND windowHandle;
	TString anafaceFile;
	bool anafaceSet;
	RECT area;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNew3DObject();
	afx_msg void OnEdit3DModelCommand();
};
