#include "stdafx.h"
#include "HTMLParser.h"
#include "SSGenerator.h"

/*
* Method: (HTMLParser) (Constructor)
* Purpose: Sets up the HTML Parser with a place to draw the web page
* Parameters: TrecComPointer<ID2D1RenderTarget> resources - the region to draw the page to
*			HWND window - the window to draw to
*			TString location - The url the HTML file is located in
*			bool onDrive - whether or not the URL is on the local drive (flase means it's on the web)
* Returns: void
*/
HTMLParser::HTMLParser(TrecComPointer<ID2D1RenderTarget> resources, HWND window, TString location, bool onDrive)
{
	renderer = resources;
	windowHandle = window;
	fileLoc = location;
	this->onDrive = onDrive;
	pMode = boldMode = italicMode = false;
	currentColunm = maxColunm = row = 0;
	linkMode = 0;
	mode = 0;
	tableMode = 0;
	listCount = 0;
	classList = new TArray<styleTable>();
}

/*
* Method: (HTMLParser) (Destructor)
* Purpose: cleans up the HTML parser
* Parameters: void
* Returns: void
*/
HTMLParser::~HTMLParser()
{
}

/*
* Method: HTMLParser - Obj
* Purpose: Responds to calls by the reader when the reader detects a new tag to submit
* Parameters: TString* v - the string with the tag
* Returns: bool - whether the tag submission was valid
*/
bool HTMLParser::Obj(TString * v)
{
	if(!v)
		return false;

	linkMode = 0;
	
	if (!v->Compare(L"body")) // This should be the Root Control
	{
		rootObj = new TLayout(renderer, classList);
		rootObj->AddClass(TString(L"body"));
		dynamic_cast<TLayout*>(rootObj.get())->setLayout(VStack);
		tableMode = tableMode | 0b00001000;
		AddToTree();
	}
	else if (!v->Compare(L"title"))
	{
		mode = mode | 0b01000000;
	}
	else if (!v->Compare(L"/title") || !v->Compare(L"title/"))
	{
		mode = mode & 0b10111111;
	}
	else if (!v->Compare(L"p"))
	{
		currentObj = TrecPointer<TTextField>(new TTextField(renderer, classList, windowHandle));
		AppendAppropriateClasses();
		currentObj->AddClass(TString(L"p"));
		mode = mode | 0b00000100;
		AddToTree();
	}
	else if (!v->Compare(L"/p") || !v->Compare(L"p/"))
	{
		goParent();
		mode = mode & 0b11111000;
	}
	else if (!v->Compare(L"hr"))
	{

	}
	else if (!v->Compare(L"ul"))
	{
		

		listClass = L"ul";
		//Set mode to unordered list
		mode = mode | 0b00010000;
		// Unset ordered Mode
		mode = mode & 0b11110111;
		//AddToTree();
	}
	else if (!v->Compare(L"ol"))
	{

		listClass = L"ol";
		// Unset mode from unordered list
		mode = mode | 0b00001000;
		// set ordered Mode
		mode = mode & 0b11101111;
		//AddToTree();
	}
	else if (!v->Compare(L"li"))
	{
		TTextField* tl = new TTextField(renderer, classList,windowHandle);
		//tl->setLayout(VStack);
		currentObj = TrecPointer<TTextField>(tl);

		AppendAppropriateClasses();
		currentObj->AddClass(TString(L"li"));

		AddToTree();
		// Activate List Mode
		mode = mode | 0b10000000;
		listCount++;
	}
	else if (!v->Compare(L"/li") || !v->Compare(L"li/"))
	{
		mode = mode & 0b01111111;
		goParent();
	}
	else if (!v->Compare(L"b") || !v->Compare(L"strong"))
	{
		// To Do, set Text

		// Set Bold Mode to true
		mode = mode | 0b00000001;
	}
	else if (!v->Compare(L"i") || !v->Compare(L"em"))
	{
		// To Do, set Text

		// Set Bold Mode to true
		mode = mode | 0b00000010;
	}
	else if (!v->Compare(L"br"))
	{
		TTextField* textObj = GetTextField();
		if (textObj)
		{
			textObj->AppendNormalText(TString(L"\n"));
		}
	}
	else if (!v->Compare(L"img"))
	{
		currentObj = TrecPointer<TControl>(new TControl(renderer, classList));
		AddToTree();
	}
	else if (!v->Compare(L"a") || !v->Compare(L"<a>")|| !v->Compare(L"<a"))
	{

	}
	else if (!v->Compare(L"blockquote") || !v->Compare(L"<blockquote>")|| !v->Compare(L"<blockquote"))
	{

	}
	else if (!v->Compare(L"table") || !v->Compare(L"<table>") || !v->Compare(L"<table"))
	{
		TLayout* tl = new TLayout(renderer, classList);
		tl->setLayout(grid);
		tl->AddClass(TString(L"table"));
		currentObj = TrecPointer<TLayout>(tl);
		currentColunm = maxColunm = 0;

		AddToTree();
		tableMode = tableMode | 1;
		baseObj = currentObj;
	}
	else if (!v->Compare(L"/table") || !v->Compare(L"table/"))
	{
		goParent();
	}
	else if (!v->Compare(L"tr") || !v->Compare(L"<tr>") || !v->Compare(L"<tr"))
	{
		try
		{
			TLayout* tl = dynamic_cast<TLayout*>(baseObj.get());
			if (tl->GetOrganization() == grid)
			{
				tl->addRow( 100,false);
			}
		}
		catch (std::bad_cast& e)
		{
			TRACE(e.what());
		}
		currentColunm = 0;
	}
	else if (!v->Compare(L"/tr") || !v->Compare(L"tr/"))
	{
		//goParent();
	}
	else if (!v->Compare(L"th") || !v->Compare(L"<th>")||!v->Compare(L"<th"))
	{
		try
		{
			TLayout* tl = dynamic_cast<TLayout*>(baseObj.get());
			if (tl->GetOrganization() == grid)
			{
				tl->addColunm(100, true);// setNewColunmSize(currentColunm++, 100);
				maxColunm++;
				currentColunm++;
				tableMode = tableMode | 0b00110000;
			}
			
		}
		catch (std::bad_cast& e)
		{
			TRACE(e.what());
		}
		//currentColunm++;
		

	}
	else if (!v->Compare(L"/th") || !v->Compare(L"th/"))
	{
		if (!(tableMode & 0b00010000))
			goParent();
		tableMode = tableMode & 0b11101111; // Set it to table cell AND table Header
		mode = mode & 0b11111110;
	}
	else if (!v->Compare(L"td") || !v->Compare(L"<td>") || !v->Compare(L"<td"))
	{
		if (currentColunm < maxColunm)
			currentColunm++;
		tableMode = tableMode | 0b00010000; // Set it to a table cell
		tableMode = tableMode & 0b11011111; // Not using a tabl header
	}
	else if (!v->Compare(L"/td") || !v->Compare(L"td/"))
	{
		if(!(tableMode & 0b00010000))
			goParent();
		tableMode = tableMode & 0b11101111;
		//
	}
	else if (!v->Compare(L"i/") || !v->Compare(L"/i") || !v->Compare(L"/em") || !v->Compare(L"em/"))
	{
		mode = mode & 0b11111101;
	}
	else if (!v->Compare(L"b/") || !v->Compare(L"/b") || !v->Compare(L"/strong") || !v->Compare(L"strong/"))
	{
		mode = mode & 0b11111110;
	}
	else if (!v->Compare(L"style") || !v->Compare(L"<style>"))
	{
		linkMode = linkMode | 0b00000001;
	}
	else if (!v->Compare(L"/style") || !v->Compare(L"style/"))
	{
		linkMode = linkMode & 0b11111110;
	}
	else if (!v->Compare(L"/ol") || !v->Compare(L"ol/") || !v->Compare(L"/ul") || !v->Compare(L"ul/"))
	{
		// Go ahead an end all modes regarding lists
		listClass.Empty();
		mode = mode & 0b01100111;
		listCount = 0;
	}
	else if (!v->Compare(L"link"))
	{
		linkMode = linkMode | 1;
	}
	else if (!v->Compare(L"link/") || !v->Compare(L"/link"))
	{
		linkMode = 0;
	}
	/*else if (!v->Compare(L""))
	{

	}*/
	return true;
}

