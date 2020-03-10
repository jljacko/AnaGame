#include "TBitmapBrush.h"
#include <wincodec.h>
#include "DrawingBoard.h"
#include "TGeometry.h"

void TBitmapBrush::NextFrame()
{
	if (!valid)
		return;
	currentFrame++;
	if (currentFrame >= brushes.Size())
		currentFrame = 0;

	brush = brushes[currentFrame];
}

void TBitmapBrush::PrevFrame()
{
	if (!valid)return;
	if (!currentFrame)
		currentFrame = brushes.Size() - 1;
	else
		currentFrame--;
	brush = brushes[currentFrame];
}

bool TBitmapBrush::IsValid()
{
	return valid;
}

void TBitmapBrush::SetLocation(RECT_2D& loc)
{
	location = loc;
	RefreshBrush();
}

UINT TBitmapBrush::GetCurrentFrame()
{
	return currentFrame;
}

TBitmapBrush::~TBitmapBrush()
{
	SAFE_RELEASE(imageFactory)
	SAFE_RELEASE(decoder)
	SAFE_RELEASE(frameDec)
	SAFE_RELEASE(converter)

	brushes.RemoveAll();
	bitmaps.RemoveAll();
}

void TBitmapBrush::FillRectangle(const RECT_2D& r)
{
	if (Refresh() && currentFrame < bitmaps.Size() && bitmaps[currentFrame].Get() 
		&& currentFrame < brushes.Size() && brushes[currentFrame].Get())
	{
		currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), r);
	}
}

void TBitmapBrush::FillRoundedRectangle(const ROUNDED_RECT_2D& r)
{
	if (Refresh() && currentFrame < bitmaps.Size() && bitmaps[currentFrame].Get()
		&& currentFrame < brushes.Size() && brushes[currentFrame].Get())
	{
		if (!board->AddLayer(r))
			return;
		currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), r.rect);
		board->PopLayer();
	}
}

void TBitmapBrush::FillEllipse(const ELLIPSE_2D& r)
{
	if (Refresh() && currentFrame < bitmaps.Size() && bitmaps[currentFrame].Get()
		&& currentFrame < brushes.Size() && brushes[currentFrame].Get())
	{
		if(!board->AddLayer(r))
			return;
		RECT_2D rect = {
			r.point.x - r.radiusX,
			r.point.y - r.radiusY,
			r.point.x + r.radiusX,
			r.point.y + r.radiusY
		};
		currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), rect);
		board->PopLayer();
	}
}

void TBitmapBrush::FillGeometry(TrecPointer<TGeometry> geo)
{
	if (geo.Get() && Refresh() && currentFrame < bitmaps.Size() && bitmaps[currentFrame].Get()
		&& currentFrame < brushes.Size() && brushes[currentFrame].Get())
	{
		RECT_2D rect;
		if (!geo->GetBounds(rect))
			return;
		if (!board->AddLayer(geo))
			return;
	
		currentRenderer->DrawBitmap(bitmaps[currentFrame].Get(), rect);
		board->PopLayer();
	}
}


TBitmapBrush::TBitmapBrush(TrecPointer<TFileShell> picture, TrecPointer<DrawingBoard> rt, RECT_2D& loc): TBrush(rt)
{
	valid = false;

	brushType = brush_type_bitmap;

	if (!picture.Get())
		return;

	imageFactory = NULL;
	decoder = NULL;
	frameDec = NULL;
	converter = NULL;
	IWICBitmapScaler* scale = nullptr;
	UINT frameCount = 0;
	HRESULT result = CoCreateInstance(CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&imageFactory);
	if (!SUCCEEDED(result))
		return;

	result = imageFactory->CreateDecoderFromFilename(picture->GetPath().GetConstantBuffer(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder);


	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		return;
	}

	result = imageFactory->CreateFormatConverter(&converter);
	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		return;
	}

	result = imageFactory->CreateBitmapScaler(&scale);

	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		converter->Release();
		return;
	}


	decoder->GetFrameCount(&frameCount);
	for (UINT Rust = 0; Rust < frameCount && SUCCEEDED(decoder->GetFrame(Rust, &frameDec)); Rust++)
	{
		UINT h = 0, w = 0;

		frameDec->GetSize(&w, &h);

		result = scale->Initialize(frameDec, loc.right - loc.left, loc.bottom - loc.top,
			WICBitmapInterpolationModeFant);

		if (SUCCEEDED(result))
		{
			converter->Initialize(scale, GUID_WICPixelFormat32bppPRGBA,
				WICBitmapDitherTypeNone, NULL, 0.0f,
				WICBitmapPaletteTypeMedianCut);
			TrecComPointer<ID2D1Bitmap>::TrecComHolder map;
			rt->GetRenderer()->CreateBitmapFromWicBitmap(converter, map.GetPointerAddress());
			TrecComPointer<ID2D1Bitmap> bitmap = map.Extract();

			if (bitmap.Get())
			{
				bitmaps.push_back(bitmap);
				TrecComPointer<ID2D1BitmapBrush>::TrecComHolder bitHolder;
				rt->GetRenderer()->CreateBitmapBrush(bitmap.Get(), bitHolder.GetPointerAddress());
				brushes.push_back(TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1BitmapBrush>(bitHolder));
			}
		}
		scale->Release();
		imageFactory->CreateBitmapScaler(&scale);
	}
	scale->Release();
	if (brushes.Size())
	{
		brush = brushes[0];
		valid = true;
	}
	location = loc;
}

void TBitmapBrush::RefreshBrush()
{
	brushes.RemoveAll();
	bitmaps.RemoveAll();

	if (!valid)
		return;
	HRESULT result;
	IWICBitmapScaler* scale = nullptr;
	result = imageFactory->CreateBitmapScaler(&scale);
	UINT frameCount = 0;
	decoder->GetFrameCount(&frameCount);

	for (UINT Rust = 0; Rust < frameCount && SUCCEEDED(decoder->GetFrame(Rust, &frameDec)); Rust++)
	{
		UINT h = 0, w = 0;

		frameDec->GetSize(&w, &h);

		result = scale->Initialize(frameDec, location.right - location.left, location.bottom - location.top,
			WICBitmapInterpolationModeFant);

		if (SUCCEEDED(result))
		{
			converter->Initialize(scale, GUID_WICPixelFormat32bppPRGBA,
				WICBitmapDitherTypeNone, NULL, 0.0f,
				WICBitmapPaletteTypeMedianCut);
			TrecComPointer<ID2D1Bitmap>::TrecComHolder map;
			currentRenderer->CreateBitmapFromWicBitmap(converter, map.GetPointerAddress());
			TrecComPointer<ID2D1Bitmap> bitmap = map.Extract();

			if (bitmap.Get())
			{
				bitmaps.push_back(bitmap);
				TrecComPointer<ID2D1BitmapBrush>::TrecComHolder bitHolder;
				currentRenderer->CreateBitmapBrush(bitmap.Get(), bitHolder.GetPointerAddress());
				brushes.push_back(TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1BitmapBrush>(bitHolder));
			}
		}
		scale->Release();
		imageFactory->CreateBitmapScaler(&scale);
	}
	scale->Release();
	if (currentFrame < brushes.Size())
	{
		brush = brushes[currentFrame];
		valid = true;
	}
}
