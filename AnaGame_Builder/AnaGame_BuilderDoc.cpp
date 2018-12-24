
// AnaGame_BuilderDoc.cpp : implementation of the CAnaGameBuilderDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AnaGame_Builder.h"
#endif

#include "AnaGame_BuilderDoc.h"
#include "NewProjectDialogAG.h"
#include "NewFileTDialog.h"
#include <AnafaceParser.h>
#include <TML_Reader_.h>
#include <propkey.h>
#include "ArenaApp.h"
#include "NameDialog.h"
#include "SourceCodeApp.h"
#include "ModelDialog.h"
//#include <DirectoryInterface.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAnaGameBuilderDoc

IMPLEMENT_DYNCREATE(CAnaGameBuilderDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnaGameBuilderDoc, CDocument)
END_MESSAGE_MAP()

// Found on the Home Tab
TString on_LoadNewSolution = L"LoadNewSolution";
TString on_SaveFile = L"SaveFile";
TString on_SaveAll = L"SaveAllFiles";
TString on_NewFile = L"OnNewFile";
TString on_ImportFile = L"OnImportFile";
TString on_Print = L"OnPrint";

// Found on the Arena Tab
TString on_NewArena = L"OnNewArena";
TString on_Change3DColor = L"OnUpdateClearColor";
TString on_NewModel = L"OnNewModel";

// Found on the Document (Source Code) Tab
TString on_NewCodeFile = L"OnNewCodeFile";
TString on_ImportCode = L"OnImportCode";

// CAnaGameBuilderDoc construction/destruction

CAnaGameBuilderDoc::CAnaGameBuilderDoc() : Page(), backColor(D2D1::ColorF::White)
{
	// TODO: add one-time construction code here
	CAnaGameBuilderApp* app = dynamic_cast<CAnaGameBuilderApp*>(AfxGetApp());
	
	fileAnaface = GetDirectory(cd_Executable);
	fileAnaface += L"\\Resources\\AnagameBuilderLayout.tml";

	//ID2D1Factory* rawFact = nullptr;
	init = false;
	//Page();
	D2D1_FACTORY_OPTIONS factOpts;
	ZeroMemory(&factOpts, sizeof(factOpts));
	factOpts.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	ID2D1Factory1* rawFact = nullptr;
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factOpts, &rawFact)))
		fact = nullptr;
	else
		fact = rawFact;

	// Set the Array of Action Listeners
	ideListeners[0] = &CAnaGameBuilderDoc::OnNewProject;
	ideListeners[1] = &CAnaGameBuilderDoc::OnNewFile;
	ideListeners[2] = &CAnaGameBuilderDoc::OnNewArena;
	ideListeners[3] = &CAnaGameBuilderDoc::OnUpdateClearColor;
	ideListeners[4] = &CAnaGameBuilderDoc::OnNewModel;
	ideListeners[5] = &CAnaGameBuilderDoc::OnNewCodeFile;
	ideListeners[6] = &CAnaGameBuilderDoc::OnImportCodeFile;
	ideListeners[7] = &CAnaGameBuilderDoc::OnSaveAll;
	ideListeners[8] = &CAnaGameBuilderDoc::OnSave;


	// Now set the structure to link the listeners to their text name
	eventNameID enid;

	enid.eventID = 0;
	enid.name = on_LoadNewSolution;
	idMatch.push_back(enid);

	// On Save File
	enid.eventID = 8;
	enid.name = on_SaveFile;
	idMatch.push_back(enid);
	
	// On Save All Files
	enid.eventID = 7;
	enid.name = on_SaveAll;
	idMatch.push_back(enid);

	// On New File
	enid.eventID = 1;
	enid.name = on_NewFile;
	idMatch.push_back(enid);

	// On New Arena
	enid.eventID = 2;
	enid.name = on_NewArena;
	idMatch.push_back(enid);

	// On Change 3D Background color
	enid.eventID = 3;
	enid.name = on_Change3DColor;
	idMatch.push_back(enid);

	// On New Model
	enid.eventID = 4;
	enid.name = on_NewModel;
	idMatch.push_back(enid);

	// On New Code File
	enid.eventID = 5;
	enid.name = on_NewCodeFile;
	idMatch.push_back(enid);

	// On Import Code
	enid.eventID = 6;
	enid.name = on_ImportCode;
	idMatch.push_back(enid);

	// Now Make sure that Anagame has a workspace
	TString directory = GetDirectoryWithSlash(cd_Documents);
	directory += L"AnaGame";
	DWORD fileAtt = GetFileAttributesW(directory);
	if (fileAtt == INVALID_FILE_ATTRIBUTES)
	{
		int er = GetLastError();
		if (er == ERROR_PATH_NOT_FOUND || er == ERROR_FILE_NOT_FOUND
			|| er == ERROR_INVALID_NAME || er == ERROR_BAD_NETPATH)
		{
			CreateDirectory(directory, nullptr);
		}
	}

	directory += L"\\Projects";
	fileAtt = GetFileAttributesW(directory);
	if (fileAtt == INVALID_FILE_ATTRIBUTES)
	{
		int er = GetLastError();
		if (er == ERROR_PATH_NOT_FOUND || er == ERROR_FILE_NOT_FOUND
			|| er == ERROR_INVALID_NAME || er == ERROR_BAD_NETPATH)
		{
			CreateDirectory(directory, nullptr);
		}
	}
}

