#pragma once
#include "TControl.h"

typedef enum orgLayout
{
	grid,
	VStack,
	HStack,
	VBuff,
	HBuff,
	VMix,
	HMix,
	tCanvas
}orgLayut;

typedef enum conflictRes
{
	default_Margin,
	default_size

}conflictRes;

typedef enum specialLayout
{
	Basic,
	comboBox,
	list,
	Flyout

}specialLayout;

class containerControl : public TObject
{
public:
	UINT x = 0, y=0;
	TrecPointer<TControl> contain;
	bool extend;
	UINT x2 = 0, y2 = 0;
};

class _ANAFACE_DLL TLayout :
	public TControl
{
	friend class AnafaceUI;
public:
	TLayout(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta);
	virtual ~TLayout();

	bool setLayout(orgLayout);
	bool setConflictResolutionMode(conflictRes);
	bool setSpecialFunction(specialLayout);

	bool addColunm(int, bool markDetected);
	bool addRow(int, bool markDetected);
	int addChild(TrecPointer<TControl>, UINT, UINT);
	int addChild(TrecPointer<TControl>, UINT, UINT, UINT, UINT);
	bool setGrid(TDataArray<int>&, TDataArray<int>&);
	bool setStack(TDataArray<int>&);
	//bool setMainChild(int, int);

	//int loadFromTML(CArchive* ar) override;
	int loadFromHTML(TFile* ar) override;
	void storeInTML(TFile* ar, int childLevel,bool ov = false)override;
	void storeInHTML(TFile* ar)override;

	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	virtual void onDraw(TObject* obj = nullptr) override;

	int returnColumnsWidth(int x);
	int returnRowsHeight(int y);
	int getColunmWidth(int x);
	int getRowHeight(int y);
	virtual UINT determineMinHeightNeeded()override;
	void SetNewLocation(const D2D1_RECT_F& r)override;
	void ShrinkHeight()override;

	void setNewColunmSize(int xLoc, int x);
	void setNewRowSize(int yLoc, int y);

	int getRowNumber();
	int getColumnNumber();

	D2D1_RECT_F getRawSectionLocation(int r, int c);

	TrecPointer<TControl> GetLayoutChild(int x, int y);
	orgLayout GetOrganization();

	virtual UCHAR* GetAnaGameType()override;

	virtual void SetNewRenderTarget(TrecPointer<DrawingBoard> rt);

protected:
	TArray<containerControl> lChildren;   // used to organize children more easily than regular TControl

	void SwitchChildControl(TrecPointerSoft<TControl> curControl, TrecPointer<TControl> newControl)override;
	//RECT expand; // if combobox
	int colunms, rows;
	orgLayout organization;
	specialLayout specialFunction;
	conflictRes conflictResolute;
	//bool showExpanded;
	int returnMinX(bool newColumn);
	int returnMinY(bool newRow);
	D2D1_RECT_F returnRectX(int);
	D2D1_RECT_F returnRectY(int);

	//RECT returnRectInner(int x, int y);

	TrecPointer<containerControl> tempContC;
	TControl* main;

	TDataArray<int> columnLines, rowLines;
	TDataArray<UCHAR> columnFlex, rowFlex;
	bool GetColumnFlexAt(UINT col);
	bool GetRowFlexAt(UINT row);

	void AddToColFlex(bool val);
	void AddToRowFlex(bool val);

	UINT GetTotalFlexRow();
	UINT GetTotalSetRow();
	UINT GetTotalFlexCol();
	UINT GetTotalSetCol();
	void Resize(D2D1_RECT_F& r)override;

	bool updateRow, updateColumn;


	// Brushes unique to TLayouts
	TrecComPointer<ID2D1Brush> internalBrush;
	D2D1_COLOR_F internalColor;
	bool internalInit;
	float thickness;
};

