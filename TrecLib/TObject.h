#pragma once
//#define _AFXDLL
#include "afx.h"
#include <new>
#include <memory>
#include "TrecLib.h"

extern UCHAR TArrayType[];
extern UCHAR TDataArrayType[];
extern UCHAR TMapType[];

/*
* Class TObject
* Serves as the base object for the AnaGame platform
*/
class _TREC_LIB_DLL TObject 
{
public:
	TObject();
	virtual ~TObject();
	LPCTSTR* getType();

	unsigned char* alloStack(int);
	void deAlloStack();
	unsigned char* getStack();
	int getStackSize();

	virtual UCHAR* GetAnaGameType();

protected:
	LPCTSTR* sys_Type;
	unsigned char* startStack;
	int stackSize;
	unsigned char* endStack;
};


 _TREC_LIB_DLL WCHAR* boolToString(bool);
