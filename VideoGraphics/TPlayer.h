#pragma once
#include <mfapi.h>
#include <mfidl.h>
#include <evr.h>
#include <Mferror.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <strsafe.h>
#include "VideoGraphics.h"
#include <TObject.h>



const UINT WM_APP_PLAYER_EVENT = WM_APP + 1;

typedef _VIDEO_GRAPHICS enum PlayerState
{
	Closed = 0,     // No session.
	Ready,          // Session was created, ready to open a file. 
	OpenPending,    // Session is opening a file.
	Started,        // Session is playing a file.
	Paused,         // Session is paused.
	Stopped,        // Session is stopped (ready to play). 
	Closing         // Application has closed the session, but is waiting for MESessionClosed.
}PlayerState;



class _VIDEO_GRAPHICS  TPlayer : public IMFAsyncCallback, public TObject
{
public:
	static HRESULT CreateInstance(HWND hVideo, HWND hEvent, TPlayer **ppPlayer);
	// IUnknown methods
	STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	// IMFAsyncCallback methods
	STDMETHODIMP  GetParameters(DWORD*, DWORD*);

	STDMETHODIMP  Invoke(IMFAsyncResult* pAsyncResult);

	// Playback
	HRESULT       OpenURL(const WCHAR *sURL);
	HRESULT       Play();
	HRESULT       Pause();
	HRESULT       Stop();
	HRESULT       Shutdown();
	HRESULT       HandleEvent(UINT_PTR pUnkPtr);
	PlayerState   GetState(); 
	HRESULT       SetSpeed(float f);

	// Video functionality
	HRESULT       Repaint();
	HRESULT       ResizeVideo( RECT&);

	BOOL          HasVideo() const { return (m_pVideoDisplay != NULL); }

	virtual UCHAR* GetAnaGameType()override;

protected:
	HRESULT Initialize();
	HRESULT CreateSession();
	HRESULT CloseSession();
	HRESULT StartPlayback();

	// Media event handlers
	virtual HRESULT OnTopologyStatus(IMFMediaEvent *pEvent);
	virtual HRESULT OnPresentationEnded(IMFMediaEvent *pEvent);
	virtual HRESULT OnNewPresentation(IMFMediaEvent *pEvent);

	virtual HRESULT OnSessionEvent(IMFMediaEvent*, MediaEventType)
	{
		return S_OK;
	}

	TPlayer(HWND hVideo, HWND hEvent);
	~TPlayer();

protected:
	long                    m_nRefCount;        // Reference count.

	IMFMediaSession         *m_pSession;
	IMFMediaSource          *m_pSource;
	IMFVideoDisplayControl  *m_pVideoDisplay;

	HWND                    m_hwndVideo;        // Video window.
	HWND                    m_hwndEvent;        // App window to receive events.
	PlayerState             m_state;            // Current state of the media session.
	HANDLE                  m_hCloseEvent;      // Event to wait on while closing.
};

