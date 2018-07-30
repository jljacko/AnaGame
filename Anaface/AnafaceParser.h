#pragma once
#include <Parser_.h>
#include "Anaface.h"
#include "AnafaceUI.h"
#include <TDataArray.h>


typedef struct _ANAFACE_DLL eventNameID
{
	TString name;
	int eventID;
}eventNameID;



class _ANAFACE_DLL AnafaceParser :
	public Parser_
{
public:
	AnafaceParser(TrecComPointer<ID2D1RenderTarget>, HWND,TString);
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

private:
	TrecPointer<TControl> rootObj;
	TrecPointer<TControl> currentObj;
	TrecPointer<TControl> baseObj;
//	TLayout* layoutObject; // use if object is a layout so we have access to TLayout methods
	//TTextField* textObject; // use if object is a textField so we have access to textField methods
	TrecComPointer<ID2D1RenderTarget> renderer;
	TDataArray<int> columnwidth, rowHeight;
	TDataArray<bool> columnFlex, rowFlex;
	HWND windowHandle;

	TrecPointer<TArray<styleTable>> classList;
	TrecPointer<styleTable> currentStyle;

	int xPosition, yPosition;
	int x2Pos, y2Pos;
	bool positionSet, extendPos;

	void setLayoutParam();

	TDataArray<eventNameID>* events;
	//void setLayoutPointer();

	void addToTree(TrecPointer<TControl>);
	TString fileLoc;

	void AddToEventList(R_Message_Type rmt, int id);
	bool handleEventAttribute(TrecPointer<TString>& v, TString & e);

};

