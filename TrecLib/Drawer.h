#pragma once
#include "TObject.h"

/*
 * class Drawer
 * Provides a low-level interface for the Window class so that libraries below the Window class could still call upon them to draw
 */
class _TREC_LIB_DLL Drawer :
	public TObject
{
public:
	virtual void Draw()=0;
};

