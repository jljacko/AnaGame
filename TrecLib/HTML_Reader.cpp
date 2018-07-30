#include "stdafx.h"
#include "HTML_Reader.h"

UCHAR HTML_ReaderType[] = { 3, 0b10000000, 3, 1 };

/*
* Method: (HTML_Reader) (Constructor)
* Purpose: Sets up the parser to use
* Parameters: CArchive* car - the CArchive to use
*			Parser_* par - the parser to call upon
* Returns: void
* Note: Could be depreciated - CArchive is unique to MFC and CArchive could be replaced with TFile
*/
HTML_Reader::HTML_Reader(CArchive* ca, Parser_* p):ParseReader_(ca,p)
{
	charDeduced = false;
	openTaken = false;
	contentMode = false;
	contentSpace = false;
	contentS = makeTrecPointer<TString>();
}

HTML_Reader::HTML_Reader(TFile * ta, Parser_ *p):ParseReader_(ta,p)
{
}

/*
* Method: (HTML_Reader) (Destructor)
* Purpose: Cleans up the parser
* Parameters: void
* Returns: void
*/
HTML_Reader::~HTML_Reader()
{
}

/*
* Method: HTML_Reader - read
* Purpose: Reads the file anticipating the HTML format
* Parameters: int* - the line number an error occurs
* Returns: bool - success
*/
bool HTML_Reader::read(int * i)
{
	if(!materials)
		return false;

	unsigned char char2[2];
	reader->Read(char2, 2);
	DeduceCharType(char2);

	if (!charDeduced)
	{
		i = 0;
		return false;
	}

	if (usingWide)
	{
		WCHAR w;
		memcpy(&w, char2, 2);
		TString t;
		if (!isWhiteSpace(w))
			t += w;
		//*i = 0;
		int ri = 0;
		while (reader->Read(char2, 2) == 2)
		{
			memcpy(&w, char2, 2);
			if (!isWhiteSpace(w))
				t += w;
			else
			{
				ri++;
				*i = ri;
				if (!DeduceToken(t))
					return false;
				if (openTaken)
					t = L'<';
				openTaken = false;
			}
		}
	}
	else
	{
		char charFir = char2[0];
		char charSec = char2[1];
		WCHAR w1 = ReturnWCharType(charFir);
		WCHAR w2 = ReturnWCharType(charSec);
		TString t;

		if (!isWhiteSpace(w1))
			t += w1;
		if (!isWhiteSpace(w2))
			t += w2;
		int ri = 0;
		while (reader->Read(&charFir, 1) == 1)
		{
			w1 = ReturnWCharType(charFir);

			if (contentMode)
			{
				if (w1 == L'<')
				{
					// Time to end ContentMode
					endContentMode();
					t = w1;
				}
				else
				{
					//if (!isWhiteSpace(w1))
					//	contentSpace = true;
					//if (contentSpace)
						*contentS.get() += w1;

				}
			}
			else
			{
				t += w1;
				if (w1 == L'>')
				{

					DeduceToken(t);
					contentMode = true;
				}

				
			}

			/**/
		}

	}
	return true;
}

/*
* Method: HTML_Reader - GetAnaGameType
* Purpose: Retieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type ID format
*/
UCHAR * HTML_Reader::GetAnaGameType()
{
	return HTML_ReaderType;
}

/*
* Method: HTML_Reader - DeduceCharType
* Purpose: Uses the first character to decude whether it is reading Unicode or ACSII
* Parameters: unsigned char* bytes - the bytes to analyze
* Returns: void
*/
void HTML_Reader::DeduceCharType(unsigned char * bytes)
{
	if (!bytes)
		return;
	WCHAR w_char;
	memcpy(&w_char, bytes, 2);
	char s_char = static_cast<char>(*bytes);

	if (w_char == L'<' || isWhiteSpace(w_char))
	{
		charDeduced = true;
		usingWide = true;
	}
	else if (s_char == ' ' || s_char == '<' || s_char == '\n' || s_char == '\t')
	{
		charDeduced = true;
		usingWide = false;
	}
	else
	{
		charDeduced = false;
	}
}

/*
* Method: HTML_Reader - ReturnWCharType
* Purpose: Converts a char to a WCHAR
* Parameters: char c - the char to convert
* Returns: WCHAR - the wide char version of c
* Note: Functionality is generic enough that method should be moved to a function
*/
WCHAR HTML_Reader::ReturnWCharType(char c)
{
	WCHAR w[] = { L'0',L'\0' };
	size_t conv = 0;
	char charTo[] = { c, '\0' };
	mbstowcs_s(&conv, w, 2, charTo, 1);
	return w[0];
}

/*
* Method: HTML_Reader - isWhiteSpace
* Purpose: Determines whether a character is white space or not
* Parameters: WCHAR c - the character to analyze
* Returns: bool - whether wchar is whitespace
*/
bool HTML_Reader::isWhiteSpace(WCHAR c)
{
	return c == L'\r' || c == L' ' || c == L'\n' || c == L'\t';
}

