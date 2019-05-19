#pragma once
#include <TObject.h>
#include "VariableList.h"
#include "VariableTree.h"
#include <TFile.h>
// #include "IntLanguage.h"
#include "TrecCode.h"

class IntLanguage;

typedef enum InterpretorMessage
{
	im_run,
	im_break,
	im_continue,
	im_return
}InterpretorMessage;

typedef enum InterpretorResource
{
	ir_none,
	ir_file,
	ir_string
}InterpretorResource;

typedef enum CodeMode
{
	cm_reg,		// currently in regular code
	cm_sinStr,	// currently in a single-line String
	cm_mulStr,	// currently in a multi-line String
	cm_sinCom,	// currently in a single-line comment
	cm_mulCom	// currently in a multi-line comment
}CodeMode;

typedef struct DoubIndex
{
	int strInd;
	TString stringQ;
} DoubIndex;

class _TREC_CODE_DLL TInterpretor :
	public TObject
{
	
public:
	TInterpretor();
	~TInterpretor();

	TInterpretor(TInterpretor* ti);
	TInterpretor(IntLanguage* lang);

	bool SetFile(TFile& file);
	bool SetFile(TFile& file, ULONG seek);
	void setLanguage(IntLanguage* lang);

	void SetString(TString& strCode);

	void SetGlobalVariables(VariableTree* vt);

	void SendFlowMessage(InterpretorMessage, intVariable* ret);
	intVariable* GetVariable(TString& name);

	UINT Run();
	UINT Run(TInterpretor* t);
	UINT Run(TInterpretor* t, VariableList& parameters);
	UINT Run(TInterpretor* t, TString& arguements);

protected:

	// Variables available to the Interpretor
	VariableTree localVariables;
	VariableTree* globalVariables;

	TString parameterNames;

	TInterpretor* parent;
	IntLanguage* language;


	TString singleComment;				// Comments that end when the next line begins
	TString startComment, endComment;   // Multi-line comments;
	TDataArray<TString> string, multiLineString;



	TFile sourceFile;
	TString sourceString;
	ULONG64 fileLoc;

	intVariable* returnValue;
	InterpretorMessage actionMode;
	InterpretorResource resource;

	void UpdateDoubIndex(TString&buff, UINT index, DoubIndex& sinCom, DoubIndex&  newLine, DoubIndex& mulCom,
		DoubIndex& mulComE, DoubIndex& sinStr, DoubIndex& mulStr);

	DoubIndex getNextSingleComment(TString& buff, UINT index);
	DoubIndex getNextNewline(TString& buff, UINT index);
	DoubIndex getNextMultiComment(TString& buff, UINT index);
	DoubIndex getNextMultiCommentEnd(TString& buff, UINT index);
	DoubIndex getNextSingleString(TString& buff, UINT index);
	DoubIndex getNextMultiString(TString& buff, UINT index);

	UINT GetNextStatement(TString& statement, ULONGLONG& startSeek);
	CodeMode EndsAsString(TString& statement, CodeMode sourceMode, UINT start);

	/*DoubIndex getSingleString(TString& read, UINT in);
	DoubIndex getMultiString(TString& read, UINT in);
	int getStringEnd(TString& read, UINT in, UINT);
	int getMulStringEnd(TString& read, UINT in, UINT dIn);
	int getSingleComment(TString& read, UINT in);
	int getMultiComment(TString& read, UINT in, bool);
	int getNextLine(TString& read, UINT in);

	bool setStringMode(CodeMode& sourceMode, UINT& readData, DoubIndex& nextSinStr, DoubIndex& nextMulStr);*/
};

