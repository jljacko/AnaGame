#pragma once
#include "TWindow.h"

typedef enum ide_page_type
{
	ide_page_type_body,
	ide_page_type_basic_console,
	ide_page_type_deep_console,
	ide_page_type_upper_right,
	ide_page_type_lower_right,
	ide_page_type_upper_left,
	ide_page_type_lower_left
}ide_page_type;


class _TAP_DLL TIdeWindow :
	public TWindow
{
public:
	TIdeWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins);
};

