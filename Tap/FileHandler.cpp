#include "FileHandler.h"
#include "Page.h"
#include "TIdeWindow.h"
#include <TFileNode.h>
#include <DirectoryInterface.h>

/**
 * Method: FileHandler::FileHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
 * Returns: New FileHandler Object
 */
FileHandler::FileHandler(TrecPointer<TInstance> instance) : EventHandler(instance)
{
}

/**
 * Method: FileHandler::~FileHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
FileHandler::~FileHandler()
{
}

/**
 * Method: FileHandler::Initialize
 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
 * Returns: void
 */
void FileHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get() || !page->GetWindowHandle().Get())
		return;
	TIdeWindow* ideWin = dynamic_cast<TIdeWindow*>(page->GetWindowHandle().Get());

	browser = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTreeDataBind>(page->GetRootControl());

	if (browser.Get())
	{
		TrecPointer<TFileShell> rootFile;

		TrecPointer<TObjectNode> node = TrecPointerKey::GetNewSelfTrecPointerAlt<TObjectNode, TFileNode>(0);
		
		if (ideWin)
			rootFile = ideWin->GetEnvironmentDirectory();
		
		if(!rootFile.Get())
			rootFile = TFileShell::GetFileInfo(GetDirectory(CentralDirectories::cd_Documents));
		dynamic_cast<TFileNode*>(node.Get())->SetFile(rootFile);

		browser->SetNode(node);
	}
}

/**
 * Method: FileHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void FileHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
}

/**
 * Method: FileHandler::ProcessMessage
 * Purpose: Processes the message sent to the handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
 * Returns: void
 */
void FileHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

/**
 * Method: FileHandler::ShouldProcessMessageByType
 * Purpose: Reports whether this Object is of the correct type to recieve the message
 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
 * Returns: bool - true if the type matches, false oherwise
 */
bool FileHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if (!message.Get())
		return false;
	return message->GetHandlerType() == handler_type::handler_type_file_manager;
}
