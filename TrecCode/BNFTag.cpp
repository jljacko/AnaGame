#include "stdafx.h"
#include "BNFTag.h"
#include "TagHeaders.h"
#include "IntLanguage.h"

BNFTag::BNFTag(const TString & tagName)
{
	name.Set(tagName);
}

BNFTag::~BNFTag()
{
}

BNFTag * BNFTag::GetTag(TString & tagName)
{
	return new BNFTag(tagName);
}

BNFTag * BNFTag::GetFunctionalTag(TString & tagName, BlockType bt)
{
	// Basic Conditional
	if (tagName == TString(L"if_statement"))
		return new ConditionalBlockTag(tagName, bt, false, false, false); 

	// Basic Loops
	if (tagName == TString(L"while_statement"))
		return  new ConditionalBlockTag(tagName, bt, true, false, false); 
	if (tagName == TString(L"until_statement"))
		return  new ConditionalBlockTag(tagName, bt, true, false, false, true); // Included to support the Perl Language (which has n Until block)
	if (tagName == TString(L"for_1_statement"))
		return  new ConditionalBlockTag(tagName, bt, true, false, false);
	if (tagName == TString(L"for_3_statement"))
		return  new ConditionalBlockTag(tagName, bt, true, true, false);

	// Error Handlers
	if (tagName == TString(L"try_statement"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"catch"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"finally"))
		return nullptr; // To-Do: Create the If bnf tag

	// Function functionality
	if (tagName == TString(L"function_statement"))
		return new ProcedureBlockTag(tagName, bt, false, false, false);
	if (tagName == TString(L"parameters"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"function_expr"))
		return new ProcedureBlockTag(tagName, bt, false, false, true);
	if (tagName == TString(L"block"))
		return new BlockTag(bt);
	
	// Flow Tags
	if (tagName == TString(L"break"))
		return new BreakTag();
	if (tagName == TString(L"continue"))
		return new ContinueTag();
	if (tagName == TString(L"return"))
		return nullptr; // To-Do: Create the If bnf tag

	// Variables
	if (tagName == TString(L"class_statement"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"var"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"declaration"))
		return nullptr; // To-Do: Create the If bnf tag

	// Gate-wise expressions
	if (tagName == TString(L"or_exp"))
		return new OrExpression(); // To-Do: Create the If bnf tag
	if (tagName == TString(L"and_exp"))
		return new AndExpression(); // To-Do: Create the If bnf tag
	if (tagName == TString(L"not_test"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"xor_expr"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"or_expr"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"and_expr"))
		return nullptr; // To-Do: Create the If bnf tag

	// Shifts
	if (tagName == TString(L"left_shift"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"right_shift"))
		return nullptr; // To-Do: Create the If bnf tag

	// Arithmetic expressions
	if (tagName == TString(L"subtract_exp"))
		return new SubtractExpressionTag();
	if (tagName == TString(L"add_exp"))
		return new AddExpressionTag();
	if (tagName == TString(L"mul_exp"))
		return new MultipExpressionTag(); 
	if (tagName == TString(L"div_exp"))
		return new DivExpressionTag(); 
	if (tagName == TString(L"mod_exp"))
		return new ModExpressionTag(); 
	if (tagName == TString(L"float_div_exp"))
		return new F_DivExpressionTag(); 
	if (tagName == TString(L"power"))
		return new PowerTag(); 

	// Methods
	if (tagName == TString(L"method_call"))
		return nullptr; // To-Do: Create the If bnf tag
	if (tagName == TString(L"method_attr"))
		return nullptr; // To-Do: Create the If bnf tag

	// Basics
	if (tagName == TString(L"null"))
		return new NullTag(); 
	if (tagName == TString(L"true"))
		return new TrueTag(); 
	if (tagName == TString(L"false"))
		return new FalseTag();
	if (tagName == TString(L"array"))
		return nullptr; // To-Do: Create the array bnf tag

	return nullptr;
}

void BNFTag::SetSyntaxString(TString & s)
{
	rawSyntax.Set(s);
}