CAnaGameBuilderDoc::~CAnaGameBuilderDoc()
{
	for (UINT c = 0; c < ActiveDocuments.Count(); c++)
	{
		TrecPointer<BuilderApp> ba = ActiveDocuments.RemoveAt(c);
		ba.Delete();
	}
	rootControl.Delete();
}

BOOL CAnaGameBuilderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CAnaGameBuilderDoc serialization

void CAnaGameBuilderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAnaGameBuilderDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAnaGameBuilderDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAnaGameBuilderDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAnaGameBuilderDoc diagnostics

#ifdef _DEBUG
void CAnaGameBuilderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnaGameBuilderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
void CAnaGameBuilderDoc::OnDraw(CDC * pDC)
{
	if (!init)
		InitializeBuilderView(pDC);
	if (rootControl.get() && regRenderTarget.get())
	{
	
		contextDevice->BeginDraw();
		

		RECT arenaRect = { 0,0,0,0 };
		if(currentDocument.get())
			arenaRect = currentDocument->Get3DArea();
		

		if (engine.get())
			engine->PrepareScene(backColor);

		//contextDevice->Clear(D2D1::ColorF(D2D1::ColorF::White));
		rootControl->onDraw();

		HDC contDC = 0;
		if (gdiRender.get() && SUCCEEDED(gdiRender->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &contDC)))
		{

			gdiRender->ReleaseDC(nullptr);


			if (engine.get())
				engine->FinalizeScene();
			

			RECT winLoc;
//			if (GetWindowRect(windowHandle, &winLoc))
//				InvertRect(contDC, &winLoc);
			int w = arenaRect.right - arenaRect.left;
			int h = arenaRect.bottom - arenaRect.top;
			int er = 0;
			if (!isSnipZero(arenaRect))
			{
				//RECT d3dRect = arenaControl->getSnip();
				
				BLENDFUNCTION bf;
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.AlphaFormat = 0;
				bf.SourceConstantAlpha = 0xff;
				gdiRender->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &contDC);
				//if (!AlphaBlend(contDC, arenaRect.left, arenaRect.top, w, h, deviceH, arenaRect.left, arenaRect.top, w, h, bf))
				//	er = GetLastError();
				gdiRender->ReleaseDC(nullptr);
			}

			/*if (deviceH)
			{
				BOOL b;
				gdiRender->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &contDC);
				//b = BitBlt(deviceH, 0, 0, width, height, contDC, 0, 0, SRCCOPY);
				gdiRender->ReleaseDC(nullptr);
				//else
				//{
				//	b = BitBlt(windDC, arenaRect.left, arenaRect.top, w, h, contDC,
				//		arenaRect.left, arenaRect.top, MERGEPAINT);
					
				//}
				if (!b)
				{
					int er = GetLastError();
				}
			}*/
			
			
		}
		HRESULT res = contextDevice->EndDraw();

	}
}
void CAnaGameBuilderDoc::InitializeBuilderView(CDC * pDC)
{
	if (!pDC)
		return;
	deviceH = pDC->GetSafeHdc();
	windowHandle = WindowFromDC(deviceH);

	int er = Initialize3D(TString(L"BuilderEngine"));

	if (er)
		return;

	CFile file;
	bool FileOpened = file.Open(fileAnaface.GetBuffer(), CFile::modeRead);
	fileAnaface.ReleaseBuffer();
	if (!FileOpened)
	{
		return ;
	}

	// Now Create THe Anaface that lays it out
	TString directory = file.GetFilePath();
	TString fileName = file.GetFileName();
	int ind = directory.Find(fileName, 0);
	if (ind > 0)
		directory.Delete(ind, fileName.GetLength());

	AnafaceParser* aParse = new AnafaceParser(regRenderTarget, windowHandle, &directory);
	aParse->setEventSystem(idMatch);

	CArchive *archer = new CArchive(&file, CArchive::load);

	TML_Reader_* reader = new TML_Reader_(archer, aParse);
	int readingError;
	if (!reader->read(&readingError))
	{
		archer->Close();
		delete archer;
		file.Close();
		delete reader;
		delete aParse;
		return ;
	}
	else
	{
		rootControl = aParse->getRootControl();
		if (rootControl.get())
		{
			rootControl->onCreate(area);
		}
		else
		{
			archer->Close();
			delete archer;
			file.Close();
			delete reader;
			delete aParse;
			return ;
		}
	}
	archer->Close();
	delete archer;
	file.Close();
	delete reader;
	delete aParse;
	init = true;
	InitilaizeControls();
}
#endif //_DEBUG


