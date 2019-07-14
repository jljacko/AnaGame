#include "TImage.h"

static D2D1_MATRIX_3X2_F identityMatrix = D2D1::Matrix3x2F::Identity();

TImage::TImage(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> styles): TControl(rt, styles, false)
{
	rotation = identityMatrix;
	currentImage = 0;
}

TImage::TImage(TImage& im) : TControl(im)
{
	rotation = identityMatrix;
	currentImage = 0;
}

TImage::TImage() :TControl()
{
	rotation = identityMatrix;
	currentImage = 0;
}

TImage::~TImage()
{
	for (UINT Rust = 0; Rust < images.Size(); Rust++)
	{
		TBitmap bit = images[Rust];
		if (bit.d2dBitbap) bit.d2dBitbap->Release();
		if (bit.wicBitmap) bit.wicBitmap->Release();
	}
}

UINT TImage::onFill(CPoint& pixel, D2D1_COLOR_F& color, float tolerance)
{
	if(currentImage >= images.Size())
		return 1;
	
	TBitmap bitmap = images[currentImage];
	if (!bitmap.d2dBitbap || !bitmap.wicBitmap)
		return 2;

	// Create a Lock through which we can edit the bitmap
	UINT height = 0, width = 0;
	bitmap.wicBitmap->GetSize(&width, &height);

	double resHeight = 0.0, resWidth = 0.0;
	bitmap.wicBitmap->GetResolution(&resWidth, &resHeight);
	
	pixel.x -= location.left;
	pixel.y -= location.top;

	pixel.x = static_cast<double>(pixel.x) / resWidth;
	pixel.y = static_cast<double>(pixel.y) / resHeight;

	IWICBitmapLock* pLock = nullptr;
	WICRect wicRect{ 0,0,width, height };

	if (SUCCEEDED(bitmap.wicBitmap->Lock(&wicRect, WICBitmapLockRead | WICBitmapLockWrite, &pLock))) {
		UINT stride = 0;
		pLock->GetStride(&stride);
		BYTE* imageData = nullptr;
		UINT imageDataSize = 0;

		UINT iColor = 0;
		ConvertD2D1ColorToUIntColor(color, iColor);

		if (SUCCEEDED(pLock->GetDataPointer(&imageDataSize, &imageData)))
		{
			// Prepare a marked map for marking pixels for change
			TDataArray<TDataArray<PixelMark>> pixels;
			
			for (UINT Rust = 0, pixelCount = 0; pixelCount < imageDataSize; Rust++)
			{
				TDataArray<PixelMark> row;
				for (UINT C = 0; pixelCount < imageDataSize && C < stride; C++, pixelCount++)
				{
					PixelMark pixelMark;
					memcpy(&pixelMark.pixel, &imageData[4 * pixelCount], 4);
					pixelMark.marker = 0; // This pixel, just created, has not been looked at by the algorithm

					row.push_back(pixelMark);
				}
				pixels.push_back(row);
			}

			// Now determine the pixels that need to be updated
			MarkPixels(pixel, iColor, tolerance, pixels);


			// Now that we know which pixels to update, go through and update the pixels
			for (UINT Rust = 0; Rust < pixels.Size(); Rust++)
			{
				for (UINT C = 0; C < pixels[Rust].Size(); C++)
				{
					auto pixelMark = pixels[Rust][C];
					if (pixelMark.marker == 2)
					{
						memcpy(&imageData[4 * (Rust * stride + C)], &pixelMark.pixel, 4);
					}
				}
			}

			ID2D1Bitmap* releaseImage = bitmap.d2dBitbap;

			renderTarget->CreateBitmapFromWicBitmap(bitmap.wicBitmap, &bitmap.d2dBitbap);
			images[currentImage].d2dBitbap = bitmap.d2dBitbap;
			releaseImage->Release();
		}
		pLock->Release();
	}

	return 0;
}

