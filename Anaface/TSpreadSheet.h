#pragma once
#include "TLayoutEx.h"


typedef struct _ANAFACE_DLL BorderList
{
	float thickness;
	D2D1_COLOR_F color;
	bool isVertical;
	USHORT singlDimension;
	USHORT doubDum1, doubDum2;
}BorderList;

class _ANAFACE_DLL TSpreadSheet :
	public TLayoutEx
{
public:


	TSpreadSheet(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta, HWND w);
	~TSpreadSheet();

	virtual bool onCreate(RECT)override;
	virtual void onDraw(TObject* obj = nullptr) override;
	virtual UCHAR* GetAnaGameType()override;

	TString GetData();
	TString GetDataSplitTokens();

	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

protected:
	TString title, splitTokens;
	UCHAR header_x_span, header_y_span;
	bool initializeFields, drawLines, stickToNums, hasTitle;
	TDataArray<BorderList> borders;
	HWND window;

};

