#pragma once
#include "stdafx.h"
#include "TrecLib.h"
#include "TString.h"

/*
* enum FileEncodingType
* used to track what type of encoding the file is using
*/
typedef enum FileEncodingType
{
	fet_unknown,          // We do not know the encoding of this file
	fet_acsii,            // The encoding is acsii
	//fet_acsii16,          // The encoding is acsii-16
	fet_unicode,          // The encoding is Unicode
	fet_unicode_little,   // The encoding is Unicode - little endian
	fet_unicode8,         // The encoding is Unicode-8
	fet_unicode32,        // The encoding is Unicode-32
	fet_unicode32_little, // The encoding is Unicode-32 - little endian
	fet_unicode7          // The Encoding is UTF-7
}FileEncodingType;

/*
 * Class: TFile
 * Provides AnaGame's implementation of the File Class
*/
class _TREC_LIB_DLL TFile : public CFile, public TObject
{
public:
	TFile();
	TFile(LPCTSTR lpszFileName,
		UINT nOpenFlags);
	TFile(TString& file, UINT nOpenFlags);
	~TFile();

	// Methods that override CFile's Methods
	BOOL Open(
		LPCTSTR lpszFileName,
		UINT nOpenFlags,
		CFileException* pError = NULL) override;

	BOOL Open(TString& file, UINT flags, CFileException* pError = NULL);

	virtual BOOL Open(
		LPCTSTR lpszFileName,
		UINT nOpenFlags,
		CAtlTransactionManager* pTM,
		CFileException* pError = NULL)override;

	// Methods Imported from CArchive
	
	BOOL ReadString(CString& rString);
	UINT ReadString(CString& rString, UINT nMax);
	LPTSTR ReadString(LPTSTR lpsz, UINT nMax);
	void WriteString(LPCTSTR lpsz);
	bool IsOpen();
	bool SetEncoding(FileEncodingType fet);
	TString GetFileDirectory();
	virtual UCHAR* GetAnaGameType() override;

protected:
	FileEncodingType DeduceEncodingType();
private:

	FileEncodingType fileEncode;
};