// CAnaGameBuilderDoc commands

void CAnaGameBuilderDoc::InitilaizeControls()
{
	TrecPointer<TLayout> rootLayout;
	try
	{
		rootLayout = dynamic_cast<TLayout*>(rootControl.get());
		TrecPointer<TControl> rib = rootLayout->GetLayoutChild(0, 0);
		ribbon = dynamic_cast<AnafaceUI*>(rib.get());
		rib = ribbon->GetChildAt(0);
		if (!rib.get())
		{

			return;
		}
		ribbon1 = dynamic_cast<TLayout*>(rib.get());
		rib = ribbon1->GetLayoutChild(0, 0); // Rib should become a stack
		TrecPointer<TLayout> ribLay;
		ribLay = dynamic_cast<TLayout*>(rib.get());
		rib = ribLay->GetLayoutChild(0, 0);// Rib should now be a TextField
		solutionName = dynamic_cast<TTextField*>(rib.get());

		rib = rootLayout->GetLayoutChild(0, 1);
		ribLay = dynamic_cast<TLayout*>(rib.get());
		rib = ribLay->GetLayoutChild(0, 0);
		classUI = dynamic_cast<AnafaceUI*>(rib.get());
		rib = ribLay->GetLayoutChild(1, 0);
		body = dynamic_cast<AnafaceUI*>(rib.get());
		rib = ribLay->GetLayoutChild(0, 1);
		outputPanel = dynamic_cast<AnafaceUI*>(rib.get());

		// Now Set up Arena Ribbon
		ribbon4 = dynamic_cast<TLayout*>(ribbon->GetChildAt(3).get());
		arenaStack1 = dynamic_cast<TLayout*>(ribbon4->GetLayoutChild(0, 0).get());
		arenaStack3 = dynamic_cast<TLayout*>(ribbon4->GetLayoutChild(2, 0).get());
		arenaStack1->setActive(false);
		arenaStack3->setActive(false);
	}
	catch (std::bad_cast&e)
	{

		return;
	}

	
}

void CAnaGameBuilderDoc::OnRButtonUp(UINT nFlags, CPoint point, messageOutput& mo)
{
}

void CAnaGameBuilderDoc::OnLButtonDown(UINT nFlags, CPoint point, messageOutput& mo)
{
	ScrollLButtonDown(nFlags, point, &mo);
	if (mo != negative)
	{
		//doUpdate(&mo);
		return;
	}



	//ClientToScreen(&point);
	mo = negative;

	if (currentDocument.get())
	{
		currentDocument->OnLButtonDown(nFlags, point, mo);
		if (mo != negative && mo != negativeUpdate)
			return;
	}
	if (rootControl.get())
		rootControl->OnLButtonDown(nFlags, point, &mo, cred);

	ResetComboBoxes();
	MessageHandler();
}

void CAnaGameBuilderDoc::OnRButtonDown(UINT nFlags, CPoint point, messageOutput& mo)
{
	if (currentDocument.get())
	{
		currentDocument->OnRButtonDown(nFlags, point, mo);
		if (mo != negative && mo != negativeUpdate)
			return;
	}

	if (rootControl.get())
		rootControl->OnRButtonDown(nFlags, point, &mo, cred);
	MessageHandler();
}

