#pragma once
#include "stdafx.h"
#include <TString.h>
#include <TMap.h>
// TSocket command target
#define SOCKET_RESULT int

class TSocket : public CSocket
{
public:
	TSocket();
	virtual ~TSocket();

	bool setRequestHeader(TString, TString, TString);

	SOCKET_RESULT sendGet(TString);
	SOCKET_RESULT sendPost(TString, TString);
private:
	bool requestHeaderSet = false;
	TString user_agent,accept_lang,accept_encode; // Should remain the same based off of browser used
	TString referer, host;                        // Expected to be updated
};

bool SOCKET_SUCCESS(SOCKET_RESULT);