/*
* Method: HTML_Reader - DeduceToken
* Purpose: Attempts to determine what type of HTML tokens it was given
* Parameters: TString& t - the string detected
* Returns: bool - whether or not the string was processed
*/
bool HTML_Reader::DeduceToken(TString & t)
{
	if (!t.GetLength())
		return true;

	TrecPointer<TArray<TString>> tokens = t.split(L"<>");

	if (!tokens.get())
		return false;

	// We expect the count to at least be 1
	if (tokens->Count())
	{
		TrecPointer<TArray<TString>> tokens2 = tokens->ElementAt(0)->split(L"\"");
		if (tokens2.get() && tokens2->Count())
		{
			if (!parseQuoteTokens(tokens2))
				return false;
		}
	}

	/*if (t[0] == L'<')
	{
		bool contentNeeded = false;
		TString content;
		int endToken = t.Find(L'>');
		if (endToken == t.GetLength() - 1 && t.Find(L'/') == t.GetLength() - 2)
		{
			if (t == TString(L"<b/>") || t == TString(L"<i/>") || t == TString(L"<p/>"))
				respond->Obj(&t);
			else
			{
				respond->goParent();
			}
			contentMode = true;
		}
		else if (endToken == t.GetLength() - 1)
		{
			contentMode = true;
			/*contentNeeded = true;
			content = t.SubString(endToken);

			if (usingWide)
			{
				WCHAR w_char = L'\0';
				while (reader->Read(&w_char, 2) == 2 && (w_char != L'<' || w_char != L'>'))
				{
					content += w_char;
				}
				if (w_char == L'<')
					openTaken = true;
			}//* /
		}
		else
			respond->goChild();
		respond->Obj(&t);


	}
	else if (t.Find(L">") == t.GetLength() - 1)
	{
		contentMode = true;
	}
	else if (t.Find(L'=') != -1)
	{
		TrecPointer<TArray<TString>> peices = t.split(TString(L"="));
		if (peices->Count() == 2)
		{
			respond->Attribute(peices->ElementAt(1), peices->ElementAt(0)->GetBuffer());
			peices->ElementAt(0)->GetBuffer();
		}
	}
	*/
	t = L"";
	return true;
}

/*
* Method: HTML_Reader - GetArchive
* Purpose: Retrieves the archive used for the HTML file
* Parameters: void
* Returns: CArchive* - the archive being used
*/
CArchive * HTML_Reader::GetArchive()
{
	return reader;
}

/*
* Method: HTML_Reader - SubmitToken
* Purpose: Deduces the token
* Parameters: TString t - the string to look into
* Returns: void
*/
void HTML_Reader::SubmitToken(TString t)
{
	DeduceToken(t);
}

/*
* Method: HTML_Reader - parseQuoteTokens
* Purpose: Manages tokens placed within Quotation marks
* Parameters: TrecPointer<TArray<TString>>& tokens - the tokens to parse
* Returns: bool - success result
*/
bool HTML_Reader::parseQuoteTokens(TrecPointer<TArray<TString>>& tokens)
{
	if(!tokens.get() || !tokens->Count())
		return false;

	TrecPointer<TArray<TString>> tokens2 = tokens->ElementAt(0)->split(L" \n\t\r\s");
	respond->Obj(tokens2->ElementAt(0).get());
	




	if (tokens2->Count() > 1)
	{
		if (tokens->Count() > 1)
		{
			tokens2->ElementAt(tokens2->Count() - 1)->Append(*tokens->ElementAt(1).get());
			TrecPointer<TArray<TString>> tokens3 = tokens2->ElementAt(1)->split(L"=");
			if (tokens3->Count() > 1 && tokens3->ElementAt(0).get())
			{
				respond->Attribute(tokens3->ElementAt(1), *tokens3->ElementAt(0).get());
				//tokens3->ElementAt(1)->ReleaseBuffer();
			}

			// To-Do: Implement support for quotation marks
			for (UINT c = 3; c < tokens->Count(); c += 2)
			{
				tokens2 = tokens->ElementAt(c-1)->split(L" \n\t\r\s");
				tokens2->ElementAt(tokens2->Count() - 1)->Append(*tokens->ElementAt(c).get());
				tokens3 = tokens2->ElementAt(0)->split(L"=");
				if (tokens3->Count() > 1 && tokens3->ElementAt(0).get())
				{
					respond->Attribute(tokens3->ElementAt(1), *tokens3->ElementAt(0).get());
					//tokens3->ElementAt(1)->ReleaseBuffer();
				}
				else if (tokens3->Count() == 1)
				{
					respond->Obj(tokens3->ElementAt(0).get());
				}
			}
		}
		else
		{
			for (UINT c = 1; c < tokens2->Count(); c++)
			{
				TrecPointer<TArray<TString>> tokens3 = tokens2->ElementAt(c)->split(L"=");
				if (tokens3->Count() > 1 && tokens3->ElementAt(1).get())
				{
					respond->Attribute(tokens3->ElementAt(0), *tokens3->ElementAt(1).get());
					//tokens3->ElementAt(1)->ReleaseBuffer();
				}
				else if (tokens3->Count() == 1)
				{
					respond->Obj(tokens3->ElementAt(0).get());
				}
			}
		}
	}

	return true;
}

/*
* Method: HTML_Reader - endContentMode
* Purpose: Signifies that the content between HTML tages is over
* Parameters: void
* Returns: void
*/
void HTML_Reader::endContentMode()
{
	respond->Attribute(contentS, TString(L"caption"));
	contentMode = false;
	contentSpace = false;
	contentS->Empty();
}
