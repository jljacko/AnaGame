#include "TFileNode.h"
#include "TDirectory.h"

TFileNode::TFileNode(UINT l) : TObjectNode(l)
{
}

TString TFileNode::GetContent()
{
	if (!data.Get())
		return TString();

	return data->GetName();
}

bool TFileNode::IsExtendable()
{
	if (!data.Get())
		return false;
	return data->IsDirectory();
}

bool TFileNode::Initialize()
{
	return false;
}

bool TFileNode::Initialize(TString& value)
{
	data = TFileShell::GetFileInfo(value);
	return data.Get();
}

void TFileNode::Extend()
{
	files.RemoveAll();
	if (data.Get() && data->IsDirectory())
	{
		TDataArray<TrecPointer<TFileShell>> shellFiles;
		dynamic_cast<TDirectory*>(data.Get())->GetFileListing(shellFiles);

		for (UINT Rust = 0; Rust < shellFiles.Size(); Rust++)
		{
			TrecPointer<TFileShell> f = shellFiles[Rust];
			if (f.Get())
			{
				TrecPointer<TObjectNode> n = TrecPointerKey::GetNewTrecPointerAlt<TObjectNode, TFileNode>(level + 1);
				files.push_back(n);
				dynamic_cast<TFileNode*>(n.Get())->SetFile(f);
			}
		}
	}
}

TrecPointer<TObjectNode> TFileNode::GetChildNodes(UINT index)
{
	if (index < files.Size())
		return files[index];
	return TrecPointer<TObjectNode>();
}

void TFileNode::DropChildNodes()
{
	files.RemoveAll();
}

void TFileNode::SetFile(TrecPointer<TFileShell>& d)
{
	data = d;
}