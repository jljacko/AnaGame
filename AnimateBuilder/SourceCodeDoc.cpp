// SourceCodeDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "SourceCodeDoc.h"


// SourceCodeDoc

IMPLEMENT_DYNCREATE(SourceCodeDoc, CDocument)

SourceCodeDoc::SourceCodeDoc()
{
}

BOOL SourceCodeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

SourceCodeDoc::~SourceCodeDoc()
{
}


BEGIN_MESSAGE_MAP(SourceCodeDoc, CDocument)
END_MESSAGE_MAP()


// SourceCodeDoc diagnostics

#ifdef _DEBUG
void SourceCodeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void SourceCodeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// SourceCodeDoc serialization

void SourceCodeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
	CRichEditDoc::Serialize(ar);
}
CRichEditCntrItem * SourceCodeDoc::CreateClientItem(REOBJECT * preo) const
{
	return new CRichEditCntrItem(preo, const_cast<SourceCodeDoc*>(this));;
}

#endif


// SourceCodeDoc commands
