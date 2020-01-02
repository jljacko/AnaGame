#pragma once


#ifndef _TREC_CODE_DLL
#define _TREC_CODE_DLL __declspec(dllexport)
#else
#define _TREC_CODE_DLL __declspec(dllimport)
#endif