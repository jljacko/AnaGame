#include "stdafx.h"
#include "SourceCodeApp.h"
#include <DirectoryInterface.h>
#include <AnafaceParser.h>
#include <TML_Reader_.h>

static UINT scCount = 0;

SourceCodeApp::SourceCodeApp(TrecPointer<AnafaceUI> m, TrecPointer<AnafaceUI>o, TrecPointer<AnafaceUI> e, HWND w) :BuilderApp(m, o, e, w),
	col_keyword(D2D1::ColorF::Blue), col_type_id(D2D1::ColorF::LightBlue), col_var_id(D2D1::ColorF::Gray), col_fun_id(D2D1::ColorF::Gold)
{
	// Time to make sure parameters are valid
	if (!m.get() || !o.get() || !w || !e.get())
		return;

	// Now set the structure to link the listeners to their text name
	eventNameID enid;


	TString fileBase;
	fileBase = GetDirectory(cd_AppData);
	TString fileBody = fileBase;
	fileBody += L"\\AnaGame\\LineTextEditor.txt";

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
