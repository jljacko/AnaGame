#include "stdafx.h"
#include "TPlayer.h"

template <class T> HRESULT GetEventObject(IMFMediaEvent *event, T ** obj)
{
	if (!obj)
		return E_POINTER;

	*obj = nullptr;

	PROPVARIANT var;
	HRESULT res = event->GetValue(&var);

	if (SUCCEEDED(res))
	{
		if (var.vt == VT_UNKNOWN)
		{
			res = var.punkVal->QueryInterface(obj);
		}
		else
		{
			res = MF_E_INVALIDTYPE;
		}
		PropVariantClear(&var);
	}
	return res;
}

HRESULT CreateMediaSource(PCWSTR sURL, IMFMediaSource ** source)
{
	MF_OBJECT_TYPE type = MF_OBJECT_INVALID;

	IMFSourceResolver* resolve = nullptr;
	IUnknown* uSource = nullptr;

	HRESULT res = MFCreateSourceResolver(&resolve);
	if (FAILED(res))
		goto done;

	res = resolve->CreateObjectFromURL(sURL, MF_RESOLUTION_MEDIASOURCE, nullptr, &type, &uSource);

	if (FAILED(res))
		goto done;
	res = uSource->QueryInterface(IID_PPV_ARGS(source));

done:
	if (resolve)
		resolve->Release();
	if (uSource)
		uSource->Release();

	return res;
}

HRESULT CreateMediaSinkActivate(
	IMFStreamDescriptor *pSourceSD,     // Pointer to the stream descriptor.
	HWND hVideoWindow,                  // Handle to the video clipping window.
	IMFActivate **ppActivate
);

HRESULT AddBranchToPartialTopology(IMFTopology *pTop, IMFMediaSource* source, IMFPresentationDescriptor* pd, DWORD iStream, HWND vidWin);



HRESULT CreatePlaybackTopology(IMFMediaSource* source, IMFPresentationDescriptor* pd, HWND vidWin, IMFTopology ** top)
{
	if (!top)
		return E_POINTER;
	*top = nullptr;
	IMFTopology* pTop = nullptr;

	DWORD streams = 0;

	HRESULT res = MFCreateTopology(&pTop);
	if (FAILED(res))
		goto done;

	res = pd->GetStreamDescriptorCount(&streams);
	if (FAILED(res))
		goto done;

	for (DWORD c = 0; c < streams;c++)
	{
		res = AddBranchToPartialTopology(pTop, source, pd, c, vidWin);
		if (FAILED(res))
			goto done;
	}

	pTop->AddRef();
	*top = pTop;

done:
	return res;


}

// Add a source node to a topology.
HRESULT AddSourceNode(
	IMFTopology *pTopology,           // Topology.
	IMFMediaSource *pSource,          // Media source.
	IMFPresentationDescriptor *pPD,   // Presentation descriptor.
	IMFStreamDescriptor *pSD,         // Stream descriptor.
	IMFTopologyNode **ppNode)         // Receives the node pointer.
{
	IMFTopologyNode *pNode = NULL;

	// Create the node.
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the attributes.
	hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, pSource);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
	if (FAILED(hr))
	{
		goto done;
	}

	// Add the node to the topology.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	if(pNode)
		pNode->Release();
	return hr;
}

// Add an output node to a topology.
HRESULT AddOutputNode(
	IMFTopology *pTopology,     // Topology.
	IMFActivate *pActivate,     // Media sink activation object.
	DWORD dwId,                 // Identifier of the stream sink.
	IMFTopologyNode **ppNode)   // Receives the node pointer.
{
	IMFTopologyNode *pNode = NULL;

	// Create the node.
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the object pointer.
	hr = pNode->SetObject(pActivate);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the stream sink ID attribute.
	hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
	if (FAILED(hr))
	{
		goto done;
	}

	// Add the node to the topology.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	if(pNode)
		pNode->Release();
	return hr;
}

