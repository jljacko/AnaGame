#pragma once
#include "TControl.h"
class TScrollerControl : public TControl
{
public:
	TScrollerControl(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> styles);

	void onDraw(TObject* obj)override;

	void SetChildControl(TrecPointer<TControl> cont);

	virtual void Resize(D2D1_RECT_F&)override;

protected:
	TrecPointer<TControl> childControl;
};

