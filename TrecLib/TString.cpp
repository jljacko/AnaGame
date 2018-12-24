#include "stdafx.h"
#include "TString.h"


UCHAR TStringType[] = { 2, 0b10000000, 1 };

/*
* Method: (TString) (Constructor)
* Purpose: Default Constructor for the TString Class
* Parameters: void
* Returns: void
*/
TString::TString()
{
}



/*
* Method: (TString) (Destructor) 
* Purpose: Cleans up the TString
* Parameters: void
* Returns: void
*/
TString::~TString()

{
	//CString::~CString();
	//int e = 3;
}

/*
* Method: (TString) (constructor) 
* Purpose: Uses a pointer to an existing (MFC) CString to build the TString
* Parameters: CString* cps - pointer to CString to copy
* Returns: void
*/
TString::TString(CString* cps) : CString( *cps )
{
	//sys_Type = new LPCTSTR((LPCTSTR)"SYS_TSTRING");
}

/*
* Method: (TString) (constructor) 
* Purpose: Uses a pointer to another TString to build this one
* Parameters: TString* orig - pointer to TString to copy
* Returns: void
*/
TString::TString(TString * orig) : CString(*orig)
{

}

/*
* Method: (TString) (constructor) 
* Purpose: Uses a C-String  to construct TString
* Parameters: const char* cps - C-String to copy
* Returns: void
*/
TString::TString(const char* cps) : CString{ cps }
{
	//sys_Type = new LPCTSTR((LPCTSTR)"SYS_TSTRING");
}

/*
* Method: (TString) (constructor) 
* Purpose: Builds a TString off of a wide char array
* Parameters: const WCHAR* wcps - Wide Character array to copy
* Returns: void 
*/
TString::TString(const WCHAR * wcps) : CString{wcps}
{
}

/*
* Method: (TString) (constructor)
* Purpose: Uses a reference to an existing (MFC) CString to build the TString
* Parameters: CString& c - reference to CString to copy
* Returns: void
*/
TString::TString(CString & c) : CString{c}
{
}

/*
* Method: (TString) (constructor)
* Purpose: Uses a reference to another TString to build this one
* Parameters: TString* orig - reference to TString to copy
* Returns: void
*/
TString::TString(TString & c):CString{c}
{
}
/*
* Method: (TString) (constructor)
* Purpose: Uses a reference to an existing C++ String to build the TString
* Parameters: CString* cps - reference to C++ String to copy
* Returns: void
*/
TString::TString(std::string & str): CString{str.c_str()}
{
}

