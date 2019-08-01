#include "stdafx.h"
#include "BlockTag.h"

BlockTag::BlockTag(BlockType bt) : BNFTag(TString(L"block"))
{
	blockType = bt;
}

BlockTag::~BlockTag()
{
}

TagCheck BlockTag::ProcessTag(TString& code, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	UCHAR closeBlockStack = 1;

	TString readCode;

	WCHAR mode = L'\0';
	WCHAR currentChar;
	UINT codeEnd = codeStart;
	switch (blockType)
	{
	case block_curly:
		file->Seek(codeStart, 0);

		do {
			codeEnd += file->ReadString(readCode, (UINT)100);
			for (UINT Rust = 0; Rust < readCode.GetLength() && closeBlockStack; Rust++)
			{
				currentChar = readCode[Rust];
				if (!mode)
				{
					if (currentChar == L'\'' || currentChar == L'"')
						mode = currentChar;
					else if (currentChar == L'{')
						closeBlockStack++;
					else if (currentChar == L'}')
					{
						closeBlockStack--;
						if (!closeBlockStack)
						{
							codeEnd -= (readCode.GetLength() - Rust);
						}
					}
				}
				else
				{
					if (currentChar == mode)
						mode == L'\0';
				}
			}
		} while (readCode.GetLength() && closeBlockStack);
		

		break;
	}

	TInterpretor* newInterpretor = new TInterpretor(inter);
	newInterpretor->SetGlobalVariables(&globalVariables);
	newInterpretor->SetFile(file, codeStart, 0, codeEnd);

	intVariable var;
	var.hold = 5; // Used for functions
	var.value.object = newInterpretor;

	TagCheck info(true, TString(L""), codeEnd, new intVariable(var));

	return info;
}