void CAnaGameBuilderDoc::OnMouseMove(UINT nFlags, CPoint point, messageOutput& mo)
{
	ScrollMouseMove(nFlags, point, &mo);
	if (mo != negative)
	{
		//doUpdate(&mo);
		return;
	}

	TFlyout* fly = TFlyout::DoDraw();
	if (fly)
	{
		fly->OnMouseMove(nFlags, point, &mo, cred);
	}

	if (currentDocument.get())
	{
		currentDocument->OnMouseMove(nFlags, point, mo);
		if (mo != negative && mo != negativeUpdate)
			return;
	}

	if (rootControl.get())
		rootControl->OnMouseMove(nFlags, point, &mo, cred);
	MessageHandler();
}

void CAnaGameBuilderDoc::OnLButtonDblClk(UINT nFlags, CPoint point, messageOutput& mo)
{
	//messageOutput mo = positiveContinue;
	if (currentDocument.get())
	{
		currentDocument->OnLButtonDblClk(nFlags, point, mo);
		if (mo != negative && mo != negativeUpdate)
			return;
	}

	if (rootControl.get())
		rootControl->OnLButtonDblClk(nFlags, point, &mo, cred);
	MessageHandler();
}

void CAnaGameBuilderDoc::OnLButtonUp(UINT nFlags, CPoint point, messageOutput& mo)
{
	//messageOutput mo = negative;

	ScrollLButtonUp(nFlags, point, &mo);
	if (mo != negative)
	{
		return;
	}
	mo = positiveContinue;
	TFlyout* fly = TFlyout::DoDraw();
	if (fly)
	{
		fly->OnLButtonUp(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
		{
			fly->Hide();
			//thisDoc->SetRightClick(CPoint(0, 0));


		}
	}

	if (currentDocument.get())
	{
		currentDocument->OnLButtonUp(nFlags, point, mo);
		if (mo != negative && mo != negativeUpdate)
			return;
	}

	if (rootControl.get())
		rootControl->OnLButtonUp(nFlags, point, &mo, cred);
}

void CAnaGameBuilderDoc::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput& mo)
{
	//messageOutput mo = positiveContinue;

	if (currentDocument.get())
	{
		currentDocument->OnChar(nChar,nRepCnt, nFlags, mo);
		if (mo != negative && mo != negativeUpdate)
			return;
	}

	if (rootControl.get())
		rootControl->OnChar(true, nChar, nRepCnt, nFlags, &mo, cred);
	MessageHandler();
}

void CAnaGameBuilderDoc::OnSize(UINT nType, int cx, int cy)
{
	Page::OnSize(nType, cx, cy);
	if (rootControl.get())
		rootControl->Resize(area);
}

void CAnaGameBuilderDoc::MessageHandler()
{
	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < cred.Size(); c++)
	{
		tc = cred.at(c).control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < ANAGAME_IDE_METHOD_COUNT)
		{
			// call method
			if (ideListeners[e_id])
				(this->*ideListeners[e_id])(tc, ea);
		}
	}

	//onDraw();
	cred.RemoveAll();
	this->UpdateAllViews(nullptr);
}