/*
* Method: TString - ConvertToInt
* Purpose: Converts a Compliant TString into an integer
* Parameters: int* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToInt(int* value)
{
	int size = this->GetLength();
	if (!size)
		return NOT_NUMB;
	int temp = *value;

	*value = 0;
	int hold;
	bool positive = true;

	for (int c = 0; c < size; c++)
	{
		if (convertToNumber(this->GetAt(c), &hold))
		{
			*value = (*value * 10) + hold;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			*value = temp;
			return NOT_NUMB;
		}
	}
	if (positive)
		*value = abs(*value);
	else
		*value = -(abs(*value));
	return T_NO_ERROR;
}

/*
* Method: TString - ConvertToLong
* Purpose: Converts a Compliant TString into a long integer
* Parameters: long* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToLong(long long* value)
{
	
	int size = this->GetLength();

	if (!size)
		return NOT_NUMB;
	long temp = *value;

	*value = 0;
	int hold;

	bool positive = true;

	for (int c = 0; c < size; c++)
	{
		if (convertToNumber(this->GetAt(c), &hold))
		{
			*value = (*value * 10) + hold;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			*value = temp;
			return NOT_NUMB;
		}
	}
	if (positive)
		*value = abs(*value);
	else
		*value = -(abs(*value));
	return T_NO_ERROR;

}

/*
* Method: TString - ConvertToDouble
* Purpose: Converts a Compliant TString into a double floating point number
* Parameters: double* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToDouble(double* value)
{
	int size = this->GetLength();
	if (!size)
		return NOT_NUMB;
	double temp = *value;

	*value = 0;
	double dec = 0.1;
	bool fullInt = true;
	int hold;

	bool positive = true;

	for (int c = 0; c < size; c++)
	{
		if (fullInt && convertToNumber(this->GetAt(c), &hold))
		{
			*value = (*value * 10) + hold;
		}
		else if (convertToNumber(this->GetAt(c), &hold)) // moved to decimal portion
		{
			*value = *value + (double)hold * dec;
			dec = dec / 10;
		}
		else if (this->GetAt(c) == L'.')
		{
			fullInt = false;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			*value = temp;
			return NOT_NUMB;
		}

	}
	if (positive)
		*value = abs(*value);
	else
		*value = -(abs(*value));

	return T_NO_ERROR;
}

/*
* Method: TString - ConvertToFloat
* Purpose: Converts a Compliant TString into a float
* Parameters: float* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToFloat(float* value)
{
	int size = this->GetLength();
	if (!size)
		return NOT_NUMB;
	float temp = *value;

	*value = 0;
	float dec = 0.1f;
	bool fullInt = true;
	int hold;
	bool positive = true;

	for (int c = 0; c < size; c++)
	{
		if (fullInt && convertToNumber(this->GetAt(c), &hold))
		{
			*value = (*value * 10) + hold;
		}
		else if (convertToNumber(this->GetAt(c), &hold)) // moved to decimal portion
		{
			*value = *value + (float)hold * dec;
			dec = dec / 10;
		}
		else if (this->GetAt(c) == L'.')
		{
			fullInt = false;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			*value = temp;
			return NOT_NUMB;
		}

	}

	if (positive)
		*value = abs(*value);
	else
		*value = -(abs(*value));

	return T_NO_ERROR;
}

/*
* Method: TString - split
* Purpose: Splits a String by the provided deliniators
* Parameters: TString str - the TString holding deliniators
* Returns: TrecPointer<TArray<TString>> - Array of TStrings holding tokens
*/
TrecPointer<TArray<TString>> TString::split(TString str)
{
	//TArray<TString>* tats = new TArray<TString>();
	TrecPointer<TArray<TString>> ret;
	ret = new TArray<TString>();

	if (!this->GetLength())
	{
		return ret;
	}

	CString tok;

	int pos = 0;
	tok = this->Tokenize(str, pos);
	while (!tok.IsEmpty())
	{
		(ret).get()->Add(new TString(new TString(&tok)));
		tok = this->Tokenize(str, pos);
	}
	ret.Boost();
	return ret;
}

/*
* Method: TString - GetBufferCopy
* Purpose: Gets a copy of the Raw Buffer
* Parameters: void
* Returns: WCHAR* - the copy of the TStrings buffer
*/
WCHAR * TString::GetBufferCopy()
{
	WCHAR* returnable = new WCHAR[GetLength() + 1];
	for (int c = 0; c < GetLength(); c++)
		returnable[c] = *this[c];
	returnable[GetLength()] = L'\0';
	return returnable;
}

/*
* Method: TString - SubString
* Purpose: Returns a substring specified 
* Parameters: UINT beginningIndex - the index to start at
*			int endIndex - exclusive index to end (use negative value to go to end)
* Returns: TString - the Substring generated
*/
TString TString::SubString(UINT beginningIndex, int endIndex)
{

	TString returnable;
	if (endIndex == -1)
	{
		for (int c = beginningIndex; c < GetLength(); c++)
			returnable += GetAt(c);
	}
	else
	{
		endIndex = abs(endIndex);
		if (endIndex > GetLength())
			return returnable;
		for (UINT c = beginningIndex; c < endIndex; c++)
			returnable += GetAt(c);
	}
	return returnable;
}

/*
* Method: TString - Trim
* Purpose: Removes Whitespace on both sides
* Parameters: void
* Returns: void
*/
void TString::Trim()
{
	TrimRight();
	TrimLeft();
}

