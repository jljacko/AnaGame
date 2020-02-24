#pragma once
#include "TObject.h"
#include "TrecReference.h"
#include <d2d1_1.h>

/*
 * class Drawer
 * Provides a low-level interface for the Window class so that libraries below the Window class could still call upon them to draw
 */
class _TREC_LIB_DLL Drawer :
	public TObject
{
public:
	virtual void Draw()=0;
    HWND GetWindowHandle();
    TrecComPointer<ID2D1Factory1> GetFactory();

protected:
    TrecComPointer<ID2D1Factory1> directFactory;
    HWND parent, currentWindow;
};

