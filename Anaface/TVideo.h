#pragma once
#include "TControl.h"
#include <TPlayer.h>

class TPlayPauseButton :public TControl
{
	friend class TVideo;
public:
	TPlayPauseButton(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta);
	~TPlayPauseButton();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	void onDraw(TObject* obj = nullptr)override;

	virtual UCHAR* GetAnaGameType()override;

private:
	bool state;
	TrecComPointer<ID2D1SolidColorBrush> brush;
	class TVideo* targetPlayer;
	bool vidButFocus;
};




class _ANAFACE_DLL TVideo :	public TControl
{
public:
	TVideo(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta, HWND win);
	~TVideo();
	bool setVideo(const WCHAR* str);
	bool Play();
	bool Pause();
	bool Stop();
	bool Speed(float);
	// Time Functions

	bool onCreate(RECT r)override;
	void onDraw(TObject* obj = nullptr)override;

private:
	TPlayer* player;
	HWND window;
};

