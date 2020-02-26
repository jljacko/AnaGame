#pragma once
#include "TGadgetControl.h"
class _ANAFACE_DLL TCheckBox :
	public TGadgetControl
{
public:
	TCheckBox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);
	~TCheckBox();

	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	void onDraw(TObject* obj = nullptr) override;
	bool IsClicked();
	//virtual void storeInTML(CArchive* ar, int childLevel)override;

	afx_msg void OnLButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	virtual UCHAR* GetAnaGameType()override;
private:

	bool isClicked;
	int width, height;
	bool onClick;

};

