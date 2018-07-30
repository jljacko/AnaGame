// AnafaceDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "AnafaceDoc.h"
#include "TControl_Edit.h"
#include "TLayoutEdit.h"
//#include "TContainerDialog.h"
#include "TLayoutLocDialog.h"
#include "TControl_TDialog.h"

// AnafaceDoc

IMPLEMENT_DYNCREATE(AnafaceDoc, CDocument)

AnafaceDoc::AnafaceDoc()
{
	//windowSet = false;
	// TODO: add one-time construction code here
	ID2D1Factory* f = nullptr;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &f);
	ASSERT(SUCCEEDED(hr));
	factory = f;
	if (!SUCCEEDED(hr))
	{
		printf("Factory not Created\n");
		return;
	}

	ID2D1DCRenderTarget* tempRT = nullptr;

	hr = factory->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		0, 0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
	)), &tempRT);
	ASSERT(SUCCEEDED(hr));
	if (!SUCCEEDED(hr))
	{
		factory = NULL;
		
		return;
	}
	rt = tempRT;
	focusControl = NULL;
	editControl = NULL;
}

BOOL AnafaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

AnafaceDoc::~AnafaceDoc()
{
	editControl.Delete();
	rt.Delete();
	factory.Delete();
}


BEGIN_MESSAGE_MAP(AnafaceDoc, CDocument)
	ON_COMMAND(ID_BUTTON3, &AnafaceDoc::OnButtonNewTControl)
	ON_COMMAND(ID_BUTTON2, &AnafaceDoc::OnButtonEditTControl)
	ON_COMMAND(ID_BUTTON5, &AnafaceDoc::OnButtonNewLayout)
	ON_COMMAND(ID_BUTTON9, &AnafaceDoc::OnButton9)
END_MESSAGE_MAP()


// AnafaceDoc diagnostics

#ifdef _DEBUG
void AnafaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void AnafaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// AnafaceDoc serialization

void AnafaceDoc::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		// TODO: add storing code here
		if (!editControl.get())
			return;
		editControl->storeInTML(&ar,0);
	}
	else
	{
		CFile* file = ar.GetFile();
		TString directory = file->GetFilePath();
		TString fileName = file->GetFileName();
		int ind = directory.Find(fileName, 0);
		if (ind > 0)
			directory.Delete(ind, fileName.GetLength());
		// TODO: add loading code here
		AnafaceParser* aParse = new AnafaceParser(rt, windowHandle,&directory);
		TML_Reader_* reader = new TML_Reader_(&ar, aParse);
		int readingError;
		if (!reader->read(&readingError))
		{
			readingError = readingError + 0;
		}
		else
		{
			editControl = aParse->getRootControl();
			editControl->onCreate(area);
			this->UpdateAllViews(NULL);
		}
	}
}
#endif

void AnafaceDoc::setUpTControl(CDC * pCD)
{


	UINT boundResult = pCD->GetBoundsRect(&area, 0);
	GetClientRect(WindowFromDC(pCD->m_hDC), &area);
	ID2D1RenderTarget* tempRt = rt.get();
	
	if (typeid(tempRt) != typeid(ID2D1DCRenderTarget*))
		return;
	ID2D1DCRenderTarget* tempDC = dynamic_cast<ID2D1DCRenderTarget*>(tempRt);

	HRESULT hr = tempDC->BindDC(pCD->GetSafeHdc(), &area);
	ASSERT(SUCCEEDED(hr));
}

void AnafaceDoc::OnDraw()
{
	if (rt.get())
	{
		//rt->Clear();
		rt->BeginDraw();
		if (editControl.get())
			editControl->onDraw();
		rt->EndDraw();
	}
}

TrecPointer<TControl> AnafaceDoc::returnRootControl()
{
	return editControl;
}

TrecPointer<TControl> AnafaceDoc::returnFocusControl()
{
	return focusControl;
}

void AnafaceDoc::UpdateEditeViaMouseClick(UINT flags, CPoint p)
{
	messageOutput mo = negative;
	if (editControl.get())
		editControl->Builder_OnLButtonDown(flags, p, focusControl, &mo);
	if (mo == positiveContinueUpdate || mo == positiveOverrideUpdate)
		this->UpdateAllViews(NULL);
}

RECT AnafaceDoc::getArea()
{
	return area;
}

