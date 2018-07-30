/*#include "stdafx.h"
#include "AnafaceUI.h"
//using namespace ControlTypeSafety;

TControlPointer * ControlTypeSafety::getNewTControl(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta)
{
	TControlPointer* tcp = new TControlPointer;
	if(!tcp)
		return nullptr;
	tcp->contType = tc_TControl;
	tcp->pointer.tc = new TControl(rt, ta);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.tc);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer ControlTypeSafety::getNewTLayout(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta)
{
	TControlPointer tcp;
	tcp->contType = tc_TLayout;
	tcp->pointer.tl = std::make_shared<TLayout>(rt, ta);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.tl);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer * ControlTypeSafety::getNewComboBox(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TComboBox;
	tcp->pointer.tcob = new TComboBox(rt, ta);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.tcob);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer * ControlTypeSafety::getNewRadioButton(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TRadioButton;
	tcp->pointer.trb = new TRadioButton(rt, ta);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.trb);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer * ControlTypeSafety::getNewCheckBox(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TCheckBox;
	tcp->pointer.tchb = new TCheckBox(rt, ta);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.tchb);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer * ControlTypeSafety::getNewTextField(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta,HWND hw)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TTextField;
	tcp->pointer.ttf = new TTextField(rt, ta,hw);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.ttf);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer * ControlTypeSafety::getNewArena(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta, HWND w)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TArena;
	tcp->pointer.ta = new TArena(rt, ta,w);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.ta);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer * ControlTypeSafety::getNewVideo(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta, HWND win)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TVideo;
	try{
	tcp->pointer.tv = new TVideo(rt, ta,win);
	}
	catch (int i)
	{
		if (i == 1)
		{
			if (tcp->pointer.tv)
				delete tcp->pointer.tv;
			delete tcp;
			return nullptr;
		}
	}
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.tv);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer * ControlTypeSafety::getNewFlyout(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TFlyout;
	tcp->pointer.tf = new TFlyout(rt, ta);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.tf);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

TControlPointer * ControlTypeSafety::getNewContextMenu(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TContextMenu;
	tcp->pointer.tcm = new TContextMenu(rt, ta);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.tcm);
	tcp->rootType->setWrapperPointer(tcp);
	return tcp;
}

_ANAFACE_DLL TControlPointer * ControlTypeSafety::getNewCanvas(ID2D1RenderTarget * rt, std::shared_ptr<TArray<styleTable>> ta)
{
	TControlPointer* tcp = new TControlPointer;
	if (!tcp)
		return nullptr;
	tcp->contType = tc_TCanvas;
	tcp->pointer.tca = new TCanvas(rt,ta);
	tcp->rootType = std::shared_ptr<TControl>(tcp->pointer.tca);
}

_ANAFACE_DLL TControlPointer * ControlTypeSafety::getNewGeoGroup(ID2D1RenderTarget * rt)
{
	TControlPointer* tcp = new TControlPointer;
	if(!tcp)
		return nullptr;
	tcp->contType = tc_GeoGroup;
	tcp->pointer.geo = new GeoGroup(rt);
	tcp->rootType = NULL;    // Geo Group is not a TControl
}

bool ControlTypeSafety::tcp_onCreate(TControlPointer* tcp, RECT l)
{
	if (!tcp)
		return false;
	switch (tcp->contType)
	{
	case tc_TArena:
		if (tcp->pointer.ta)
			tcp->pointer.ta->onCreate(l);
		break;
	case tc_TCheckBox:
		if (tcp->pointer.tchb)
			tcp->pointer.tchb->onCreate(l);
		break;
	case tc_TComboBox:
		if (tcp->pointer.tcob)
			tcp->pointer.tcob->onCreate(l);
		break;
	case tc_TContextMenu:
		if (tcp->pointer.tcm)
			tcp->pointer.tcm->onCreate();
		break;
	case tc_TControl:
		if (tcp->pointer.tc)
			tcp->pointer.tc->onCreate(l);
		break;
	case tc_TFlyout:
		if (tcp->pointer.tf)
			tcp->pointer.tf->onCreate(l);
		break;
	case tc_TLayout:
		if (tcp->pointer.tl)
			tcp->pointer.tl->onCreate(l);
		break;
	case tc_TRadioButton:
		if (tcp->pointer.trb)
			tcp->pointer.trb->onCreate(l);
		break;
	case tc_TTextField:
		if (tcp->pointer.ttf)
			tcp->pointer.ttf->onCreate(l);
		break;
	case tc_TVideo:
		if (tcp->pointer.tv)
			tcp->pointer.tv->onCreate(l);
		break;
	case tc_TCanvas:
		if (tcp->pointer.tca)
			tcp->pointer.tca->onCreate(l);
		break;
	case tc_GeoGroup:
		if (tcp->pointer.geo)
			tcp->pointer.geo->onCreate(l);
	}
}

void  ControlTypeSafety::tcp_onDraw(TControlPointer* tcp)
{
	if (!tcp)
		return ;
	switch (tcp->contType)
	{
	case tc_TArena:
		if (tcp->pointer.ta)
			tcp->pointer.ta->onDraw();
		break;
	case tc_TCheckBox:
		if (tcp->pointer.tchb)
			tcp->pointer.tchb->onDraw();
		break;
	case tc_TComboBox:
		if (tcp->pointer.tcob)
			tcp->pointer.tcob->onDraw();
		break;
//	case tc_TContextMenu:
	//	if (tcp->pointer.tcm)
		//	tcp->pointer.tcm->onDraw();
		//break;
	case tc_TControl:
		if (tcp->pointer.tc)
			tcp->pointer.tc->onDraw();
		break;
	case tc_TFlyout:
		if (tcp->pointer.tf)
			tcp->pointer.tf->onDraw();
		break;
	case tc_TLayout:
		if (tcp->pointer.tl)
			tcp->pointer.tl->onDraw();
		break;
	case tc_TRadioButton:
		if (tcp->pointer.trb)
			tcp->pointer.trb->onDraw();
		break;
	case tc_TTextField:
		if (tcp->pointer.ttf)
			tcp->pointer.ttf->onDraw();
		break;
	case tc_TVideo:
		if (tcp->pointer.tv)
			tcp->pointer.tv->onDraw();
		break;
	case tc_TCanvas:
		if (tcp->pointer.tca)
			tcp->pointer.tca->onDraw();
		break;
	case tc_GeoGroup:
		if (tcp->pointer.geo)
			tcp->pointer.geo->onDraw();
	}
}

bool tcp_addAttribute(TControlPointer* tcp, WCHAR* wch, std::shared_ptr<TString> tstr)
{
	if (!tcp || !wch || !tstr)
		return false;
	if (tcp->rootType)
		return tcp->rootType->addAttribute(wch, tstr);
	else if (tcp->contType == tc_GeoGroup && tcp->pointer.geo)
		return tcp->pointer.geo->addAttribute(wch, tstr);
	else
		return false;
}

void  ControlTypeSafety::deleteControl(TControlPointer** tcp_dp)
{
	if (*tcp_dp == NULL)
		return;

	TControlPointer* tcp = *tcp_dp;

	switch (tcp->contType)
	{
	case tc_TArena:
		if (tcp->pointer.ta)
			delete tcp->pointer.ta;
		break;
	case tc_TCheckBox:
		if (tcp->pointer.tchb)
			delete tcp->pointer.tchb;
		break;
	case tc_TComboBox:
		if (tcp->pointer.tcob)
			delete tcp->pointer.tcob;
		break;
	case tc_TContextMenu:
		if (tcp->pointer.tcm)
			delete tcp->pointer.tcm;
		break;
	case tc_TControl:
		if (tcp->pointer.tc)
			delete tcp->pointer.tc;
		break;
	case tc_TFlyout:
		if (tcp->pointer.tf)
			delete tcp->pointer.tf;
		break;
	case tc_TLayout:
		if (tcp->pointer.tl)
			delete tcp->pointer.tl;
		break;
	case tc_TRadioButton:
		if (tcp->pointer.trb)
			delete tcp->pointer.trb;
		break;
	case tc_TTextField:
		if (tcp->pointer.ttf)
			delete tcp->pointer.ttf;
		break;
	case tc_TVideo:
		if (tcp->pointer.tv)
			delete tcp->pointer.tv;
		break;
	}

	delete tcp;

	*tcp_dp = NULL;
}
*/