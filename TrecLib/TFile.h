#pragma once
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
class _TREC_LIB_DLL TFile : public TObject
{
public:
	TFile();
	TFile(TString& lpszFileName,
		UINT nOpenFlags);
	~TFile();

	// Methods that override CFile's Methods
	bool Open(TString& lpszFileName,
		UINT nOpenFlags);


	// Methods Imported from CArchive
	
	BOOL ReadString(TString& rString);
	UINT ReadString(TString& rString, UINT nMax);

	UINT ReadString(TString& rString, WCHAR chara);
	
	void WriteString(TString& lpsz);
	bool IsOpen();
	bool SetEncoding(FileEncodingType fet);
	TString GetFileDirectory();
	virtual UCHAR* GetAnaGameType() override;

	TString GetFileExtension();

	void Close();
	void Flush();
	TString GetFileName();
	TString GetFilePath();
	TString GetFileTitle();
	ULONGLONG GetLength();
	ULONGLONG GetPosition();

	UINT Read(void* buffer, UINT count);
	ULONGLONG Seek(LONGLONG offset, UINT from);
	void SeekToBegin();
	ULONGLONG SeekToEnd();
	void Write(const void* buffer, UINT count);

protected:
	FileEncodingType DeduceEncodingType();
private:
	TString filePath;
	FileEncodingType fileEncode;
	HFILE fileHandle;
	ULONGLONG position;
};