void CAnaGameBuilderDoc::OnNewProject(TControl * tc, EventArgs ea)
{
	TrecPointer<NewProjectDialogAG> diag;
	diag = new NewProjectDialogAG(currentSolution);
	int error = 0;
	if (!diag->InitializeWindow(error))
		return;
	if (!diag->Initialize2D(error))
		return;
	if (!diag->InitializeAnaface(error))
		return;
	diag->InitializeControls();
	diag->DoModal();

	if (diag->GetOkay())
	{
		TrecPointer<Solution> sol = diag->GetSolution();
		if (sol.get() != currentSolution.get())
		{
			// To-Do: Handle situation
			//if(sol.get())
			//	sol->SaveFile()

			// End To-Do
			if (solutionName.get())
			{
				solutionName->SetText(sol->GetName());
			}


			// Now Save New Solution
			TString directory = GetDirectoryWithSlash(cd_Documents);
			directory += L"AnaGame\\Projects\\";

			if (currentSolution.get())
			{
				TString directory2 = directory;
				directory2 += currentSolution->GetName();
				DWORD fileAtt = GetFileAttributesW(directory2);
				if (fileAtt == INVALID_FILE_ATTRIBUTES)
				{
					int er = GetLastError();
					if (er == ERROR_PATH_NOT_FOUND || er == ERROR_FILE_NOT_FOUND
						|| er == ERROR_INVALID_NAME || er == ERROR_BAD_NETPATH)
					{
						CreateDirectory(directory2, nullptr);
					}
					else
					{
						// To-Do: Handle error
					}
				}

				TString fileSol = directory2;
				fileSol += L"\\";
				fileSol += sol->GetName();
				fileSol += L".agsol";
				//CreateDirectory(directory, nullptr);
				TFile file(fileSol, CFile::modeWrite);
				if (file.IsOpen())
				{
					//file.SetEncoding(fet_unknown);
					sol->SaveFile(file);
				}
			}


			directory += sol->GetName();
			DWORD fileAtt = GetFileAttributesW(directory);
			if (fileAtt == INVALID_FILE_ATTRIBUTES)
			{
				int er = GetLastError();
				if (er == ERROR_PATH_NOT_FOUND || er == ERROR_FILE_NOT_FOUND
					|| er == ERROR_INVALID_NAME || er == ERROR_BAD_NETPATH)
				{
					// Time to create the directory 
					TString fileSol = directory;
					fileSol += L"\\";
					fileSol += sol->GetName();
					fileSol += L".agsol";
					CreateDirectory(directory, nullptr);
					TFile file;
					file.Open(fileSol, CFile::modeWrite | CFile::modeCreate);
					if (file.IsOpen())
					{
						//file.SetEncoding(fet_unicode);
						sol->SaveFile(file);
					}
					else
					{
						er = GetLastError();
						return;
					}
				}
			}
		}
		else if (sol.get()) // Here, we are adding a project to the current solution
		{

		}
	}


	//delete diag;
}

void CAnaGameBuilderDoc::OnNewFile(TControl * tc, EventArgs ea)
{
	TrecPointer<NewFileTDialog> diag;
	diag = new NewFileTDialog();
	int error = 0;
	if (!diag->InitializeWindow(error))
		return;
	if (!diag->Initialize2D(error))
		return;
	if (!diag->InitializeAnaface(error))
		return;
	diag->InitializeControls();
	diag->DoModal();
}

void CAnaGameBuilderDoc::OnNewArena(TControl * tc, EventArgs ea)
{
	TString name;
	// To-Do: Set up means through which a name is uncovered, via TDialog
	TrecPointer<NameDialog> diag;
	diag = new NameDialog();
	int error = 0;
	if (!diag->InitializeWindow(error))
		return;
	if (!diag->Initialize2D(error))
		return;
	if (!diag->InitializeAnaface(error))
		return;
	diag->InitializeControls();
	diag->DoModal();

	name = diag->GetName();

	// end to-do

	if (!name.Compare(L""))
		return;

	for (UINT c = 0; c < ActiveDocuments.Count(); c++)
	{
		try
		{
			ArenaApp* aa = dynamic_cast<ArenaApp*>(ActiveDocuments.ElementAt(c).get());
			if (aa && aa->GetArenaName() == name)
			{
				currentDocument = ActiveDocuments.ElementAt(c);
				TrecPointer<TControl> mainP = currentDocument->GetMainPage();
				if(mainP.get())
					body->addControl(mainP, TString());
				return;
			}
		}
		catch (std::bad_cast& e)
		{

		}
	}

	// To Do: Add Mechanism through which an Arena saved on Disk could be retrieved



	// End To-Do

	// Now We can create a new ArenaApp
	ArenaApp* aa = new ArenaApp(body, outputPanel, classUI, windowHandle);
	aa->SetArenaName(name);

	aa->InitializeControls();
	TrecPointer<TControl> tp_tc = aa->GetMainPage();

	if (!tp_tc.get())
		return;

	BuilderApp* ba = aa;
	TrecPointer<BuilderApp> tp_ba;
	tp_ba = ba;
	ActiveDocuments.Add(tp_ba);
//	body->addControl(tp_tc);
	currentDocument = tp_ba;
	if (arenaStack1.get())
		arenaStack1->setActive(true);
	if (arenaStack3.get())
		arenaStack3->setActive(true);
}

