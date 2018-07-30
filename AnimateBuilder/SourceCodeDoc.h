#pragma once
#include "stdafx.h"

// SourceCodeDoc document

class SourceCodeDoc : public CRichEditDoc
{
	DECLARE_DYNCREATE(SourceCodeDoc)

public:
	SourceCodeDoc();
	virtual ~SourceCodeDoc();

public:

	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;


#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
