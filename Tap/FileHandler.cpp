#include "FileHandler.h"
#include "Page.h"
#include "TIdeWindow.h"
#include <TFileNode.h>
#include <DirectoryInterface.h>

FileHandler::FileHandler(TrecPointer<TInstance> instance) : EventHandler(instance)
{
}

FileHandler::~FileHandler()
{
}

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

void FileHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
}

void FileHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

bool FileHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if (!message.Get())
		return false;
	return message->GetHandlerType() == handler_type::handler_type_file_manager;
}
