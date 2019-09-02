#pragma once
#include "TByteCode.h"
#include <AnafaceUI.h>
#include "TapParser.h"

class Tap_
{
public:
	Tap_();
	~Tap_();

	int load(CArchive*);
	int run();
	bool onClick(CPoint);
	bool onDblClick(CPoint);
	bool onChar(WCHAR);
	bool onMouseMove(CPoint);



private:
	TMap<pByteCode> functionTable;
	TMap<oByteCode> objectTable;
	TMap<AnafaceUI> UITable;
	AnafaceUI* currentPage;
	TControl* activeInput;

	TArray<use> fileList;
};