HRESULT AddBranchToPartialTopology(IMFTopology *pTop, IMFMediaSource* source, IMFPresentationDescriptor* pd, DWORD iStream, HWND vidWin)
{
	IMFStreamDescriptor *pSD = nullptr;
	IMFActivate         *pSinkActivate = nullptr;
	IMFTopologyNode     *pSourceNode = nullptr;
	IMFTopologyNode     *pOutputNode = nullptr;

	BOOL sel = FALSE;

	HRESULT res = pd->GetStreamDescriptorByIndex(iStream, &sel, &pSD);
	if (FAILED(res))
		goto done;

	if (sel)
	{
		res = CreateMediaSinkActivate(pSD, vidWin, &pSinkActivate);
		if (FAILED(res))
			goto done;
		res = AddSourceNode(pTop, source, pd, pSD, &pSourceNode);
		if (FAILED(res))
			goto done;
		res = AddOutputNode(pTop, pSinkActivate, 0, &pOutputNode);
		if (FAILED(res))
			goto done;
		res = pSourceNode->ConnectOutput(0, pOutputNode, 0);

	}
done:
	if (pSD)
		pSD->Release();
	if (pSinkActivate)
		pSinkActivate->Release();
	if (pSourceNode)
		pSourceNode->Release();
	if (pOutputNode)
		pOutputNode->Release();
	return res;

}






HRESULT TPlayer::CreateInstance(HWND hVideo, HWND hEvent, TPlayer ** ppPlayer)
{
	if (ppPlayer == NULL)
	{
		return E_POINTER;
	}

	TPlayer *pPlayer = new TPlayer(hVideo, hEvent);
	if (pPlayer == NULL)
	{
		return E_OUTOFMEMORY;
	}
	HRESULT hr = pPlayer->Initialize();
	if (SUCCEEDED(hr))
	{
		*ppPlayer = pPlayer;
	}
	else
	{
		pPlayer->Release();
	}
	return hr;
}

STDMETHODIMP TPlayer::QueryInterface(REFIID iid, void ** ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(TPlayer, IMFAsyncCallback),
		{ 0 }
	};
	return QISearch(this, qit, iid, ppv);
}

STDMETHODIMP_(ULONG) TPlayer::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

STDMETHODIMP_(ULONG) TPlayer::Release()
{
	ULONG uCount = InterlockedDecrement(&m_nRefCount);
	if (uCount == 0)
	{
		delete this;
	}
	return uCount;
}

STDMETHODIMP TPlayer::GetParameters(DWORD *, DWORD *)
{
	return E_NOTIMPL;
}

STDMETHODIMP TPlayer::Invoke(IMFAsyncResult * pAsyncResult)
{
	MediaEventType met = MEUnknown;

	IMFMediaEvent *pEvent = nullptr;

	HRESULT res = m_pSession->EndGetEvent(pAsyncResult, &pEvent);
	if (FAILED(res))
		goto done;

	res = pEvent->GetType(&met);
	if (FAILED(res))
		goto done;

	if (met == MESessionClosed)
	{
		SetEvent(m_hCloseEvent);
	}
	else
	{
		res = m_pSession->BeginGetEvent(this, nullptr);
		if (FAILED(res))
			goto done;
	}

	if (m_state != Closing)
	{
		pEvent->AddRef();

		PostMessage(m_hwndEvent, WM_APP_PLAYER_EVENT, (WPARAM)pEvent, (LPARAM)met);
	}

done:
	if (pEvent)
		pEvent->Release();
	return S_OK;

	return E_NOTIMPL;
}

HRESULT TPlayer::OpenURL(const WCHAR * sURL)
{
	IMFTopology *pTopology = nullptr;
	IMFPresentationDescriptor* PD = nullptr;

	HRESULT res = CreateSession();
	if (FAILED(res))
	{
		goto done;
	}
	res = CreateMediaSource(sURL, &m_pSource);
	if (FAILED(res))
	{
		goto done;
	}

	res = m_pSource->CreatePresentationDescriptor(&PD);
	if (FAILED(res))
	{
		goto done;
	}

	res = CreatePlaybackTopology(m_pSource, PD, m_hwndVideo, &pTopology);
	if (FAILED(res))
	{
		goto done;
	}

	m_state = OpenPending;

done:
	if (FAILED(res))
	{
		m_state = Closed;
	}
	if (PD)
		PD->Release();
	if (pTopology)
		pTopology->Release();
	return res;

}

HRESULT TPlayer::Play()
{
	//if (m_state != Paused && m_state != Stopped)
	//	return MF_E_INVALIDREQUEST;
	if (!m_pSession || !m_pSource)
		return E_UNEXPECTED;

	return StartPlayback();
}