UINT TImage::setLinearGradient(TDataArray<D2D1_COLOR_F>& colors)
{
	if (!cover.get())
		cover = new TContent(renderTarget, this);
	cover->SetLinearImage(colors);
	return 0;
}

UINT TImage::setRadialGradient(TDataArray<D2D1_COLOR_F>& colors)
{
	if (!cover.get())
		cover = new TContent(renderTarget, this);
	cover->SetRadialImage(colors);
	return 0;
}

UINT TImage::setLinearGradient(TDataArray<D2D1_GRADIENT_STOP>& colors)
{
	if (!cover.get())
		cover = new TContent(renderTarget, this);
	cover->SetLinearImage(colors);
	return 0;
}

UINT TImage::setRadialGradient(TDataArray<D2D1_GRADIENT_STOP>& colors)
{
	if (!cover.get())
		cover = new TContent(renderTarget, this);
	cover->SetRadialImage(colors);
	return 0;
}


UINT TImage::rotate(CPoint& point)
{
	CPoint center((location.left + location.right) / 2, (location.bottom + location.top) / 2);
	double degrees = atan2(-(static_cast<double>(point.y) - center.y),
		(static_cast<double>(point.x) - center.y));
	rotation = D2D1::Matrix3x2F::Rotation(degrees,D2D1::Point2F(center.x, center.y));
	return 0;
}

void TImage::onDraw(TObject* obj)
{
	if (!renderTarget.get()) return;
	renderTarget->SetTransform(rotation);
	TControl::onDraw(obj);
	renderTarget->SetTransform(identityMatrix);
}

void MarkPixelsRoot(CPoint& startPoint, UINT idealPixel, float tolerance, TDataArray<TDataArray<PixelMark>>& pixels)
{
	if (!pixels.Size() || startPoint.x >= pixels[0].Size() || startPoint.y >= pixels.Size())
		return;

	UINT startPixel = pixels[startPoint.y][startPoint.x].pixel;

	// pixels[startPoint.y][startPoint.x].marker == 2;

	MarkPixels(startPoint, startPixel, tolerance, pixels);
}

void MarkPixels(CPoint& startPoint, UINT startPixel, float tolerance, TDataArray<TDataArray<PixelMark>>& pixels)
{
	// If this pixel is out of bounds, then don't analyze it
	if (!pixels.Size() || startPoint.x >= pixels[0].Size() || startPoint.y >= pixels.Size())
		return;

	// If this pixel has already been looked at, don't look at it again
	if (pixels[startPoint.y][startPoint.x].pixel) return;

	if (ColorsMatch(startPixel, pixels[startPoint.y][startPoint.x].pixel, tolerance))
	{
		pixels[startPoint.y][startPoint.x].pixel = 2;

		MarkPixels(CPoint(startPoint.x + 1, startPoint.y), startPixel, tolerance, pixels);
		MarkPixels(CPoint(startPoint.x - 1, startPoint.y), startPixel, tolerance, pixels);
		MarkPixels(CPoint(startPoint.x, startPoint.y + 1), startPixel, tolerance, pixels);
		MarkPixels(CPoint(startPoint.x, startPoint.y - 1), startPixel, tolerance, pixels);
	}
	else
	{
		pixels[startPoint.y][startPoint.x].pixel = 1;
	}
}

void ConvertD2D1ColorToUIntColor(D2D1_COLOR_F& d2dColor, UINT& uColor)
{
	uColor = 0;
	
	UINT tempColor = static_cast<UINT>(d2dColor.a * 255);
	uColor += (tempColor);

	tempColor = static_cast<UINT>(d2dColor.b * 255);
	uColor += (tempColor << 8);

	tempColor = static_cast<UINT>(d2dColor.g * 255);
	uColor += (tempColor << 16);

	tempColor = static_cast<UINT>(d2dColor.b * 255);
	uColor += (tempColor << 24);
}

bool ColorsMatch(UINT color1, UINT color2, float tolerance)
{
	return false;
}
