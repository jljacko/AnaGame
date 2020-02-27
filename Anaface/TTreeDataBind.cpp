#include "TTreeDataBind.h"
#include "TScrollerControl.h"
#include <d2d1.h>

TTreeDataBind::TTreeDataBind(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta): TControl(rt, ta)
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
	TrecPointer<TGeometry> path;
	
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

		
		if (extDraw)
		{

			TDataArray<POINT_2D> points;

			if (ext)
			{
				points.push_back(D2D1::Point2F(triLoc.left, triLoc.bottom));
				points.push_back(D2D1::Point2F(triLoc.right, triLoc.top));
				points.push_back(D2D1::Point2F(triLoc.right, triLoc.bottom));
			}
			else
			{
				points.push_back(D2D1::Point2F(triLoc.left, triLoc.bottom));
				points.push_back(D2D1::Point2F(triLoc.left, triLoc.top));
				points.push_back(D2D1::Point2F(triLoc.right, (triLoc.top + triLoc.bottom) / 2.0f));
			}
			
			path = drawingBoard->GetGeometry(points);

			if (path.Get())
			{
				if (outerBrush.Get())
					outerBrush->DrawGeometry(path, 0.5f);
				if (innerBrush.Get())
					innerBrush->FillGeometry(path);

			}
			
		}


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
	if (drawingBoard.Get())
	{
		outerBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Black));
		innerBrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::Wheat));
	}
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

			TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(targetNode, 0);

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
					isTickSelected = true;
					
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

			TrecPointer<TObjectNode> tNode = mainNode->GetNodeAt(targetNode, 0);

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
						Resize(location);
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

/*
* Method: TControl - Resize
* Purpose: Resizes the control upon the window being resized
* Parameters: RECT r - the new location for the control
*/
void TTreeDataBind::Resize(D2D1_RECT_F& r)
{
	// First Check to see if we need a new scroll control
	D2D1_RECT_F tempLoc = this->getLocation();
	if ((tempLoc.bottom - tempLoc.top > r.bottom - r.top) ||
		(tempLoc.right - tempLoc.left > r.right - r.left))
	{
		if (parent.Get() && !dynamic_cast<TScrollerControl*>(parent.Get()))
		{
			TrecPointer<TControl> scrollControl = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TScrollerControl>(drawingBoard, styles);
			scrollControl->onCreate(r, TrecPointer<TWindowEngine>());
			dynamic_cast<TScrollerControl*>(scrollControl.Get())->SetChildControl(TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis));
			TrecPointerKey::GetTrecPointerFromSoft<TControl>(parent)->SwitchChildControl(tThis, scrollControl);
			parent = TrecPointerKey::GetSoftPointerFromTrec<TControl>(scrollControl);
			location.left = r.left;
			location.top = r.top;
		}
		return;
	}


	// We do not, so proceed
	location = r;
	updateComponentLocation();
	CheckScroll();
/*
	TControl* ele = nullptr;

	
	if (children.Count() && (ele = children.ElementAt(0).Get()))
	{
		D2D1_RECT_F loc;
		if (this->isStack)
		{
			loc.left = location.left;
			loc.right = location.right;
			loc.top = location.top;
			loc.bottom = loc.top + this->widthHeight;
		}
		else
		{
			loc.top = location.top;
			loc.bottom = location.bottom;
			loc.left = location.left;
			loc.right = loc.left + this->widthHeight;
		}
		ele->Resize(loc);
	}*/
}
