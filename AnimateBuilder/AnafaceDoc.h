#pragma once
#include "stdafx.h"
#include <AnafaceParser.h>
#include <TML_Reader_.h>

// AnafaceDoc document

class AnafaceDoc : public CDocument
{
	DECLARE_DYNCREATE(AnafaceDoc)

public:
	AnafaceDoc();
	virtual ~AnafaceDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	void setUpTControl(CDC*);
	void OnDraw();

	TrecPointer<TControl> returnRootControl();
	TrecPointer<TControl> returnFocusControl();

	void UpdateEditeViaMouseClick(UINT flags, CPoint p);
	RECT getArea();
protected:
	virtual BOOL OnNewDocument();
	TrecPointer<TControl> editControl; // root control
	TrecPointer<TControl> focusControl;// control the user is currently focused on
	TrecComPointer<ID2D1Factory> factory;
	TrecComPointer<ID2D1RenderTarget> rt;
	RECT area;

	HWND windowHandle;
	bool windowSet;

	RECT getRectLoation();


	//TrecPointer<TContainer> getNewTContainer();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonNewTControl();
	afx_msg void OnButtonEditTControl();
	afx_msg void OnButtonNewLayout();
	afx_msg void OnButton9();
	afx_msg void OnClick();
};