/*
* Method: TString - ConvertToColor
* Purpose: Converts the string into a Direct2D color representation
* Parameters: D2D1_COLOR_F & color - the color to generate
*			ColorFrmat& cf - the Color Format detected in the string
* Returns: bool - success if true, false if TString doesn't hold a color
*/
bool TString::ConvertToColor(D2D1_COLOR_F & color, ColorFormat& cf)
{
	if(!GetLength())
		return false;
	// First Check for a Hex Format
	if (GetAt(0) == L'#' && GetLength() == 7 || GetLength() == 9)
	{
		UINT value = 0;
		UCHAR c = 0,shift;
		for (c = 1, shift = 24; c < GetLength(); c += 2, shift -= 8)
		{
			UINT v1, v2;
			if (!convertToNumberHex(GetAt(c), v1) || convertToNumberHex(GetAt(c + 1), v2))
				return false;
			v1 *= 16;
			v1 += v2;

			value += (v1 << shift);
		}

		if (c == 7) // we have cform_hex
		{
			cf = cform_hex;
			value = value | 255; // Make alpha completly transparent
		}
		else if (c == 9) // we have cform_hexa
		{
			cf = cform_hexa;
		}
		else // Supposed to be hex, but invalid length encountered
		{
			return false;
		}
		UCHAR r = value >> 24;
		UCHAR g = value >> 16;
		UCHAR b = value >> 8;
		UCHAR a = static_cast<UCHAR>(value);

		color.r = static_cast<float>(r) / 255.0f;
		color.g = static_cast<float>(g) / 255.0f;
		color.b = static_cast<float>(b) / 255.0f;
		color.a = static_cast<float>(a) / 255.0f;
		return true;
	}

	// This wasn't a hex, try AnaGames format
	TrecPointer<TArray<TString>> values = split(TString(L", "));
	if (values.get() && values->Count() > 2)
	{
		bool works = true;
		D2D1_COLOR_F tempColor = { 0.0f,0.0f,0.0f,0.0f };
		if (!works || !values->ElementAt(0).get() || values->ElementAt(0)->ConvertToFloat(&tempColor.r))
			works = false;

		if (!works || !values->ElementAt(1).get() || values->ElementAt(1)->ConvertToFloat(&tempColor.g))
			works = false;

		if (!works || !values->ElementAt(2).get() || values->ElementAt(2)->ConvertToFloat(&tempColor.b))
			works = false;

		if (works && values->Count() > 3)
		{
			if (!values->ElementAt(3).get() || values->ElementAt(3)->ConvertToFloat(&tempColor.a))
				works = false;
			else
				cf = cform_ana_a;
		}
		else if (works)
		{
			tempColor.a = 1.0f;
			cf = cform_ana;
		}

		if (works)
			color = tempColor;
	}

	// Okay, check for rbg[a] or hsl[a]
	values = split(TString(L", ()"));
	if (!values.get() || values->Count() < 4) // We don't have enough data for either format
		return false;

	if (!values->ElementAt(0).get())
		return false;
	values->ElementAt(0)->MakeLower();

	if (!values->ElementAt(0)->Compare(L"rgb") || !values->ElementAt(0)->Compare(L"rgba"))
	{
		// Okay, it seems we have an rgb[a] format

		// First, make sure we have our strings.
		if (!values->ElementAt(1).get() || !values->ElementAt(2).get() || !values->ElementAt(3).get())
			return false;

		//D2D1_COLOR_F tempColor = { 0.0f,0.0f,0.0f,0.0f };
		int tempColor[4];
		if (values->ElementAt(1)->ConvertToInt(&tempColor[0]) ||
			values->ElementAt(2)->ConvertToInt(&tempColor[1]) ||
			values->ElementAt(3)->ConvertToInt(&tempColor[2]))
			return false;

		// Okay, we have legitimate values now, time to convert
		color.r = static_cast<float>(tempColor[0]) / 255.0f;
		color.g = static_cast<float>(tempColor[1]) / 255.0f;
		color.b = static_cast<float>(tempColor[2]) / 255.0f;

		// Try to see if alpha is available
		if (values->Count() > 4 && values->ElementAt(4).get() && !values->ElementAt(4)->ConvertToFloat(&color.a))
		{
			cf = cform_rgba;
			return true;
		}

		// Alpha didn't work, set to default of 1.0f
		cf = cform_rgb;
		color.a = 1.0f;
		return true;

	}

	if (!values->ElementAt(0)->Compare(L"hsl") || !values->ElementAt(0)->Compare(L"hsla"))
	{
		// Whoa! We have the more complex hsl format

		// Try getting the hue
		int h = 0;
		if (!values->ElementAt(1).get() || values->ElementAt(1)->ConvertToInt(&h))
			return false;
		
		h = abs(h) % 360;

		if (!values->ElementAt(2).get() || values->ElementAt(3).get())
			return false;

		values->ElementAt(2)->Remove(L'%');
		values->ElementAt(3)->Remove(L'%');

		float s, l;
		if (values->ElementAt(2)->ConvertToFloat(&s) || values->ElementAt(2)->ConvertToFloat(&l))
			return false;
		if (s > 100.0f || l > 100.0f)
			return false;

		// We now have legitimate values for hsl and now we need to convert it to rgb
		// The algorithm is based of of the JavaScript code for the conversion by a Garry Tan 
		// was posted at StackOverFlow at this url: https://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
		if (!s)
		{
			color.r = color.g = color.b = 0.0f;
		}
		else
		{
			l /= 100.f;
			s /= 100.f;

			float q = l < 0.5 ? l * (100.0f + s) : l + s - l * s;
			float p = 2 * l - q;

			int r_h = h + 120;
			int g_h = h;
			int b_h = h - 120;

			// Target the r value
			color.r = ConvertHueToRGB(p, q, r_h);
			color.g = ConvertHueToRGB(p, q, g_h);
			color.b = ConvertHueToRGB(p, q, b_h);


		}

		// Try to see if alpha is available
		if (values->Count() > 4 && values->ElementAt(4).get() && !values->ElementAt(4)->ConvertToFloat(&color.a))
		{
			cf = cform_hsla;
			return true;
		}

		// Alpha didn't work, set to default of 1.0f
		cf = cform_hsl;
		color.a = 1.0f;
		return true;
	}

	// No regonizable format was used, return false;
	return false;
}


