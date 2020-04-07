#pragma once
#include <Parser_.h>
#include "Anaface.h"
#include "AnafaceUI.h"
#include <TDataArray.h>
#include "TSpreadSheet.h"
#include "TDataBind.h"
#include "AnimationBuilder.h"


typedef struct _ANAFACE_DLL eventNameID
{
	TString name;
	int eventID;
}eventNameID;

typedef enum class anaface_parser_mode
{
	anaface_parser_mode_normal,
	anaface_parser_mode_normal_story,
	anaface_parser_mode_persistant_story
};



class _ANAFACE_DLL AnafaceParser :
	public Parser_
{
public:
	AnafaceParser(TrecPointer<DrawingBoard>, HWND,TString);
	~AnafaceParser();

	void setEventSystem(TDataArray<eventNameID>& e);

	// for the initial object type
	virtual bool Obj(TString* v) override;
	virtual bool Attribute(TrecPointer<TString> v, TString & e)override;
	// for the attribute name

	// for the attribute value (here it may be good that TStrings are used)

	virtual bool submitType(TString v)override;
	virtual bool submitEdition(TString v)override;

	virtual bool goChild()override;
	virtual void goParent()override;
	TrecPointer<TControl> getRootControl();

	virtual UCHAR* GetAnaGameType() override;


	// Animation Methods
	TDataArray<TrecPointer<AnimationBuilder>> GetAnimations();
	TDataArray<TString> GetStoryBoards();
	TDataArray<TString> GetPersistentStoryBoards();

private:
	TrecPointer<TControl> rootObj;
	TrecPointer<TControl> currentObj;
	TrecPointer<TControl> baseObj;
//	TLayout* layoutObject; // use if object is a layout so we have access to TLayout methods
	//TTextField* textObject; // use if object is a textField so we have access to textField methods
	TrecPointer<DrawingBoard> renderer;
	TDataArray<int> columnwidth, rowHeight;
	TDataArray<bool> columnFlex, rowFlex;
	HWND windowHandle;

	TrecPointer<TArray<styleTable>> classList;
	TrecPointer<styleTable> currentStyle;

	TDataArray<TrecPointer<AnimationBuilder>> animations;
	TrecPointer<AnimationBuilder> currentAnimation;
	TDataArray<TString> basicStoryBoards;
	TDataArray<TString> persistantStoryBoards;
	anaface_parser_mode parserMode;

	int xPosition, yPosition;
	int x2Pos, y2Pos;
	bool positionSet, extendPos;

	void setLayoutParam();

	TDataArray<eventNameID>* events;
	//void setLayoutPointer();

	void addToTree(TrecPointer<TControl>);
	TString fileLoc;
	TString singleParam;

	void AddToEventList(R_Message_Type rmt, int id);
	bool handleEventAttribute(TrecPointer<TString>& v, TString & e);

};

