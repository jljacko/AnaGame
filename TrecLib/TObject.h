#pragma once

#include <new>
#include <memory>
#include "TrecLib.h"

#include "AnagameDef.h"


extern UCHAR TArrayType[];
extern UCHAR TDataArrayType[];
extern UCHAR TMapType[];

class TString;

/*
* Class TObject
* Serves as the base object for the AnaGame platform
*/
class _TREC_LIB_DLL TObject 
{
public:
	TObject();
	virtual ~TObject();

	virtual UCHAR* GetAnaGameType();
	virtual TString getVariableValueStr(TString& varName);
	virtual TString toString();

protected:
	
};






 _TREC_LIB_DLL WCHAR* boolToString(bool);
