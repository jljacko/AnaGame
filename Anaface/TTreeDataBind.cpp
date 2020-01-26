#include "TTreeDataBind.h"

TTreeDataBind::TTreeDataBind(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta): TControl(rt, ta)
{
}

TTreeDataBind::~TTreeDataBind()
{
}

void TTreeDataBind::onDraw(TObject* obj)
{
	if (!mainNode.Get())
		return;

	TrecPointer<TObjectNode> curNode = mainNode;

	D2D1_RECT_F cLoc = location;
	cLoc.bottom = cLoc.top + 30;

	while (curNode.Get())
	{

	}
}

UCHAR* TTreeDataBind::GetAnaGameType()
{
	return nullptr;
}

bool TTreeDataBind::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	return false;
}

void TTreeDataBind::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
}

void TTreeDataBind::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
}

void TTreeDataBind::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
}

void TTreeDataBind::SetNode(TrecPointer<TObjectNode> newNode)
{
}

D2D1_RECT_F TTreeDataBind::getLocation()
{
	return D2D1_RECT_F();
}
