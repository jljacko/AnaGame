#pragma once
#include "TrecPointerI.h"

/*
* class TrecPointerBase
* Serves as the base class for all TrecPointers and interface between applications and the TrecPointer
*	Registry
*/
class _TREC_LIB_DLL TrecPointerBase
{
public:
	TrecPointerBase();
	TrecPointerBase(TrecPointerBase& copy/*, const char* type = nullptr*/);
	~TrecPointerBase();

	void clean();
	void Boost();

private:
	unsigned char timeCount;
	UINT index;
	bool dontDeRef;
protected:
	void IncrementCount(void* ref, const char* = nullptr);
	void IncrementCount(const char* = nullptr);
	void* DecrementCount(const char* = nullptr);
	void* NukeCount(const char* = nullptr);
	void SetTrecPointer(TrecPointerBase& base);
	void * Get();
	void nullify();
	void init(void* ref, const char* = nullptr);
	void setHold();
	bool getHold();
	void resetHold();

	void stopDeRef();
	void enableDeRef();
	bool getRefStatus();
	//void* validateReference();
};

