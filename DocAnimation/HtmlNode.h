#pragma once
#include <Tobject.h>
#include <TString.h>

class HtmlNode :
	public TObject
{
public:
	HtmlNode();
	~HtmlNode();


protected:
	TString tagTitle;
};