/*
* Method: HTMLParser - Attribute
* Purpose: Sets up an attribute for a current tag
* Parameters: TString * v, TString e
* Returns: bool - false
* NOTE: DEPRECIATED - not used
*/
bool HTMLParser::Attribute(TString * v, TString e)
{
	return false;
}

/*
* Method: HTMLParser - Attribute
* Purpose: Sets up an attribute for a current tag
* Parameters: TrecPointer<TString> v - the value of the attribute
*				TString& e - the type of attribute
* Returns: bool - whether the submission was valid
*/
bool HTMLParser::Attribute(TrecPointer<TString> v, TString& e)
{
	if(!e || !v.get())
		return false;

	v->Trim();

	if (!v->GetLength())
		return true;

	if (!wcscmp(e, L"caption"))
	{
		if (mode & 0b10000000)
		{
			TString cap = *v.get();
			if (mode & 0b00010000) // Unordered List
			{
				v->Format(_T("*  %ws"), cap.GetBuffer());
				cap.ReleaseBuffer();
			}
			else if (mode & 0b00001000) // Ordered List
			{
				v->Format(_T("%hu. %ws"), listCount, cap.GetBuffer());
				cap.ReleaseBuffer();
			}
		}

		if (tableMode & 0b00010000)
		{
			currentObj = TrecPointer<TTextField>(new TTextField(renderer, classList, windowHandle));
			AppendAppropriateClasses();
			if (tableMode & 0b00100000)
			{
				currentObj->AddClass(TString(L"th"));
				mode = mode & 1; // In Header Cells, Use Bold Mode
			}
			else
				currentObj->AddClass(TString(L"td"));
			mode = mode | 0b00000100;
			AddToTree();
			tableMode = tableMode & 0b11001111;
		}
		if (linkMode & 1)
		{
			CSSGenerator* css = new CSSGenerator(v.get());
			css->Parse();

			// To-Do: Retrieve styles from CSS object
			TrecPointer<TArray<styleTable>> st = css->GetStyles();

			for (UINT c = 0; c < st->Count(); c++)
			{
				classList->Add(st->ElementAt(c));
			}

			//
			delete css;
		}
		else if (mode & 0b01000000)
		{
			pageTitle = v.get();
		}
		else
		{
			TTextField* textObj = GetTextField();
			if (!textObj)
			{
				if(!tableMode)
					return true;
				if (tableMode & 1)
				{
					TLayout* layout = nullptr;
					try
					{
						layout = dynamic_cast<TLayout*>(baseObj.get());
					}
					catch (std::bad_cast& e)
					{
						return true;
					}
					if (!layout)
						return true;

					textObj = new TTextField(renderer, classList, windowHandle);
					currentObj = textObj;
					AddToTree();
				}
			}
			if (mode & 0b00000001) // Bold Mode is activated
			{
				if (mode & 0b00000010)
					textObj->AppendBoldItalicText(*v.get());
				else
					textObj->AppendBoldText(*v.get());
			}
			else // BoldMode is not activated
			{
				if (mode & 0b00000010)
					textObj->AppendItalicText(*v.get());
				else
					textObj->AppendNormalText(*v.get());
			}
		}

	}
	else if (!wcscmp(e, L"rel"))
	{
		linkMode = linkMode | 0b00000010;
	}
	else if (!wcscmp(e, L"title"))
	{
		pageTitle = v.get();
	}
	else if (!wcscmp(e, L"href"))
	{
		if (linkMode & 0b00000011)
		{
			CFile file;
			CFileException cfe;
			//TString* str = v.get();
			if(fileLoc.GetLength())
				if (fileLoc.GetAt(fileLoc.GetLength() - 1) != L'\\')
					fileLoc.AppendChar(L'\\');
			TString str = fileLoc;
			if(v.get())
				str += *v.get();
			if (!file.Open(str, CFile::modeRead, &cfe))
				return true;
			CArchive* archie = new CArchive(&file, CArchive::load);
			CSSGenerator* css = new CSSGenerator(*archie);
			css->Parse();
			// To-Do: retrieve styles from CSS object
			TrecPointer<TArray<styleTable>> st = css->GetStyles();

			for (UINT c = 0; c < st->Count(); c++)
			{
				classList->Add(st->ElementAt(c));
			}

			//
			delete css;
			archie->Close();
			delete archie;
			file.Close();
		}
	}

	return true;
}