/*
* Method: TString - operator=
* Purpose: Assigns an Existing TString to this TString
* Parameters: TString& t - the TString to copy
* Returns: void 
*/
TString TString::operator=(TString &t)
{
	this->Empty();
	for (int c = 0;c < t.GetLength();c++)
	{
		this->AppendChar(t.GetAt(c));
	}
	return this;
}

/*
* Method: TString - operator=
* Purpose: Assigns an Existing TString to this TString
* Parameters: TString& t - the TString to copy
* Returns: void
*/
TString TString::operator=(CString &t)
{
	this->Empty();
	for (int c = 0;c < t.GetLength();c++)
	{
		this->AppendChar(t.GetAt(c));
	}
	return this;
}

/*
* Method: TString - operator=
* Purpose: Assigns an Existing TString to this TString
* Parameters: TString* s - the TString to copy
* Returns: void
*/
TString TString::operator=(TString * s)
{
	if (s)
	{
		this->Empty();
		for (int c = 0; c < s->GetLength(); c++)
		{
			this->AppendChar(s->GetAt(c));
		}
	}
	return this;
}

/*
* Method: TString - operator=
* Purpose: Assigns an Existing CString to this TString
* Parameters: CString* s - the CString to copy
* Returns: void
*/
TString TString::operator=(CString * s)
{
	if (s)
	{
		this->Empty();
		for (int c = 0; c < s->GetLength(); c++)
		{
			this->AppendChar(s->GetAt(c));
		}
	}
	return this;
}

/*
* Method: TString - operator=
* Purpose: Assigns an existing wide string to this TString
* Parameters: WCHAR* w - the wide string to copy
* Returns: void
*/
TString TString::operator=(WCHAR * w)
{
	if (w)
	{
		this->Empty();
		for (int c = 0; *w != L'\0'; c++, w++)
			this->AppendChar(*w);
	}
	return this;
}

