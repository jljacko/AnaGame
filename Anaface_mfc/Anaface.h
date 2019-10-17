#pragma once
#ifndef _ANAFACE_DLL
#define _ANAFACE_DLL __declspec(dllexport)
#else
#define _ANAFACE_DLL __declspec(dllimport)
#endif