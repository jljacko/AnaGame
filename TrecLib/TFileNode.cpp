#include "TFileNode.h"
#include "TDirectory.h"

/*
* Method: (TFileNode) (Constructor)
* Purpose: Constructs the TFileNode
* Parameters: UINT l - level of the node (used by the TObjectNode class)
* Returns: void
*/
TFileNode::TFileNode(UINT l) : TObjectNode(l)
{
}

/*
* Method: TFileNode - GetContent
* Purpose: Retrieves the Content to present about this object
* Parameters: void 
* Returns: TString - the name of the File/directory this node references
*/
TString TFileNode::GetContent()
{
	if (!data.Get())
		return TString();

	return data->GetName();
}

/*
* Method: TFileNode - IsExtendable
* Purpose: Reports whether this node is extendable - in this case, whether the file is a directory or not
* Parameters: void
* Returns: bool - whether the node is extendable (i.e. references a directory)
*/
bool TFileNode::IsExtendable()
{
	if (!data.Get())
		return false;
	return data->IsDirectory();
}

bool TFileNode::IsExtended()
{
	return files.Size();
}

TrecPointer<TObjectNode> TFileNode::GetNodeAt(UINT target, UINT current)
{
	if (target == current)
		return TrecPointerKey::GetTrecPointerFromSoft<TObjectNode>(self);

	if(target < current)
		return TrecPointer<TObjectNode>();

	UINT diff = target - current++;

	for (UINT rust = 0; rust < files.Size(); rust++)
	{
		if (files[rust]->TotalChildren() < diff)
		{
			diff -= files[rust]->TotalChildren();
			current += files[rust]->TotalChildren();
		}
		else return files[rust]->GetNodeAt(target, current);
	}
	return TrecPointer<TObjectNode>();
}

UINT TFileNode::TotalChildren()
{
	UINT ret = 0;
	for (UINT rust = 0; rust < files.Size(); rust++)
	{
		ret += files[rust]->TotalChildren() + 1;
	}
	return ret;
}

/*
* Method: TFileNode - Initialize
* Purpose: Supposed to initialize the node tree
* Parameters: void
* Returns: bool - false as the Node cannot be initialized without the path provided
*/
bool TFileNode::Initialize()
{
	return false;
}

/*
* Method: TFileNode - Initialize
* Purpose: Initializes the node with the path of the file/Directory to start with
* Parameters: TStrng& value - the path of the File to reference
* Returns: bool - true of a file object was generated, false otherwise
*/
bool TFileNode::Initialize(TString& value)
{
	data = TFileShell::GetFileInfo(value);
	return data.Get();
}

/*
* Method: TFileNode - Extend
* Purpose: Epands the Tree by getting all of the files in the directory
* Parameters: void
* Returns: void
*
* Note: This method only functions of the file in question is a directory, this can be determined via the IsExtendable method
*/
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
				TrecPointer<TObjectNode> n = TrecPointerKey::GetNewSelfTrecPointerAlt<TObjectNode, TFileNode>(level + 1);
				files.push_back(n);
				dynamic_cast<TFileNode*>(n.Get())->SetFile(f);
			}
		}
	}
}

/*
* Method: TFileNode - GetChildNodes
* Purpose: Retrieves Child Nodes one at a time via an index.
* Parameters: UINT index - the index of the child node to retrieve
* Returns: TrecPointer<TObjectNode> - the node in question
*
* Note: Pointer returned is null if the index is too high. To browse the tree, start the index at 0 and once the result is null, you can be sure there are no more nodes
*/
TrecPointer<TObjectNode> TFileNode::GetChildNodes(UINT index)
{
	if (index < files.Size())
		return files[index];
	return TrecPointer<TObjectNode>();
}

/*
* Method: TFileNode - DropChildNodes
* Purpose: Drops the child nodes, provided with a control in mind
* Parameters: void
* Returns: void
*/
void TFileNode::DropChildNodes()
{
	files.RemoveAll();
}

/*
* Method: TFileNode - SetFile
* Purpose: Manually Initialize a node with an existing File Record
* Parameters: TrecPointer<TFileShell>& d - the file to reference
* Returns: void
*/
void TFileNode::SetFile(TrecPointer<TFileShell>& d)
{
	data = d;
	DropChildNodes();
}