/*
* Method: TString - operator=
* Purpose: Assigns a WCHAR to this TString
* Parameters: WCHAR w - the Character to copy
* Returns: void
*/
TString TString::operator=(WCHAR w)
{
	this->Empty();
	this->AppendChar(w);
	return this;
}

/*
* Method: TString - operator+
* Purpose: Adds the contents of an existing TString to this one 
* Parameters: TString& t - the TString to append
* Returns: void
*/
TString TString::operator+(TString & t)
{
	for (int c = 0; c < t.GetLength();c++)
		AppendChar(t.GetAt(c));
	return this;
}

/*
* Method: TString - operator+
* Purpose: Adds the contents of an existing TString to this one
* Parameters: TString& t - the TString to append
* Returns: void
*/
TString TString::operator+(CString &t)
{
	for (int c = 0; c < t.GetLength();c++)
		AppendChar(t.GetAt(c));
	return this;
}

/*
* Method: TString - operator+
* Purpose: Adds the contents of an existing TString to this one
* Parameters: TString& t - the TString to append
* Returns: void
*/
TString TString::operator+(TString *t)
{
	if (t)
	{
		for (int c = 0; c < t->GetLength(); c++)
			AppendChar(t->GetAt(c));
	}
	return this;
}

/*
* Method: TString - operator+
* Purpose: Adds the contents of an existing CString to this one
* Parameters: CString& t - the CString to append
* Returns: void
*/
TString TString::operator+(CString *t)
{
	if (t)
	{
		for (int c = 0; c < t->GetLength(); c++)
			AppendChar(t->GetAt(c));
	}
	return this;
}

/*
* Method: TString - operator+
* Purpose: Adds the contents of an existing wide string to this one
* Parameters: WCHAR* w - the wide string to append
* Returns: void
*/
TString TString::operator+(WCHAR * w)
{
	if (w)
	{
		for (int c = 0; w[c] != L'\0'; c++)
			this->AppendChar(*w);
	}
	return this;
}

/*
* Method: TString - operator==
* Purpose: Conpares the TString with a TString and returns true if contents are equal
* Parameters: TString& s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator==(TString & s)
{
	if (GetLength() != s.GetLength())
		return false;
	for (int c = 0; c < GetLength(); c++)
		if (GetAt(c) != s[c])
			return false;
	return true;
}

/*
* Method: TString - operator==
* Purpose: Conpares the TString with a CString and returns true if contents are equal
* Parameters: CString& s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator==(CString & s)
{
	if (GetLength() != s.GetLength())
		return false;
	for (int c = 0; c < GetLength(); c++)
		if (GetAt(c) != s[c])
			return false;
	return true;
}

/*
* Method: TString - operator==
* Purpose: Conpares the TString with a TString and returns true if contents are equal
* Parameters: TString* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator==(TString * s)
{
	if(!s)
	return false;
	if (GetLength() != s->GetLength())
		return false;
	for (int c = 0; c < GetLength(); c++)
		if (GetAt(c) != s->GetAt(c))
			return false;
	return true;
}

/*
* Method: TString - operator==
* Purpose: Conpares the TString with a CString and returns true if contents are equal
* Parameters: CString* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator==(CString * s)
{
	if (!s)
		return false;
	if (GetLength() != s->GetLength())
		return false;
	for (int c = 0; c < GetLength(); c++)
		if (GetAt(c) != s->GetAt(c))
			return false;
	return true;
}

/*
* Method: TString - operator==
* Purpose: Conpares the TString with a wide string and returns true if contents are equal
* Parameters: WCHAR* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator==(WCHAR * s)
{
	if(!s)
		return false;
	for (int c = 0; c < GetLength(); c++,s++)
	{
		if (*s == L'\0')
			return false;
		if (*s != GetAt(c))
			return false;
	}
	return *s == L'\0';
}

/*
* Method: TString - operator!=
* Purpose: Conpares the TString with a TString and returns true if contents are unequal
* Parameters: TString& s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator!=(TString & s)
{
	return !(*this == s);
}

/*
* Method: TString - operator!=
* Purpose: Conpares the TString with a CString and returns true if contents are unequal
* Parameters: CString& s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator!=(CString & s)
{
	return !(*this == s);
}

/*
* Method: TString - operator!=
* Purpose: Conpares the TString with a TString and returns true if contents are unequal
* Parameters: TString* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator!=(TString * s)
{
	return !(*this == s);
}

/*
* Method: TString - operator!=
* Purpose: Conpares the TString with a CString and returns true if contents are unequal
* Parameters: CString* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator!=(CString * s)
{
	return !(*this == s);
}

/*
* Method: TString - operator!=
* Purpose: Conpares the TString with a wide string and returns true if contents are unequal
* Parameters: WCHAR* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator!=(WCHAR * s)
{
	return !(*this == s);
}

/*
* Method: TString - GetAnaGameType
* Purpose: Retrieves the Class type for the AnaGame Virtual Machine
* Parameters: void
* Returns: UCHAR* - The AnaGAme type ID
*/
UCHAR * TString::GetAnaGameType()
{
	return TStringType;
}


