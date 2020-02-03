#include "TTreeDataBind.h"
#include <d2d1.h>

TTreeDataBind::TTreeDataBind(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta): TControl(rt, ta)
{
	isNodeSelected = isTickSelected = false;
	nodeSelected = 0;
}

TTreeDataBind::~TTreeDataBind()
{
}

void TTreeDataBind::onDraw(TObject* obj)
{
	if (!mainNode.Get())
		return;

	TrecPointer<TControl> cont = children.ElementAt(0);
	if (!cont.Get())
		return;

	TrecPointer<TObjectNode> curNode = mainNode;

	D2D1_RECT_F cLoc = location;
	cLoc.bottom = cLoc.top + 30;
	ID2D1PathGeometry* path = nullptr;
	ID2D1Factory* fact = nullptr;
	renderTarget->GetFactory(&fact);
	
	ID2D1SolidColorBrush* outerBrush = nullptr;
	ID2D1SolidColorBrush* innerBrush = nullptr;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &outerBrush);
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Wheat), &innerBrush);
	
	for (UINT c = 0; curNode.Get(); c++)
	{
		bool ext = curNode->IsExtended();
		bool extDraw = curNode->IsExtendable();

		UINT level = curNode->GetLevel();
		D2D1_RECT_F triLoc = cLoc;
		triLoc.bottom -= 5;
		triLoc.top += 5;
		triLoc.left += 5 + level * 5;
		triLoc.right = triLoc.left + 20;

		if (fact)
			fact->CreatePathGeometry(&path);
		
		if (extDraw && path)
		{
			ID2D1GeometrySink* sink = nullptr;
			path->Open(&sink);
			if (sink)
			{
				sink->SetFillMode(D2D1_FILL_MODE_WINDING);
				D2D1_POINT_2F points[2];

				if (ext)
				{
					sink->BeginFigure(D2D1::Point2F(triLoc.left, triLoc.bottom), D2D1_FIGURE_BEGIN_FILLED);
					points[0] = D2D1::Point2F(triLoc.right, triLoc.top);
					points[1] = D2D1::Point2F(triLoc.right, triLoc.bottom);
				}
				else
				{
					sink->BeginFigure(D2D1::Point2F(triLoc.left, triLoc.bottom), D2D1_FIGURE_BEGIN_FILLED);
					points[0] = D2D1::Point2F(triLoc.left, triLoc.top);
					points[1] = D2D1::Point2F(triLoc.right, triLoc.bottom + triLoc.bottom / 2.0f);
				}
				sink->AddLines(points, ARRAYSIZE(points));
				sink->EndFigure(D2D1_FIGURE_END_CLOSED);

				if (SUCCEEDED(sink->Close()))
				{
					if (outerBrush)
						renderTarget->DrawGeometry(path, outerBrush, 0.5f);
					if (innerBrush)
						renderTarget->FillGeometry(path, innerBrush);

				}
				sink->Release();
				sink = nullptr;
			}
		}
		path->Release();
		path = nullptr;

		UINT r = triLoc.right;

		triLoc = cLoc;

		triLoc.left = r;

		cont->setLocation(triLoc);
		cont->onDraw(curNode.Get());

		// Prepare for the next draw
		cLoc.bottom += 30;
		cLoc.top += 30;

		curNode = mainNode->GetNodeAt(c + 1, 0);
	}
}

UCHAR* TTreeDataBind::GetAnaGameType()
{
	return nullptr;
}

bool TTreeDataBind::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	TControl::onCreate(r, d3d);
	return false;
}

void TTreeDataBind::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	if (isContained(&point, &location))
	{
		if (mainNode.Get())
		{
			float dist = point.y - location.top;

			UINT targetNode = static_cast<UINT>(dist) / 30;

			TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(0, targetNode);

			if (tNode.Get())
			{
				UINT level = tNode->GetLevel();
				D2D1_RECT_F cLoc = location;
				cLoc.top = cLoc.top + 30 * targetNode;
				cLoc.bottom = cLoc.top + 30;
				D2D1_RECT_F triLoc = cLoc;
				triLoc.bottom -= 5;
				triLoc.top += 5;
				triLoc.left += 5 + level * 5;
				triLoc.right = triLoc.left + 20;

				if (isContained(&point, &triLoc))
				{
					isTickSelected = false;
					
				}
				clickedButtons.push_back(this);
				isNodeSelected = true;
				nodeSelected = targetNode;
			}
		}

		TControl::OnLButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
	}
}

void TTreeDataBind::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (isContained(&point, &location))
	{
		if (mainNode.Get())
		{
			float dist = point.y - location.top;

			UINT targetNode = static_cast<UINT>(dist) / 30;

			TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(0, targetNode);

			if (tNode.Get())
			{
				UINT level = tNode->GetLevel();
				D2D1_RECT_F cLoc = location;
				cLoc.top = cLoc.top + 30 * targetNode;
				cLoc.bottom = cLoc.top + 30;
				D2D1_RECT_F triLoc = cLoc;
				triLoc.bottom -= 5;
				triLoc.top += 5;
				triLoc.left += 5 + level * 5;
				triLoc.right = triLoc.left + 20;

				if (isContained(&point, &triLoc) && isTickSelected && targetNode == nodeSelected)
				{
					if (tNode->IsExtended())
						tNode->DropChildNodes();
					else if(tNode->IsExtendable())
					{
						tNode->Extend();
					}
				}
			}
		}
		TControl::OnLButtonUp(nFlags, point, mOut, eventAr);
	}
	isTickSelected = isNodeSelected = false;
	nodeSelected = 0;
}

void TTreeDataBind::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	TControl::OnMouseMove(nFlags, point, mOut, eventAr);
}

void TTreeDataBind::SetNode(TrecPointer<TObjectNode> newNode)
{
	mainNode = newNode;
}

D2D1_RECT_F TTreeDataBind::getLocation()
{
	D2D1_RECT_F ret = location;
	if (mainNode.Get())
		ret.bottom = ret.top + 30 * (mainNode->TotalChildren() + 1);
	return ret;
}