UINT BNFTag::CompileTag(TDataArray<BNFTag*>& tagList)
{
	// TO-Do: update call to address "\|", where we don't want to split
	auto syntax = rawSyntax.split(TString(L"|"), true);

	for (UINT c = 0; c < syntax->Size(); c++)
	{
		TString curSyntax = syntax->at(c);
		TrecPointer<TDataArray<TString>> synComps = curSyntax.split(TString(L" "));

		this->syntax.push_back(TDataArray<TagMark>());

		for (UINT rust = 0; rust < synComps->Size(); rust++)
		{
			TString token = synComps->at(rust);
			token.Trim();

			if (token.GetSize() < 2)
				continue;

			if (token[0] == L'\'' && token[token.GetSize() - 1] == L'\'')
			{
				token.Set(token.SubString(1, token.GetSize() - 1));

				this->syntax[c].push_back(TagMark( false, token, -1 ));
			}
			else if (token[0] == L'<' && token[token.GetSize() - 1] == L'>')
			{
				token.Set(token.SubString(1, token.GetSize() - 1));

				this->syntax[c].push_back(TagMark{ true , token, findTagIndex(tagList, token) });
			}
			else if (token[0] == L'[' && token[token.GetSize() - 1] == L']'
				&& token[1] == L'[' && token[token.GetSize() - 2] == L']')
			{
				token.Set(token.SubString(2, token.GetSize() - 2));

				this->syntax[c].push_back(TagMark{ true , token, findTagIndex(tagList, token) });
			}
		}
		if (!this->syntax[c].Size()) continue;

		// Now to make sure we don't have two tag marks in a row. If we do, this is an invalid syntax as
		// TrecCode won't know where to draw the boundary in a peice of code between the two tags
		bool isTag = this->syntax[c][0].isTag;
		for (UINT rust = 1; rust < this->syntax[c].Size(); rust++)
		{
			if (isTag && this->syntax[c][rust].isTag)
				return 1; // We are dealing with two tag marks in a row. Illegal as there is no way to know how to draw the boundary
			isTag = this->syntax[c][rust].isTag;
		}
	}
	TString bnfLang(L"<<<");
	bnfLang.Append(this->name);
	bnfLang.Append(L">>>");
	Log(lt_bnf, bnfLang);
	TString code(L"\t");
	for (UINT C = 0; C < this->syntax.Size(); C++)
	{
		

		for (UINT Rust = 0; Rust < this->syntax[C].Size(); Rust++)
		{
			if (!this->syntax[C][Rust].isTag)
			{
				auto value = this->syntax[C][Rust].mark;
				code.Append(TString(L"<<<") + value + TString(L">>> "));
			} 
			else
			{
				auto value = this->syntax[C][Rust].tagIndex;
				code.AppendFormat(_T("[%d] "), value);
			}
		}
		code.Append(L" |  ");
		
	}
	Log(lt_bnf, code);


	return 0;
}

TagCheck BNFTag::ProcessTag(UINT statementStart, UINT tagStart, TrecPointer<TFile> file, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags, IntLanguage& lang, UINT end)
{
	TString code;
	file->Seek(tagStart, CFile::begin);

	UINT length = 100;
	if (end > tagStart && end - tagStart < length)
		length = end - tagStart;

	file->ReadString(code, length);



	return TagCheck();
}

TagCheck BNFTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	return TagCheck();
}

