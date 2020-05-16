#pragma once
#include "TGadgetControl.h"
#include "TFlyout.h"

class TComboBox;

/**
 * Class: TComboBoxExtension
 * Purpose: Special control meant to be attached to a TComboBox via a TFlyout and providing the drop-down menu
 */
class TComboBoxExtension: public TControl
{
	friend class TComboBox;
public: 
	TComboBoxExtension(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, TrecSubPointer<TControl, TComboBox> combo);

	void addElement(TString&);
	bool removeElement(TString&);

	void Resize(D2D1_RECT_F& r) override;

	virtual void onDraw(TObject* obj)override;

	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)override;

	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;
protected:
	TDataArray<TString> elements;
	UINT childHeight;

	UINT maxHeight;

	int hoverSelection;
	int clickSelection;
	TrecSubPointer<TControl, TComboBox> combo;
};


/*
* Class: TComboBox
* Purpose: Supports Anaface's ability to have drop down menus
* SuperClass: TGadgetControl - allows the combobox to have a feture within it
*/
class _ANAFACE_DLL TComboBox :
	public TGadgetControl
{
	friend TComboBox;
public:
	TComboBox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta);
	~TComboBox();

	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;

	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)override;

	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	void Resize(D2D1_RECT_F& r) override;

	void UpdateCaption(TString& str, UINT index);
private:
	TrecPointer<TFlyout> flyout;
	TrecSubPointer<TControl, TComboBoxExtension> extension;

	TString defaultText;


	D2D1_POINT_2F leftpoint, vertexPoint, rightPoint;

	bool initClick;
};
