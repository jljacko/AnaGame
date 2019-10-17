#pragma once
#ifndef _TAP_DLL
#define _TAP_DLL __declspec(dllexport)
#else
#define _TAP_DLL __declspec(dllimport)
#endif