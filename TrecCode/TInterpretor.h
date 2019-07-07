#pragma once
#include <TObject.h>
#include "VariableList.h"
#include "VariableTree.h"
#include <TFile.h>
// #include "IntLanguage.h"
#include "TrecCode.h"

class IntLanguage; // Declare a class called IntLanguage, the source file can use it's actual speficiations

// Message that running code can send back to the interpretor to know when it is time to stop, restart from the beginning, or some other action
typedef enum InterpretorMessage
{
	im_run,
	im_break,		// Stops the interpretor without returning a value to the parent interpretor
	im_continue,	// Causes the interpretor to restart
	im_return		// Stops the interpretor and returns a value to the parant interpretor
}InterpretorMessage;

// 
typedef enum InterpretorResource
{
	ir_none,
	ir_file,
	ir_string
}InterpretorResource;

// Used when filtering-out comments for simpler code parsing
typedef enum CodeMode
{
	cm_reg,		// currently in regular code
	cm_sinStr,	// currently in a single-line String
	cm_mulStr,	// currently in a multi-line String
	cm_sinCom,	// currently in a single-line comment
	cm_mulCom	// currently in a multi-line comment
}CodeMode;

// Used when filtering out comments to know when a given string is being generated and where in the file that string begins
typedef struct DoubIndex
{
	int strInd;
	TString stringQ;
} DoubIndex;

// Data types used for handling 
typedef struct Parameter
{
	TString paramType; // Type of the parameter. For implicit typed-languages, this should be left empty
	TString paramName; // What the funtion/method interpretor refers to the params as (as opposed to what the caller does)
}Parameter;

class _TREC_CODE_DLL TInterpretor :
	public TObject
{
	
public:
	TInterpretor();
	~TInterpretor();

	TInterpretor(TInterpretor* ti);
	TInterpretor(IntLanguage* lang);

	bool SetFile(TrecPointer<TFile> file);
	bool SetFile(TrecPointer<TFile> file, ULONG seek, UINT line, ULONG64 end = 0);
	void SetParams(TString& params, WCHAR paramDivider);
	void setLanguage(IntLanguage* lang);

	void SetString(TString& strCode);

	void SetGlobalVariables(VariableContainer* vt);

	void SendFlowMessage(InterpretorMessage, intVariable* ret);
	void setLine(UINT line);
	intVariable* GetVariable(TString& name);

	UINT Run();
	UINT Run(TInterpretor* t); // Should be called On Interpretors assigned control blocks
	UINT Run(TInterpretor* t, VariableList& parameters); // Should be called on Interpretors assigned to functions and methods


protected:

	// Variables available to the Interpretor
	VariableTree localVariables;
	VariableContainer* globalVariables;

	TDataArray<Parameter> parameters;

	TInterpretor* parent;
	IntLanguage* language;


	TString singleComment;				// Comments that end when the next line begins
	TString startComment, endComment;   // Multi-line comments;
	TDataArray<TString> string, multiLineString;



	TrecPointer<TFile> sourceFile;
	TString sourceString;
	ULONG64 fileLoc;
	ULONG64 fileLocEnd; // Where the Interpretor is supposed to stop reading (0 if end of file)
	UINT startLine;

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

