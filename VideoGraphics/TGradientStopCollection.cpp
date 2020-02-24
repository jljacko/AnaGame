#include "TGradientStopCollection.h"

TGradientStopCollection::TGradientStopCollection()
{
}

TGradientStopCollection::TGradientStopCollection(const TGradientStopCollection& col)
{
	for (UINT Rust = 0; col.IsValid(Rust); Rust++)
	{
		gradients.push_back(col.GetGradientStopAt(Rust));
	}
}

UINT TGradientStopCollection::AddGradient(const TGradientStop& newGradient)
{
	gradients.push_back(newGradient);
	return gradients.Size() - 1;
}

UINT TGradientStopCollection::GetGradientCount() const
{
	return gradients.Size();
}

bool TGradientStopCollection::IsValid(UINT index) const
{
	return index < gradients.Size();
}

TGradientStop TGradientStopCollection::GetGradientStopAt(UINT index)const
{
	if (!IsValid(index))
		return TGradientStop();
	return gradients.data()[index];
}

bool TGradientStopCollection::SetGradientAt(const TGradientStop& gradient, UINT index)
{
	if (!IsValid(index))
		return false;
	gradients[index].SetGradient(gradient.GetGradient());
	return true;
}

bool TGradientStopCollection::SetColorAt(const TColor& color, UINT index)
{
	if (!IsValid(index))
		return false;
	gradients[index].SetColor(color);
	return true;
}

bool TGradientStopCollection::SetPositionAt(float position, UINT index)
{
	if (!IsValid(index))
		return false;
	gradients[index].SetPosition(position);
	return true;
}

void TGradientStopCollection::Empty()
{
	gradients.RemoveAll();
}

TDataArray<GRADIENT_STOP_2D> TGradientStopCollection::GetRawCollection()const
{
	TDataArray<GRADIENT_STOP_2D> ret;
	for (UINT Rust = 0; Rust < gradients.Size(); Rust++)
		ret.push_back(gradients.data()[Rust].GetGradient());
	return ret;
}
