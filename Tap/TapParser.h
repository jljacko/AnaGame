#pragma once
#include "Parser.h"
typedef enum useType
{
	Anaface,   // UI
	code,      // Animate bytecode
	image,     // pictures
	video,     // videos
	graphics,  // useful for Direct3D
	shadeCode  // most 3D API's use some form of shading language
}useType;
typedef struct use
{
	TString fileDirectory; // name and directory of the file
	useType type;          // purpose of the file
}use;
class TapParser :
	public Parser
{
public:
	TapParser(TArray<use>*);
	~TapParser();
	virtual bool Obj(TString v);
	// for the attribute name

	virtual bool Attribute(TrecPointer<TString> v, TString& e);
private:
	useType currentType;
	TArray<use>* fileList;
};

