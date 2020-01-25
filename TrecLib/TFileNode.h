#pragma once
#include "TObjectNode.h"
#include "TFileShell.h"

/*
 * class TFileNode
 * Purpose: Implements the TObjectNode interface for Files found on the local hard drive
 */
class _TREC_LIB_DLL TFileNode :
	public TObjectNode
{
public:
	TFileNode(UINT l);


	TString GetContent() override;
	bool IsExtendable() override;
	virtual bool Initialize() override;
	virtual bool Initialize(TString& value) override;
	virtual void Extend() override;
	virtual TrecPointer<TObjectNode> GetChildNodes(UINT index) override;
	virtual void DropChildNodes() override;

	void SetFile(TrecPointer<TFileShell>& d);
protected:
	TrecPointer<TFileShell> data;
	TDataArray<TrecPointer<TObjectNode>> files;
};

