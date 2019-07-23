#pragma once

#include "TArray.h"
#include "TObject.h"
#include <d2d1_1.h>
#include <string>
#include "TrecPointer.h"

#define T_NO_ERROR 0
#define T_OVERFLOW 1
#define NOT_NUMB 2

/*
* ColorFormat
* Used to keep track of how the color originally was presented
*/
typedef enum _TREC_LIB_DLL ColorFormat
{
	cform_hex,
	cform_hexa,
	cform_rgb,
	cform_rgba,
	cform_hsl,
	cform_hsla,
	cform_ana,
	cform_ana_a
}ColorFormat;

typedef struct IndexRange {
	int start;
	int end;
}IndexRange;

typedef struct strData
{
	WCHAR* w_str;
	char* c_str;
};


/*
* Class: TString
* Represents the UNICODE version of the String class, with features such as split,
*	number extraction, substring, and trim
*/
class _TREC_LIB_DLL TString : public TObject
{

public:
	TString();
	~TString();
	TString(TString*);
	TString(const char*);
	TString(const WCHAR*);
	TString(TString& c);
	TString(std::string& str);
	TString(WCHAR c);

	short ConvertToInt(int*);
	short ConvertToDouble(double*);
	short ConvertToLong(long long*);
	short ConvertToFloat(float*);

	TrecPointer<typename TArray<TString>> split(TString, bool checkBackSlash = false);
	WCHAR* GetBufferCopy();
	TString SubString(UINT beginningIndex, int endIndex = -1);
	void Trim();

	bool ConvertToColor(D2D1_COLOR_F& color, ColorFormat& cf);

	int FindOutOfQuotes(TString& subString, int start = 0);

	// Set Methods, to be used in place of Assignment operators to avoid C26444 Warnings
	void Set(TString& s);
	void Set(TString* s);

	void Set(WCHAR* w);
	void Set(WCHAR w);


	TString operator=(TString&);
	TString operator=(TString* s);
	TString operator=(WCHAR* w);
	TString operator=(WCHAR w);
	TString operator+(TString&);
	TString operator+(TString*);
	TString operator+(WCHAR*);
	TString operator+(WCHAR w);
	TString operator+=(TString&);
	TString operator+=(TString*);
	TString operator+=(WCHAR*);
	TString operator+=(WCHAR w);

	//void operator+=(TString*);
	//void operator

	bool operator==(TString& s);
	bool operator==(TString* s);
	bool operator==(WCHAR* s);

	bool operator!=(TString& s);
	bool operator!=(TString* s);
	bool operator!=(WCHAR* s);

	WCHAR operator[](UINT loc);

	virtual UCHAR* GetAnaGameType() override;

	UINT GetSize();
	WCHAR GetAt(UINT c);

	// Relics from MFC CString that we now hve to implement ourselves
	void AppendFormat(WCHAR* format, ...);
	void AppendFormat(TString& format, ...);
	void Format(WCHAR* format, ...);
	void Format(TString& format, ...);

	int Compare(TString& other);
	int CompareNoCase(TString& other);
	static int Compare(TString& str1, TString& str2);

	int Delete(int index, int count = 1);
	TString GetDelete(int& ret, int index, int count = 1);
	
	int Find(TString& sub, int start = 0);
	int Find(WCHAR sub, int start = 0);
	int FindOneOf(TString& chars, int start = 0);

	bool SetAsEnvironmentVariable(TString& var);

	int Insert(int index, TString& subStr);
	TString GetInsert(int& ret, int index, TString& subStr);
	int Insert(int index, WCHAR ch);
	TString GetInsert(int& ret, int index, WCHAR ch);

	void SetLower();
	TString GetLower();
	void SetUpper();
	TString GetUpper();

	void SetReverse();
	TString GetReverse();

	int Remove(WCHAR c);
	TString GetRemove(int& ret, WCHAR c);

	int Replace(TString& oldStr, TString& newStr);
	TString GetReplace(int& ret, TString& oldStr, TString& newStr);
	int Replace(WCHAR& oldStr, WCHAR& newStr);
	TString GetReplace(int& ret, WCHAR& oldStr, WCHAR& newStr);

	TString Tokenize(TString& tokens, int& start);

	void Trim();
	TString GetTrim();
	void TrimRight();
	TString GetTrimRight();
	void TrimLeft();
	TString GetTrimLeft();

private:
	UINT size;			// Note: these values focus on the number of WCHARs
	UINT capacity;
	strData string;
};

/*
* Function: convertToNumber
* Purpose: Converts a character into a number
* Parameters: TCHAR c - the character to convert
*			int* i - the new number
* Returns: bool - whether character was a number
*/
bool convertToNumber(TCHAR c, int* i);

/*
* Function: convertToNumber
* Purpose: Converts a character into a number, allowing for hew values
* Parameters: TCHAR c - the character to convert
*			int* i - the new number
* Returns: bool - whether character was a number
*/
bool convertToNumberHex(TCHAR c, UINT& i);

/*
* Function: convertHueToRGB
* Purpose: Converts an hsv value to a comparble RGB format
* Parameters: float p - the p of the new color 
*			float q - the q of the new color
*			int hue - the hue of the color
* Returns: float - the color in an RGB channel
*/
float ConvertHueToRGB(float p, float q, int hue);

/*
* Function: ReturnWCharType
* Purpose: Returns the WCHAR representation of the char
* Parameters: char c - the chaacter to convert
* Returns: WCHAR - the wide version of the character
*/
WCHAR ReturnWCharType(char c);