TagCheck BNFTag::ProcessTag(TString & code, UINT codeStart, TrecPointer<TFile> file, VariableContainer & globalVariables, TInterpretor & inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{

	TagCheck returnable;
	for (UINT C = 0; C < syntax.Size(); C++)
	{
		returnable = ProcessTag(code, codeStart, file, globalVariables, inter, lang, tags, C, 0);
		if (returnable.success)
			return returnable;
	}
	return returnable;
}

void BNFTag::addAttribute(TString & att, TString & val)
{
}

TString BNFTag::GetTageName()
{
	return name;
}

int BNFTag::GetIndexOfTag(TString& name, TDataArray<BNFTag*>& tags)
{
	for (int c = 0; c < tags.Size(); c++)
	{
		if (name == tags[c]->name)
			return c;
	}
	return -1;
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

TagCheck BNFTag::ProcessTag(TString& code, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables,
	TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UINT syntaxLevel, UINT syntaxStart)
{
	TagCheck returnable;
	returnable.returnValue = nullptr;
	returnable.success = false;
	auto syntaxSample = syntax[syntaxLevel];
	if (!syntaxSample.Size())
	{
		returnable.returnValue = nullptr;
		returnable.success = false;
		return returnable;
	}
	bool startWithTag = syntaxSample[syntaxStart].isTag;
	if (startWithTag)
	{
		TString code2;
		int token = -1;
		if (syntaxSample.Size() - syntaxStart == 1)
		{
			code2.Set(code);
		}
		else
		{
			token = code.FindOutOfQuotes(syntaxSample[syntaxStart + 1].mark);
			if (token == -1)
			{
				return returnable;
			}
			code2.Set(code.SubString(0, token));
		}

		BNFTag* tag = lang.getTagAt(syntaxSample[syntaxStart].tagIndex);
		if (!tag)
		{
			TString error;
			error.Format(L"Error Processing Code for '%S'\n\tCode <<<%S>>>\n\tEncountered NULL tag!", lang.getLanguageName(), code);
			returnable.error.Set(error);
			returnable.success = false;
			returnable.returnValue = nullptr;
			returnable.fileByteEnd = codeStart + code.GetSize();
			return returnable;
		}
		if (tag == this)
		{
			returnable.error.Format(L"Error Processing Code for '%S'\n\tEncountered Endless recursive tag!", lang.getLanguageName());
			returnable.success = false;
			returnable.returnValue = nullptr;
			returnable.fileByteEnd = codeStart + code.GetSize();
			return returnable;
		}

		returnable = tag->ProcessTag(code, codeStart, file, globalVariables, inter, lang, tags);

		/* Go ahead and return now if:
		 * 1. The tag was not successful
		 * 2. It was successful, but there there was only that tage to process
		 * 3. 
		 */
		if (!returnable.success || token == -1 || syntaxSample.Size() == syntaxStart + 2)
		{
			return returnable;
		}

		UINT appendChar = token + syntaxSample[syntaxStart + 1].mark.GetSize();
		code2.Set(code.SubString(appendChar));

		returnable = ProcessTag(code2, codeStart + appendChar, file, globalVariables, inter, lang, tags, syntaxLevel, syntaxStart + 2);
		if (returnable.success)
		{
			auto logMessage = name + TString(L" tag");
			Log(lt_code, TString(logMessage));
		}
		return returnable;
	}
	else
	{
		int tokenIndex = code.FindOutOfQuotes(syntaxSample[syntaxStart].mark);
		if (tokenIndex == -1)
			return returnable;
		if (tokenIndex > 0)
		{
			TString pre = code.SubString(0, tokenIndex);
			pre.Trim();
			if (!pre.IsEmpty())
				return returnable;
		}

		UINT appendChar = tokenIndex + syntaxSample[syntaxStart].mark.GetSize();
		TString code2 = code.SubString(appendChar);
		code2.Trim();

		if (code2.IsEmpty())
		{
			if(syntaxStart + 1 == syntaxSample.Size())
				returnable.success = true;
			return returnable;
		}
		if (syntaxStart + 1 == syntaxSample.Size())
			return returnable;
		
		returnable = ProcessTag(code2, codeStart + appendChar, file, globalVariables, inter, lang, tags, syntaxLevel, syntaxStart + 1);
		if (returnable.success)
		{
			auto logMessage = name + TString(L" tag");
			Log(lt_code, TString(logMessage));
		}
		return returnable;
	}
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

TDataArray<BNFTag*>* setUpTagList(TFile & file, BlockType bt)
{
	TDataArray<BNFTag*>* ret = new TDataArray<BNFTag*>();

	TString line;



	while (file.ReadString(line))
	{
		int split = line.Find(L"::=");
		int comment = line.Find(L"#");

		if (split < 0 || (split > comment && comment != -1))
			continue;

		while (comment != -1 && comment > 0 && line[comment - 1] == L'\\')
			comment = line.Find(L"#", comment + 1);

		TString tag = line.SubString(0, split);
		tag.Trim();

		TString syn = line.SubString(split + 3, comment);

		if (tag.GetSize() < 2)
			continue;
		BNFTag* bTag = nullptr;

		if (tag[0] == L'<' && tag[tag.GetSize() - 1] == L'>')
			bTag = BNFTag::GetTag(tag.SubString(1, tag.GetSize() - 1));

		if (tag[0] == L'[' && tag[tag.GetSize() - 1] == L']'
			&& tag[1] == L'[' && tag[tag.GetSize() - 2] == L']')
			bTag = BNFTag::GetFunctionalTag(tag.SubString(2, tag.GetSize() - 2), bt);

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

bool getVariablesBooleanValue(intVariable& var)
{
	switch (var.hold)
	{
	case 6: // These are objects
	case 5:
	case 1:
		return static_cast<bool>(var.value.object.get());
	case 3:
		return static_cast<bool>(var.value.primFloat);
	default:
		return static_cast<bool>(var.value.primInt);
	}

}

TagCheck::TagCheck()
{
	success = false;
	fileByteEnd = 0;
	returnValue = nullptr;
}

TagCheck::TagCheck(bool s, TString & e, UINT end, intVariable * iv)
{
	success = s;
	error.Set(e);
	fileByteEnd = end;
	returnValue = iv;
}

TagMark::TagMark()
{
	isTag = false;
	tagIndex = 0;
}

TagMark::TagMark(bool it, TString & m, short ti)
{
	isTag = it;
	mark.Set(m);
	tagIndex = ti;
}
