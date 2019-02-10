#include "stdafx.h"
#include "SourceCodeApp.h"
#include <DirectoryInterface.h>
#include <AnafaceParser.h>
#include <TML_Reader_.h>
#include <EnvManager.h>
#include "AnaGame_BuilderDoc.h"
#include <IntLanguage.h>

static UINT scCount = 0;

SourceCodeApp::SourceCodeApp(TrecPointer<AnafaceUI> m, TrecPointer<AnafaceUI>o, TrecPointer<AnafaceUI> e, CAnaGameBuilderDoc* r, HWND w) :BuilderApp(m, o, e, r, w),
	col_keyword(D2D1::ColorF::Blue), col_type_id(D2D1::ColorF::LightBlue), col_var_id(D2D1::ColorF::Gray), col_fun_id(D2D1::ColorF::Gold)
{
	// Time to make sure parameters are valid
	if (!m.get() || !o.get() || !w || !e.get())
		return;

	// Start interpret as null so was don't have dangling pointer
	interpret = nullptr;

	// Now set the structure to link the listeners to their text name
	eventNameID enid;


	TString fileBase;
	fileBase = GetDirectory(cd_Executable);
	TString fileBody = fileBase;
	fileBody += L"\\Resources\\LineTextEditor.txt";

	if (!m.get())
		return;
	TrecComPointer<ID2D1RenderTarget> ren = m->getRenderTarget();
	if (!ren.get())
		return;
	TFile file;
	bool fileOpened = file.Open(fileBody, CFile::modeRead);
	if (!fileOpened)
		return;

	// Now Create THe Anaface that lays it out
	TString directory = file.GetFilePath();
	TString fileName = file.GetFileName();
	int ind = directory.Find(fileName, 0);
	if (ind > 0)
		directory.Delete(ind, fileName.GetLength());

	AnafaceParser* aParse = new AnafaceParser(ren, w, directory);
	aParse->setEventSystem(handleList);

	TML_Reader_* reader = new TML_Reader_(&file, aParse);
	int readingError = 0;
	if (!reader->read(&readingError))
	{
		file.Close();
		delete reader;
		delete aParse;
		return;
	}
	mainPage = aParse->getRootControl();
	if (mainPage.get())
	{
		TString name;
		name.Format(L"Code %d", scCount++);
		m->addControl(mainPage, name);
		mainPage->onCreate(m->GetControlArea());
	}

	file.Close();
	delete reader;
	delete aParse;
}



SourceCodeApp::~SourceCodeApp()
{
}

bool SourceCodeApp::InitializeControls()
{
	try
	{
		TrecPointer<TLayout> lay = dynamic_cast<TLayout*>(mainPage.get());
		if (!lay.get())
			return false;
		lines = dynamic_cast<TTextField*>(lay->GetLayoutChild(0, 0).get());
		code = dynamic_cast<TTextField*>(lay->GetLayoutChild(1, 0).get());
		return lines.get() && code.get();
	}
	catch (std::bad_cast& e)
	{

	}
	return false;
}

void SourceCodeApp::SetContents(TFile & content)
{
	if (!content.IsOpen())
		return;
	if (code.get())
	{
		TString strContent, line;
		while (content.ReadString(line))
		{
			if (strContent.GetLength())
				strContent += L'\n';
			strContent += line;
		}

		code->SetText(strContent);

	}

	filePath = content.GetFilePath();

	PrepLanguage(content);
	//content.Close();
}

void SourceCodeApp::OnSave()
{
	if (!code.get())
		return;
	if (!filePath.GetLength())
	{
		CFileDialog cfd(FALSE);
		if (cfd.DoModal() == IDCANCEL)
		{
			return;
		}
		filePath = cfd.GetPathName();
	}

	TFile file;
	try
	{
		file.Open(filePath, CFile::modeWrite | CFile::modeCreate);
	}
	catch (CFileException& e)
	{
		file.Open(filePath, CFile::modeWrite | CFile::modeNoTruncate);
	}
	if (!file.IsOpen())
		return;

	TString write = code->GetText();
	file.WriteString(write);
	

	PrepLanguage(file);

	file.Close();

}

void SourceCodeApp::OnShow()
{
	if (ref) ref->ShowLanguagePanel(interpret);
}

void SourceCodeApp::onHide()
{
	if (ref) ref->ShowLanguagePanel(false);
}

UINT SourceCodeApp::ProcessIntLanguage()
{
	TFile sourceCode;
	CFileException ex;
	sourceCode.Open(filePath, CFile::modeRead, &ex);
	if (!sourceCode.IsOpen())
	{
		TCHAR buff[200];
		ex.GetErrorMessage(buff, 200);
		return 1;
	}
	if (!interpret)
		return 2;
	interpret->SetFile(sourceCode);
	return 0;
}

void SourceCodeApp::PrepLanguage(TFile & sourceCode)
{

	checkLanguages();

	if (!sourceCode.IsOpen())
		return;


	TString ext = sourceCode.GetFileExtension();

	sourceCode.Close();

	bool found = false;

	TString languageName = retrieveLanguageByExtension(ext);

	if (!languageName.GetLength())
		return;
	IntLanguage* lang = IntLanguage::getLanguage(languageName);

	if (!lang)
		return;

	this->interpret = new TInterpretor(lang);
	OnShow();
}