HRESULT TPlayer::Pause()
{
	if (m_state != Started)
		return MF_E_INVALIDREQUEST;
	if (!m_pSession || !m_pSource)
		return E_UNEXPECTED;

	HRESULT res = m_pSession->Pause();
	if (SUCCEEDED(res))
		m_state = Paused;
	return res;
	
}

HRESULT TPlayer::Stop()
{
	if (m_state != Started)
		return MF_E_INVALIDREQUEST;
	if (!m_pSession || !m_pSource)
		return E_UNEXPECTED;

	HRESULT res = m_pSession->Stop();
	if (SUCCEEDED(res))
		m_state = Stopped;
	return res;
}

HRESULT TPlayer::Shutdown()
{
	HRESULT res = CloseSession();

	MFShutdown();

	if (m_hCloseEvent)
	{
		CloseHandle(m_hCloseEvent);
		m_hCloseEvent = nullptr;
	}
	return res;
}

HRESULT TPlayer::HandleEvent(UINT_PTR pUnkPtr)
{
	HRESULT res = S_OK;
	MediaEventType met = MEUnknown;

	IMFMediaEvent *pEvent = (IMFMediaEvent*)pUnkPtr;
	if (!pEvent)
		return E_POINTER;

	HRESULT res2 = pEvent->GetType(&met);
	if (FAILED(res2))
		goto done;

	res2 = pEvent->GetStatus(&res);

	if (SUCCEEDED(res2) && FAILED(res))
	{
		res2 = res;
	}

	if (FAILED(res2))
		goto done;

	switch (met)
	{
	case MESessionTopologyStatus:
		res = OnTopologyStatus(pEvent);
		break;
	case MEEndOfPresentation:
		res = OnPresentationEnded(pEvent);
		break;
	case MENewPresentation:
		res = OnNewPresentation(pEvent);
		break;
	default:
		res = OnSessionEvent(pEvent, met);
		break;
	}
done:
	if (pEvent)
		pEvent->Release();

	return res;
}

PlayerState TPlayer::GetState()
{
	return m_state;
}

HRESULT TPlayer::SetSpeed(float f)
{
	PROPVARIANT var;
	PropVariantInit(&var);

	IMFRateControl* rater = nullptr;

	HRESULT returnable = MFGetService(m_pSession, MF_RATE_CONTROL_SERVICE, IID_PPV_ARGS(&rater));

	if (FAILED(returnable))
		return returnable;
	Pause();
	returnable = rater->SetRate(FALSE, f);
	if (FAILED(returnable))
		goto done;

	

done:
	if (rater)
		rater->Release();
	Play();
	return returnable;
}

HRESULT TPlayer::Repaint()
{
	if (m_pVideoDisplay)
		return m_pVideoDisplay->RepaintVideo();

	return S_OK;
}

HRESULT TPlayer::ResizeVideo( RECT& r)
{
	if (r.bottom == 0 && r.left == 0 && r.right == 0 && r.top == 0)
		Pause();
	if (m_pVideoDisplay)
	{
		return m_pVideoDisplay->SetVideoPosition(NULL, &r);
	}
	return S_OK;
}

HRESULT TPlayer::OnTopologyStatus(IMFMediaEvent * pEvent)
{
	UINT32 stats;

	HRESULT res = pEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &stats);

	if (SUCCEEDED(res) && stats == MF_TOPOSTATUS_READY)
	{
		if (m_pVideoDisplay)
			m_pVideoDisplay->Release();
		MFGetService(m_pSession, MR_VIDEO_RENDER_SERVICE, IID_PPV_ARGS(&m_pVideoDisplay));
		res = StartPlayback();
	}

	return res;
}

HRESULT TPlayer::OnPresentationEnded(IMFMediaEvent * pEvent)
{
	m_state = Stopped;
	return S_OK;
}

HRESULT TPlayer::OnNewPresentation(IMFMediaEvent * pEvent)
{
	IMFPresentationDescriptor *pPD = NULL;
	IMFTopology *pTopology = NULL;

	// Get the presentation descriptor from the event.
	HRESULT hr = GetEventObject(pEvent, &pPD);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = CreatePlaybackTopology(m_pSource, pPD, m_hwndVideo, &pTopology);
	if (FAILED(hr))
		goto done;

	hr = m_pSession->SetTopology(0, pTopology);
	if (FAILED(hr))
		goto done;
	m_state = OpenPending;
done:
	if (pTopology)
		pTopology->Release();
	if (pPD)
		pPD->Release();
	return S_OK;
}