/*
* Function: convertToNumber
* Purpose: Converts a character into a number
* Parameters: TCHAR c - the character to convert
*			int* i - the new number
* Returns: bool - whether character was a number
*/
bool convertToNumber(TCHAR c, int* i)
{
	switch (c)
	{
	case '0':
		*i = 0;
		return true;
	case '1':
		*i = 1;
		return true;
	case '2':
		*i = 2;
		return true;
	case '3':
		*i = 3;
		return true;
	case '4':
		*i = 4;
		return true;
	case '5':
		*i = 5;
		return true;
	case '6':
		*i = 6;
		return true;
	case '7':
		*i = 7;
		return true;
	case '8':
		*i = 8;
		return true;
	case '9':
		*i = 9;
		return true;
	default:
		return false;
	}
}

/*
* Function: convertToNumber
* Purpose: Converts a character into a number, allowing for hew values
* Parameters: TCHAR c - the character to convert
*			int* i - the new number
* Returns: bool - whether character was a number
*/
bool convertToNumberHex(TCHAR c, UINT & i)
{
	switch (c)
	{
	case L'0':
		i = 0;
		return true;
	case L'1':
		i = 1;
		return true;
	case L'2':
		i = 2;
		return true;
	case L'3':
		i = 3;
		return true;
	case L'4':
		i = 4;
		return true;
	case L'5':
		i = 5;
		return true;
	case L'6':
		i = 6;
		return true;
	case L'7':
		i = 7;
		return true;
	case L'8':
		i = 8;
		return true;
	case L'9':
		i = 9;
		return true;
	case L'a':
	case L'A':
		i = 10;
		return true;
	case L'b':
	case L'B':
		i = 11;
		return true;
	case L'c':
	case L'C':
		i = 12;
		return true;
	case L'd':
	case L'D':
		i = 13;
		return true;
	case L'e':
	case L'E':
		i = 14;
		return true;
	case L'f':
	case L'F':
		i = 15;
		return true;
	}
	return false;
}

/*
* Function: convertHueToRGB
* Purpose: Converts an hsv value to a comparble RGB format
* Parameters: float p - the p of the new color
*			float q - the q of the new color
*			int hue - the hue of the color
* Returns: float - the color in an RGB channel
*/
float ConvertHueToRGB(float p, float q, int hue)
{
	float javaScriptHue = static_cast<float>(hue) / 360.0f;

	if (javaScriptHue < 1.0f / 6.0f)
		return p + (q - p) * 6 * javaScriptHue;
	if (javaScriptHue < 0.5f)
		return q;
	if (javaScriptHue < 2.0f / 3.0f)
		return p + (q - p)*(2.0f / 3.0f - javaScriptHue) * 6.0f;
	return p;

}

/*
* Function: ReturnWCharType
* Purpose: Returns the WCHAR representation of the char
* Parameters: char c - the chaacter to convert
* Returns: WCHAR - the wide version of the character
*/
WCHAR ReturnWCharType(char c)
{
	WCHAR w[] = { L'0',L'\0' };
	size_t conv = 0;
	char charTo[] = { c, '\0' };
	mbstowcs_s(&conv, w, 2, charTo, 1);
	return w[0];
}
