#include "TPromptControl.h"

TPromptControl::TPromptControl(TrecComPointer<ID2D1RenderTarget>rt, TrecPointer<TArray<styleTable>> ta, HWND w):TTextField(rt,ta,w)
{
	processRunning = false;
}

TPromptControl::~TPromptControl()
{
}

bool TPromptControl::onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)
{
	return false;
}

void TPromptControl::onDraw(TObject* obj)
{
	TString shellOutput(shell.GetOutput());

	if (shellOutput.GetSize())
		TTextField::SetText(text + shellOutput);

	if (processRunning && !shell.CheckProcess())
	{
		processRunning = false;
		TTextField::SetText(text + L"\n\n" + shell.GetOutput() + L"\n\n" + shell.GetWorkingDirectory() + L"\n\n-->" + input);
	}

	TControl::onDraw(obj);
}

void TPromptControl::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
}

bool TPromptControl::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	switch (nChar)
	{

	}
	return false;
}

void TPromptControl::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
}

void TPromptControl::SetText(TString)
{
}