/*
* Method: HTMLParser - submitType
* Purpose: N/A 
* Parameters: TString v - the type of parser to check
* Returns: bool - false
* NOTE: Method not used. Only supplied for TML parsers
*/
bool HTMLParser::submitType(TString v)
{
	return false;
}

/*
* Method: HTMLParser - submitEdition
* Purpose: N/A
* Parameters: TString v - the level of parser to check
* Returns: bool - false
* NOTE: Method not used. Only supplied for TML parsers
*/
bool HTMLParser::submitEdition(TString v)
{
	return false;
}

/*
* Method: HTMLParser - goChild
* Purpose: Sets the parser to focus on a child control
* Parameters: void
* Returns: bool - whether a child can be set
*/
bool HTMLParser::goChild()
{
	if (!currentObj.get())
		return false;
	baseObj = currentObj;
	return true;
}

/*
* Method: HTMLParser - goParent
* Purpose: Sets the parser to move up on the HTML tree
* Parameters: void
* Returns: void
*/
void HTMLParser::goParent()
{
	if (baseObj.get() && baseObj->getParent().get())
		baseObj = baseObj->getParent();
}

/*
* Method: HTMLParser - GetTitle
* Purpose: Returns the Title of the web-page being buit
* Parameters: void
* Returns: TString - the title of the web-page
*/
TString HTMLParser::GetTitle()
{
	return pageTitle;
}