TPlayer::TPlayer(HWND hVideo, HWND hEvent)
{
	m_pSession = NULL;
	m_pSource = nullptr;
	m_pVideoDisplay = nullptr;
	m_hwndEvent = hEvent;
	m_hwndVideo = hVideo;
	m_state = Closed;
	m_hCloseEvent = nullptr;
	m_nRefCount = 1;
}


TPlayer::~TPlayer()
{
	ASSERT(!m_pSession);
	Shutdown();
}

UCHAR * TPlayer::GetAnaGameType()
{
	return nullptr;
}

HRESULT TPlayer::Initialize()
{
	HRESULT res = MFStartup(MF_VERSION);
	if (SUCCEEDED(res))
	{
		m_hCloseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (!m_hCloseEvent)
			res = HRESULT_FROM_WIN32(GetLastError());
	}
	return res;
	return E_NOTIMPL;
}

HRESULT TPlayer::CreateSession()
{
	HRESULT res = CloseSession();
	if (FAILED(res))
		goto done;

	ASSERT(m_state == Closed);

	res = MFCreateMediaSession(NULL, &m_pSession);
	if (FAILED(res))
		goto done;

	res = m_pSession->BeginGetEvent((IMFAsyncCallback*)this, nullptr);
	if (FAILED(res))
		goto done;
	m_state = Ready;

done:
	return res;
}

HRESULT TPlayer::CloseSession()
{
	HRESULT hr = S_OK;

	if (m_pVideoDisplay)
		m_pVideoDisplay->Release();
	m_pVideoDisplay = nullptr;

	if (m_pSession)
	{
		DWORD resultWait = 0;

		m_state = Closing;
		hr = m_pSession->Close();

		if (SUCCEEDED(hr))
		{
			resultWait = WaitForSingleObject(m_hCloseEvent, 5000);
			if(resultWait == WAIT_TIMEOUT)
				ASSERT (false);
		}
	}

	if (SUCCEEDED(hr))
	{
		if (m_pSource)
			m_pSource->Shutdown();
		if (m_pSession)
			m_pSession->Shutdown();
	}

	if (m_pSource)
		m_pSource->Release();
	if (m_pSession)
		m_pSession->Release();

	return hr;
}

HRESULT TPlayer::StartPlayback()
{
	ASSERT(m_pSession);
	PROPVARIANT varStart;

	PropVariantInit(&varStart);

	HRESULT res = m_pSession->Start(&GUID_NULL, &varStart);
	if (SUCCEEDED(res))
	{
		m_state = Started;

	}
	PropVariantClear(&varStart);

	return res;
}

HRESULT CreateMediaSinkActivate(
	IMFStreamDescriptor *pSourceSD,     // Pointer to the stream descriptor.
	HWND hVideoWindow,                  // Handle to the video clipping window.
	IMFActivate **ppActivate
)
{
	IMFMediaTypeHandler *pHandler = NULL;
	IMFActivate *pActivate = NULL;

	// Get the media type handler for the stream.
	HRESULT hr = pSourceSD->GetMediaTypeHandler(&pHandler);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the major media type.
	GUID guidMajorType;
	hr = pHandler->GetMajorType(&guidMajorType);
	if (FAILED(hr))
	{
		goto done;
	}

	// Create an IMFActivate object for the renderer, based on the media type.
	if (MFMediaType_Audio == guidMajorType)
	{
		// Create the audio renderer.
		hr = MFCreateAudioRendererActivate(&pActivate);
	}
	else if (MFMediaType_Video == guidMajorType)
	{
		// Create the video renderer.
		hr = MFCreateVideoRendererActivate(hVideoWindow, &pActivate);
	}
	else
	{
		// Unknown stream type. 
		hr = E_FAIL;
		// Optionally, you could deselect this stream instead of failing.
	}
	if (FAILED(hr))
	{
		goto done;
	}

	// Return IMFActivate pointer to caller.
	*ppActivate = pActivate;
	(*ppActivate)->AddRef();

done:
	if (pHandler)
		pHandler->Release();
	if (pActivate)
		pActivate->Release();
	return hr;
}