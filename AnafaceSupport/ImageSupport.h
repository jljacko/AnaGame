#pragma once

#include <TFile.h>
#include <TrecComPointer.h>
#include <d2d1_1.h>

int SaveImageAsJPEG(TFile& file,TrecComPointer<ID2D1Bitmap> map);
int SaveImageAsPNG(TFile& file, TrecComPointer<ID2D1Bitmap> map);
int SaveImageAsGIF(TFile& file, TrecComPointer<ID2D1Bitmap> map);