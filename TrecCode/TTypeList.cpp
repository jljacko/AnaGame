#include "pch.h"
#include "TTypeList.h"

TTypeList::TTypeList()
{
}

TrecPointer<TType> TTypeList::GetTypeByAlias(TString& name)
{
	TrecPointer<UINT> index = nameAlias.retrieveEntry(name);
	if (index.Get() && *index.Get() < typeList.Size())
		return typeList.at(*index.Get());
	return TrecPointer<TType>();
}

TrecPointer<TType> TTypeList::GetTypeByName(TString& name)
{
	for (UINT Rust = 0; Rust < typeList.Size(); Rust++)
	{
		if (typeList[Rust].Get() && typeList[Rust]->GetName() == name)
			return typeList[Rust];
	}
	return TrecPointer<TType>();
}

void TTypeList::AddType(TrecPointer<TType> type)
{
	if (!type.Get())
		return;
	TString name = type->GetName();
	UINT index = typeList.push_back(type) - 1;
	nameAlias.addEntry(name, TrecPointerKey::GetNewTrecPointer<UINT>(index));
}

bool TTypeList::SetAlias(TString& name, TString& alias)
{
	for (UINT Rust = 0; Rust < typeList.Size(); Rust++)
	{
		if (typeList[Rust].Get() && typeList[Rust]->GetName() == name)
		{
			if (!nameAlias.retrieveEntry(alias).Get())
			{
				nameAlias.addEntry(alias, TrecPointerKey::GetNewTrecPointer<UINT>(Rust));
				return true;
			}
			return false;
		}
	}
	return false;
}

bool TTypeList::SetAlias(TrecPointer<TType> type, TString& alias)
{
	for (UINT Rust = 0; Rust < typeList.Size(); Rust++)
	{
		if (typeList[Rust].Get() && typeList[Rust].Get() == type.Get())
		{
			if (!nameAlias.retrieveEntry(alias).Get())
			{
				nameAlias.addEntry(alias, TrecPointerKey::GetNewTrecPointer<UINT>(Rust));
				return true;
			}
			return false;
		}
	}
	return false;
}
