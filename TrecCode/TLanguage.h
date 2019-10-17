#pragma once

#include <TObject.h>
#include <TFile.h>

typedef enum LanguageDefaultStringEncoding
{
	ldse_acsii,
	ldse_unicode
}LanguageDefaultStringEncoding;

typedef enum LanguageBlockType
{
	lbt_curly,
	lbt_indent,
	lbt_tokens
}LanguageBlockType;

typedef enum RequireModuleHeader
{
	rmh_yes,
	rmh_no,
	rmh_no_in_default
} RequireModuleHeader;

typedef enum LanguageInheritence
{
	li_mulitple,
	li_multiple_interface,
	li_single,
	li_not_supported
};

typedef enum EnumImplementation
{
	ei_primitive,
	ei_class,
	ei_labelledUnion
};

class TLanguage :
	public TObject
{
public:
	static TrecPointer<TLanguage> getLanguage(TString& langName);
	static TrecPointer<TLanguage> getLanguage(TString& langName, TString& langVersion);

protected:
	TLanguage();
	~TLanguage();

	// Very Basic information about this particular language
	TString language;					// Name of the Language
	TString version;					// The version of the langauge
	TString statementEnd;				// Characters that signify the end of a statement in the language

	// Information used to help filter out comments during the comment filter stage. Also helps String Tags recognize strings
	TDataArray<TString> singleComment;				// Comments that end when the next line begins
	TDataArray<TString> startComment, endComment;   // Multi-line comments;
	TDataArray<TString> string, multiLineString;

	LanguageBlockType blockType;					// How TrecCode know how a block is defined
	
	// Information on how this language handles strings
	LanguageDefaultStringEncoding defaultencoding;	// What String encoding to assume if the programmer doesn't mark it
	TString switchMarker;							// Marker to mark the string as ti switch from the default encoding

	// External FIle resources, code defined outside the "current" code file
	bool externalFileName;							// Whether to use the value as a file name
	RequireModuleHeader requireModHeader;			// Whether the module requires module header and where
	bool moduleHeaderFirst;							// Does the language require the module header to be the first statement
	bool supportModuleBlock;						// Specifies that the language supports module blocks
	bool useForPublic;								// Specifies that resources in file are restricted to code outside the module unless explicitly permitted

	// Object Oriented configuration for the language
	LanguageInheritence inheritenceModel;
	EnumImplementation enumModel;
	UCHAR methodImplementation;
};

