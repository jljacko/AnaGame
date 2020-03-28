#pragma once
#include <TObject.h>
#include "TGradientStop.h"
#include <TDataArray.h>


/**
 * class TGradientStopCollection
 * Purpose: Holds a collection of gradientStops for use in creating non-solid TBrush's
 */
class _VIDEO_GRAPHICS TGradientStopCollection :	public TObject
{
public:

	/**
	 * Mehtod: TGradientStopCollection::TGradientStopCollection
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new empty TGradientStopCollection Object
	 */
	TGradientStopCollection();

	/**
	 * Mehtod: TGradientStopCollection::TGradientStopCollection
	 * Purpose: copy constructor
	 * Parameters: const TGradientStopCollection& col - the collection to copy
	 * Returns: new TGradientStopCollection Object
	 */
	TGradientStopCollection(const TGradientStopCollection& col);


	/**
	 * Mehtod: TGradientStopCollection::AddGradient
	 * Purpose: Adds a new gradient to the collection
	 * Parameters: const TGradientStop& newGradient - the grdient to add
	 * Returns: UINT - the new number of gradients currently held
	 */
	UINT AddGradient(const TGradientStop& newGradient);

	/**
	 * Mehtod: TGradientStopCollection::GetGradientCount
	 * Purpose: Reports the current count of the collected gradients
	 * Parameters: void
	 * Returns: UINT - the number of gradients currently held
	 */
	UINT GetGradientCount()const;
	

	/**
	 * Mehtod: TGradientStopCollection::IsValid
	 * Purpose: Reports whether the desired index is within bounds
	 * Parameters: UNIT index - the index of the gradient
	 * Returns: bool - whether the specified index is within bounds
	 */
	bool IsValid(UINT index)const;

	/**
	 * Mehtod: TGradientStopCollection::GetGradientStopAt
	 * Purpose: Retrieves the Gradient at the specified location
	 * Parameters: UINT index - the index of the gradient
	 * Returns: TGradientStop - the Gradient requested
	 */
	TGradientStop GetGradientStopAt(UINT index)const;

	/**
	 * Mehtod: TGradientStopCollection::GetColorAt
	 * Purpose: Gets the color at the specified index
	 * Parameters: UINT index - the index of the gradient
	 * Returns: TColor - the color requested
	 *
	 * Note: if the index is out of bounds, Black is returned
	 */
	TColor GetColorAt(UINT index);


	/**
	 * Mehtod: TGradientStopCollection::SetGradientAt
	 * Purpose: Sets the gradient at the specified location
	 * Parameters: const TGradientStop& gradient - the gradient to set
	 *				UINT index - the index of the gradient to operate on
	 * Returns: bool - whether the operation worked (if the index was in bounds)
	 */
	bool SetGradientAt(const TGradientStop& gradient, UINT index);

	/**
	 * Mehtod: TGradientStopCollection::SetColorAt
	 * Purpose: Sets the color of the specified gradient
	 * Parameters: const TColor& color - the color to set the gradient to
	 *				UINT index - the index of the gradient to operate on
	 * Returns:bool  - whether the operation worked (if the index was in bounds)
	 */
	bool SetColorAt(const TColor& color, UINT index);

	/**
	 * Mehtod: TGradientStopCollection::SetPositionAt
	 * Purpose: Sets the position of the specified gradient
	 * Parameters: float position - the position to set
	 *				UINT index - the index of the gradient to operate on
	 * Returns: bool - whether the operation worked (if the index was in bounds)
	 */
	bool SetPositionAt(float position, UINT index);


	/**
	 * Mehtod: TGradientStopCollection::Empty
	 * Purpose: Empties the collection
	 * Parameters: void
	 * Returns: void
	 */
	void Empty();


	/**
	 * Mehtod: TGradientStopCollection::GetRawCollection
	 * Purpose: Returns the collection in a form compatible with Direct2D
	 * Parameters: void
	 * Returns: TDataArray<GRADIENT_STOP_2D> - the raw gradient collection, compatible with Direct2D
	 */
	TDataArray<GRADIENT_STOP_2D> GetRawCollection()const;
protected:
	/**
	 * The collection of gradients
	 */
	TDataArray<TGradientStop> gradients;
};

