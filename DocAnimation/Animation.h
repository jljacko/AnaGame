#pragma once
#include <TObject.h>
class Animation :
	public TObject
{
public:
	Animation();
	~Animation();

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	virtual void AddAttribute(TString& key, TString& value) = 0;
};