// AnafaceDoc commands
namespace
{
	int row = 0, colunm = 0;
}
RECT AnafaceDoc::getRectLoation()
{
	if (!focusControl.get() && editControl.get())
		return RECT{ 0, 0, 0, 0 };
	if (!editControl.get())
		return area;

	if ( !focusControl.get())
		return RECT{ 0,0,0,0 };

	if (typeid(focusControl.get()) == typeid(TLayout*))
	{
		TLayoutLocDialog tlld;
		row = 0; colunm = 0;
		tlld.setPointers(&row, &colunm);
		tlld.setMaximum(dynamic_cast<TLayout*>(focusControl.get())->getRowNumber(),
			dynamic_cast<TLayout*>(focusControl.get())->getColumnNumber());
		tlld.DoModal();

		RECT returnable = focusControl->getLocation();
		RECT inner = dynamic_cast<TLayout*>(focusControl.get())->getRawSectionLocation(row, colunm);
		if (inner.right == 0 || inner.bottom == 0)
			return RECT{ 0,0,0,0 };

		returnable.top += inner.top;
		returnable.bottom -= inner.bottom;
		returnable.left += inner.left;
		returnable.right -= inner.right;
		return returnable;
	}
	else
	{
		return focusControl->getLocation();
	}
	return RECT();
}

/*
TrecPointer<TContainer> AnafaceDoc::getNewTContainer()
{
	TContainerDialog tcd;
	tcd.DoModal();
	return tcd.getContainer();
}*/

void AnafaceDoc::OnButtonNewTControl()
{
	/*
	RECT tArea;
	TrecPointer<TContainer> tcon;
	if (focusControl && focusControl)
	{
		tArea = focusControl->getLocation();
		tcon = getNewTContainer();
	}
	else if (!editControl)
	{
		tcon = std::make_shared<TContainer>();
		tArea = area;
	}
	else        // Not in the proper position to make a new control
		return;

	// TODO: Add your command handler code here
	TControl* cont = new TControl(rt, NULL);


	// Let the Control know what Pointer is containing it
	cont->setWrapperPointer(contP);

	//cont->onCreate(area);
	TControl_Edit* tceDialog = new TControl_Edit(contP);
	//tceDialog->initialize();
	tceDialog->DoModal();

	delete tceDialog;
	//

	bool onCreated = false;

	if (!editControl)
	{
		editControl = contP;
		cont->onCreate(tArea);
		onCreated = true;
	}
	if (!focusControl)
	{
		focusControl = contP;
	}
	else
	{
		if (focusControl)
		{

			tcon->setTControl(contP);
			tcon->setParent(focusControl);
			focusControl->addChild(tcon);

			if (!onCreated)
			{
				tcon->onCreate(tArea);
				onCreated = true;
			}
		}
			
	}
	*/


	TControl_TDialog* dial = new TControl_TDialog(700, 700);
	if (!dial)
		return;
	

	TString str = L"Test Dialog";
	dial->setName(str);

	int error = 0;
	dial->Initialize(error);
	//dial->Initialize2D(this->)
	dial->initializeControls();
	if (!error)
	{
		dial->DoModal();
		TrecPointer<TControl> contP = dial->GetControl();
	}



	delete dial;
	this->UpdateAllViews(NULL);
}


void AnafaceDoc::OnButtonEditTControl()
{
	if (!focusControl.get())
		return;

	RECT tempArea;
	RECT contArea = focusControl->getLocation();
	RECT margin = focusControl->getMargin();

	tempArea.bottom = contArea.bottom + margin.bottom;
	tempArea.left = contArea.left - margin.left;
	tempArea.right = contArea.right + margin.right;
	tempArea.top = contArea.top - contArea.top;

	TControl_Edit* tceDialog = new TControl_Edit(focusControl);
	//tceDialog->initialize();
	tceDialog->DoModal();
	delete tceDialog;
	// TODO: Add your command handler code here
	focusControl->onCreate( tempArea);
	this->UpdateAllViews(NULL);
}


void AnafaceDoc::OnButtonNewLayout()
{/*
	TLayout* tlay = new TLayout(rt, NULL);
	TControlPointer* tcp = new TControlPointer{ tc_TLayout,tlay,tlay };
	TLayoutEdit* tleDialog = new TLayoutEdit(tcp);
	//tleDialog->initialize();
	tleDialog->DoModal();
	delete tleDialog;*/
	// TODO: Add your command handler code here
}


void AnafaceDoc::OnButton9()
{/*
	if (!focusControl || !focusControl)
		return;
	if (typeid(focusControl.get()) != typeid(TLayout*))
		return;
	TLayoutEdit* tleDialog = new TLayoutEdit(focusControl);
	tleDialog->DoModal();
	delete tleDialog;
	*/
	// TODO: Add your command handler code here
}
