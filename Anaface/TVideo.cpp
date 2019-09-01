#include "stdafx.h"
#include "TVideo.h"


TVideo::TVideo(TrecComPointer<ID2D1RenderTarget>rt, TrecPointer<TArray<styleTable>> st,HWND win):TControl(rt, st,false)
{
	if (!win)
		throw 1;
	window = win;

	HRESULT hres = TPlayer::CreateInstance(window, window, &player);
	if (FAILED(hres))
	{
		player = nullptr;
		throw 2;
	}
}

TVideo::~TVideo()
{
	if (player)
		player->Shutdown();
}

bool TVideo::setVideo(const WCHAR* str)
{
	if(!player)
		return false;
	if (FAILED(player->OpenURL(str)))
		return false;
	return true;
}

bool TVideo::Play()
{
	if (!player)
		return false;
	if (FAILED(player->Play()))
		return false;
	return true;
}

bool TVideo::Pause()
{
	if (!player)
		return false;
	if (FAILED(player->Pause()))
		return false;
	return true;
}

bool TVideo::Stop()
{
	if (!player)
		return false;
	if (FAILED(player->Stop()))
		return false;
	return true;
}

bool TVideo::Speed(float r)
{
	return false;
}

bool TVideo::onCreate(RECT r)
{
	if (!player)
		return false;
	bool returnable = TControl::onCreate(r);
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|MediaSource"));
	if (valpoint.Get())
	{
		if (!setVideo(valpoint->GetConstantBuffer()))
			returnable = false;
	}
	player->ResizeVideo(snip);

	valpoint = attributes.retrieveEntry(TString(L"|ExternalPlayButton"));
	if (!valpoint.Get() || !valpoint->Compare(L"False"))
	{
		//TrecPointer<TContainer> cont = new TContainer();
		TrecPointer<TControl> tppb = TrecPointerKey::GetNewTrecPointerAlt<TControl, TPlayPauseButton>(renderTarget, TrecPointer<TArray<styleTable>>());
		//TControlPointer* tcp = new TControlPointer{ tc_TControl, tppb,TrecPointer<TControl>(tppb) };

		if (tppb.Get())
			dynamic_cast<TPlayPauseButton*>(tppb.Get())->targetPlayer = this;

		tppb->setParent(TrecPointerKey::GetTrecPointerFromSoft<TControl>(tThis));
		//cont->setTControl(tppb.Get());
		tppb->onCreate(RECT{ snip.left,snip.bottom - 40, snip.left + 50, snip.bottom });
		TrecPointer<TControl> tc;
		tc = tppb;
		children.Add(tc);
	}


	return returnable;
}

void TVideo::onDraw(TObject* obj)
{
	TControl::onDraw(obj);
	if(player)
	player->Repaint();
}

TPlayPauseButton::TPlayPauseButton(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta):TControl(rt,ta,false)
{
	if (rt.Get())
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder brushRaw;
		rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), brushRaw.GetPointerAddress());
		brush = brushRaw.Extract();
	}
	targetPlayer = nullptr;
	state = true;
	vidButFocus = false;
}

TPlayPauseButton::~TPlayPauseButton()
{
	brush.Nullify();
}

void TPlayPauseButton::OnLButtonDown(UINT nFlags, CPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr)
{
	TControl::OnLButtonDown(nFlags, point, mOut,eventAr);

	if (!targetPlayer)
		return;

	if (*mOut != negative && *mOut != negativeUpdate)
	{
		state = !state;
		if (state)
			targetPlayer->Pause();
		else
			targetPlayer->Play();
	}
}

void TPlayPauseButton::onDraw(TObject* obj)
{
	int left = snip.left + (snip.right - snip.left) / 3;
	int right = snip.right - (snip.right - snip.left) / 3;
	int top = snip.top + (snip.bottom - snip.top) / 4;
	int bottom = snip.bottom - (snip.bottom - snip.top) / 4;
	if (state) // Pause
	{
		int mid = snip.top + (snip.bottom - snip.top) / 2;
		D2D1_POINT_2F up = D2D1::Point2F(left, top);
		D2D1_POINT_2F down = D2D1::Point2F(left, bottom);
		D2D1_POINT_2F middle = D2D1::Point2F(right, mid);
		
		renderTarget->DrawLine(up, down, brush.Get(), 2.0f);
		renderTarget->DrawLine(up, middle, brush.Get(), 2.0f);
		renderTarget->DrawLine(down, middle, brush.Get(), 2.0f);
	}
	else // Play
	{
		D2D1_RECT_F leftRect = D2D1::RectF(left, top, left + 5, bottom);
		D2D1_RECT_F rightRect = D2D1::RectF(right - 5,top,right,bottom);
		renderTarget->FillRectangle(leftRect, brush.Get());
		renderTarget->FillRectangle(rightRect, brush.Get());
	}
}

UCHAR * TPlayPauseButton::GetAnaGameType()
{
	return nullptr;
}
