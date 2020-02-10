#pragma once
#include <TObject.h>
#include <TDataArray.h>
#include <TType.h>
#include <TMap.h>

class TTypeList :
	public TObject
{
public:
	TTypeList();

	TrecPointer<TType> GetTypeByAlias(TString& name);
	TrecPointer<TType> GetTypeByName(TString& name);

	void AddType(TrecPointer<TType>);
	bool SetAlias(TString& name, TString& alias);
	bool SetAlias(TrecPointer<TType> type, TString& alias);

protected:
	TDataArray<TrecPointer<TType>> typeList;
	TMap<UINT> nameAlias;
};

