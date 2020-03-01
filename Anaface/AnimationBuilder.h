#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include <Animation.h>
#include <TMap.h>
#include "Anaface.h"
class _ANAFACE_DLL AnimationBuilder : public TObject
{
public:
	AnimationBuilder();
	void SetName(const TString& name);
	void SetType(const TString& type);
	void SetAttribute(const TString& key, TrecPointer<TString> value);

	TString GetName();

	TrecPointer<Animation> Build();
protected:
	TString name, type;
	TMap<TString> attributes;
	animation_phase phase;
};