void CAnaGameBuilderDoc::OnUpdateClearColor(TControl * tc, EventArgs ea)
{
	if (!currentDocument.get())
		return;
	CColorDialog c;
	if (c.DoModal() == IDOK)
	{
		COLORREF co = c.GetColor();
		D2D1::ColorF _3DColor(D2D1::ColorF::White);
		_3DColor.b = static_cast<float>(GetBValue(co)) / 255.0f;
		_3DColor.r = static_cast<float>(GetRValue(co)) / 255.0f;
		_3DColor.g = static_cast<float>(GetGValue(co)) / 255.0f;
		_3DColor.a = 1.0f;
		try
		{
			ArenaApp* aa = dynamic_cast<ArenaApp*>(currentDocument.get());
			if (aa)
				aa->SetColor(_3DColor);
		}
		catch (std::bad_cast& e)
		{

		}
	}
}

void CAnaGameBuilderDoc::OnNewModel(TControl * tc, EventArgs)
{
	ArenaApp* aa = nullptr;
	try {
		aa = dynamic_cast<ArenaApp*>(currentDocument.get());
	}
	catch (std::bad_cast& e)
	{
		return;
	}
	if (!aa)
		return;

	TrecPointer<ArenaEngine> ae = aa->GetArenaEngine();
	if (!ae.get())
		return;

	TrecPointer<ModelDialog> diag;
	diag = new ModelDialog();
	int error = 0;
	if (!diag->InitializeWindow(error))
		return;
	if (!diag->Initialize2D(error))
		return;
	if (!diag->InitializeAnaface(error))
		return;
	diag->InitializeControls();
	diag->DoModal();

	if (diag->GetOkay())
	{
		TString index = diag->GetIndexString();
		TString vert = diag->GetVertexString();

		

		TrecPointer<TArray<TString>> indices = index.split(TString(L","));
		if (!indices.get() || !indices->Count())
			return;
		TDataArray<UINT> indicesNum;
		for (UINT Rust = 0; Rust < indices->Count(); Rust++)
		{
			TString* ind = indices->ElementAt(Rust).get();
			if (!ind || !ind->GetLength())
				return;
			int n = 0;
			ind->Trim();
			if (ind->ConvertToInt(&n))
				return;
			indicesNum.push_back(static_cast<UINT>(n));
		}

		TrecPointer<TArray<TString>> vertices = vert.split(TString(L";"));
		if (!vertices.get() || !vertices->Count())
			return;
		TDataArray<float> verticesNum;
		for (UINT Rust = 0; Rust < vertices->Count(); Rust++)
		{
			TString* ind = vertices->ElementAt(Rust).get();
			if (!ind || !ind->GetLength())
				return;
			float n = 0;
			ind->Trim();
			if (ind->ConvertToFloat(&n))
				return;
			verticesNum.push_back(n);
		}

		ArenaModel* am = new ArenaModel(*ae.get());
		am->SetVertexData(verticesNum, default_shader_Single_Color);
		am->SetIndices(indicesNum);
		am->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);

		am->setName(diag->GetModelName());

	}
	
}

void CAnaGameBuilderDoc::OnNewCodeFile(TControl * tc, EventArgs ea)
{
	SourceCodeApp* aa = new SourceCodeApp(body, outputPanel, classUI, windowHandle);
	//aa->SetArenaName(name);

	aa->InitializeControls();
	TrecPointer<TControl> tp_tc = aa->GetMainPage();

	if (!tp_tc.get())
		return;

	BuilderApp* ba = aa;
	TrecPointer<BuilderApp> tp_ba;
	tp_ba = ba;
	ActiveDocuments.Add(tp_ba);
	//	body->addControl(tp_tc);
	currentDocument = tp_ba;
}

void CAnaGameBuilderDoc::OnImportCodeFile(TControl * tc, EventArgs ea)
{
}

void CAnaGameBuilderDoc::OnSaveAll(TControl * tc, EventArgs ea)
{
	for (UINT Rust = 0; Rust < ActiveDocuments.Count(); Rust++)
	{
		TrecPointer<BuilderApp> ba = ActiveDocuments.ElementAt(Rust);
		BuilderApp* rba = ba.get();
		if (rba)
		{
			rba->OnSave();
		}
	}
}

void CAnaGameBuilderDoc::OnSave(TControl * tc, EventArgs ea)
{
	BuilderApp* rba = currentDocument.get();
	if (rba)
		rba->OnSave();
}