/*
* Method: HTMLParser - GetRootControl
* Purpose: Retrieves the Root Control in the Web-Page
* Parameters: void
* Returns: TrecPointer<TControl> - the Web-Page constructed
*/
TrecPointer<TControl> HTMLParser::GetRootControl()
{
	return rootObj;
}

/*
* Method: HTMLParser - GetAnaGameType
* Purpose: Reports the TObject as an HTMLParser
* Parameters: void
* Returns: UCHAR* - the AnaGame representation of this class type
*/
UCHAR * HTMLParser::GetAnaGameType()
{
	return nullptr;
}

/*
* Method: HTMLParser - GetTextField
* Purpose: Retrieves the Current object as a TTextField
* Parameters: void
* Returns: TTextField* - the current control as a Text Control (null if object is not a Text Control)
*/
TTextField * HTMLParser::GetTextField()
{
	try
	{
		return dynamic_cast<TTextField*>(currentObj.get());
	}
	catch (std::bad_cast& e)
	{
		return nullptr;
	}
	return nullptr;
}

/*
* Method: HTMLParser - AddToTree
* Purpose: Adds a Control to the HTML document Tree
* Parameters: void
* Returns: void
*/
void HTMLParser::AddToTree()
{
	ASSERT(rootObj.get());
	if (!baseObj.get())
		baseObj = rootObj;
	else
	{
		try
		{
			// First see if base object is a TLayout
			TLayout* baseLayout = dynamic_cast<TLayout*>(baseObj.get());

			if (tableMode & 1)
			{
				//baseLayout->addRow(100, false);
				baseLayout->addChild(currentObj, currentColunm - 1, baseLayout->getRowNumber() - 1);
			}
			else if (tableMode & 0b00001000)
			{
				baseLayout->addRow(100, false);
				baseLayout->addChild(currentObj, 0, baseLayout->getRowNumber() - 1);
			}
			// To Do, look at modes for which we handle TLayout
			goChild();
		}
		catch (std::bad_cast& e)
		{
			// Stick with TControl's container functionality
			baseObj->addChild(currentObj);
		}
	}
}

/*
* Method: HTMLParser - AppendAppropriateClasses
* Purpose: Adds the current list of HTML classes to the HTML object
* Parameters: void
* Returns: void 
*/
void HTMLParser::AppendAppropriateClasses()
{
	if (!currentObj.get())
		return;
	if (listClass.GetLength())
		currentObj->AddClass(listClass);
}
