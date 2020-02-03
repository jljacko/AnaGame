#pragma once
#include "TObject.h"
#include "TrecReference.h"

/*
 * class TObjectNode
 * Purpose: Provides an Interface for which Trees of ny type of Object can be formed
 */
class _TREC_LIB_DLL TObjectNode :
	public TObject
{
public:
	TObjectNode(UINT l);
	~TObjectNode();

	virtual TString GetContent() = 0;
	virtual bool IsExtendable() = 0;
	virtual bool IsExtended() = 0;
	virtual bool Initialize() = 0;
	virtual TrecPointer<TObjectNode> GetNodeAt(UINT target, UINT current) =0;
	virtual UINT TotalChildren() = 0;

	virtual bool Initialize(TString& value) = 0;
	virtual void Extend() = 0;
	virtual TrecPointer<TObjectNode> GetChildNodes(UINT) = 0;
	virtual void DropChildNodes() = 0;
	UINT GetLevel();

	void SetParent(TrecPointerSoft<TObjectNode> p);
	void SetSelf(TrecPointer<TObjectNode> s);

protected:
	UINT level;
	TrecPointerSoft<TObjectNode> parentNode;
	TrecPointerSoft<TObjectNode> self;
};

