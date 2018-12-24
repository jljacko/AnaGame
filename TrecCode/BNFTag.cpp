#include "stdafx.h"
#include "BNFTag.h"


BNFTag::BNFTag(TString & tagName)
{
	name = tagName;
}

BNFTag::~BNFTag()
{
}

BNFTag * BNFTag::GetTag(TString & tagName)
{
	return new BNFTag(tagName);
}

BNFTag * BNFTag::GetFunctionalTag(TString & tagName)
{
	return nullptr;
}

void BNFTag::SetSyntaxString(TString & s)
{
	rawSyntax = s;
}

UINT BNFTag::CompileTag(TDataArray<BNFTag*>& tagList)
{
	// TO-Do: update call to address "\|", where we don't want to split
	TrecPointer<TArray<TString>> syntax = rawSyntax.split(TString(L"|"));

	for (UINT c = 0; c < syntax->Count(); c++)
	{
		TString curSyntax = syntax->ElementAt(c).get();
		TrecPointer<TArray<TString>> synComps = curSyntax.split(TString(L" "));

		this->syntax.push_back(TDataArray<TagMark>());

		for (UINT rust = 0; rust < synComps->Count(); rust++)
		{
			TString token = synComps->ElementAt(rust).get();

			if (token.GetLength() < 2)
				continue;

			if (token[0] == L'\'' && token[token.GetLength() - 1] == L'\'')
			{
				token = token.SubString(1, token.GetLength() - 1);

				this->syntax[c].push_back(TagMark{ false, token, -1 });
			}
			else if (token[0] == L'<' && token[token.GetLength() - 1] == L'>')
			{
				token = token.SubString(1, token.GetLength() - 1);

				this->syntax[c].push_back(TagMark{ true , token, findTagIndex(tagList, token) });
			}
			else if (token[0] == L'[' && token[token.GetLength() - 1] == L']'
				&& token[1] == L'[' && token[token.GetLength() - 2] == L']')
			{
				token = token.SubString(2, token.GetLength() - 2);

				this->syntax[c].push_back(TagMark{ true , token, findTagIndex(tagList, token) });
			}
		}
	}
	return 0;
}

TagCheck BNFTag::ProcessTag(UINT statementStart, UINT tagStart, TFile & file, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags, UINT end)
{
	TString code;
	file.Seek(tagStart, CFile::begin);

	UINT length = 100;
	if (end > tagStart && end - tagStart < length)
		length = end - tagStart;

	file.ReadString(code, length);



	return TagCheck();
}

TagCheck BNFTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags)
{
	return TagCheck();
}

void BNFTag::addAttribute(TString & att, TString & val)
{
}

short BNFTag::findTagIndex(TDataArray<BNFTag*>& list, TString & token)
{
	for (UINT c = 0; c < list.Size() && c < SHORT_MAX; c++)
	{
		if (list[c]->name == token)
			return c;
	}
	return -1;
}

UINT BNFTag::CountRawTags(TDataArray<TagMark>& tags)
{
	UINT ret = 0;

	for (UINT rust = 0; rust < tags.Size(); rust++)
	{
		if (!tags[rust].isTag)
			ret++;
	}

	return ret;
}

int BNFTag::GetIndexOfToken(TString & code, TDataArray<TagMark>& tags, UINT token, UINT start)
{
	UINT targetTag = 0;

	for (UINT c = 0; c < tags.Size() && token; c++)
	{
		if (!tags[c].isTag)
		{
			token--;
			targetTag = c;
		}
	}



	return code.Find(tags[targetTag].mark, start);
}

ProcessedCode BNFTag::PreProcessLine(TString & code, UINT syntaxIndex)
{
	return ProcessedCode();
}

TDataArray<BNFTag*>* setUpTagList(TFile & file)
{
	TDataArray<BNFTag*>* ret = new TDataArray<BNFTag*>();

	TString line;

	while (file.ReadString(line))
	{
		int split = line.Find(L"::=");
		int comment = line.Find(L"#");

		if (split < 0 || split > comment)
			continue;

		while (comment != -1 && comment > 0 && line[comment - 1] == L'\\')
			comment = line.Find(L"#", comment + 1);

		TString tag = line.SubString(0, split);
		tag.Trim();

		TString syn = line.SubString(split + 3, comment);

		if (tag.GetLength() < 2)
			continue;
		BNFTag* bTag = nullptr;
		if (tag[0] == L'<' && tag[tag.GetLength() - 1] == L'>')
			bTag = BNFTag::GetTag(tag.SubString(1, tag.GetLength() - 1));

		if (tag[0] == L'[' && tag[tag.GetLength() - 1] == L']'
			&& tag[1] == L'[' && tag[tag.GetLength() - 2] == L']')
			bTag = BNFTag::GetFunctionalTag(tag.SubString(2, tag.GetLength() - 2));

		if (bTag)
		{
			bTag->SetSyntaxString(syn);
			ret->push_back(bTag);
		}
	}

	return ret;
}

UINT CompileIntLanguage(TDataArray<BNFTag*>& tags)
{
	UINT ret = 0;
	for (UINT rust = 0; rust < tags.Size(); rust++)
	{
		ret += tags[rust]->CompileTag(tags);
	}
	return ret;
}
