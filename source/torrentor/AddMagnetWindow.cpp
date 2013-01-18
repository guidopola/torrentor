#include <Application.h>
#include <Button.h>
#include <Clipboard.h>
#include <ControlLook.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <String.h>
#include <StringView.h>
#include <TextControl.h>
#include <Window.h>

#include "TorrentorMessages.h"
#include "AddMagnetWindow.h"

enum
{
	MSG_MAGNET_OPEN,	
};

OpenMagnetWindow::OpenMagnetWindow()
	:	BWindow(BRect(),
				"Open Magnet Link", 
				B_TITLED_WINDOW_LOOK,
				B_NORMAL_WINDOW_FEEL, 
				B_AUTO_UPDATE_SIZE_LIMITS |
				B_ASYNCHRONOUS_CONTROLS |
				B_NOT_ZOOMABLE |
				B_NOT_RESIZABLE |
				B_PULSE_NEEDED )
{
	SetLayout(new BGroupLayout(B_VERTICAL));
	
	
	const float spacing = be_control_look->DefaultItemSpacing();
	
	//
	//
	//
	BButton* fCancelButton 	= new BButton("", "Cancel", new BMessage(B_QUIT_REQUESTED));
	BButton* fOpenButton 	= new BButton("", "Open", new BMessage(MSG_MAGNET_OPEN));
	BStringView* fLinkLabel	= new BStringView("", "Link:");
	fLinkText = new BTextControl("", "", NULL);
	fLinkText->SetExplicitMinSize(BSize(400, 20));
	
	//
	//
	//
	AddChild(BGroupLayoutBuilder(B_VERTICAL, spacing)
		.AddGroup(B_HORIZONTAL)
			.Add(fLinkLabel)
			.Add(fLinkText)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fCancelButton)
			.AddGlue()
			.Add(fOpenButton)
		.End()
		.SetInsets(spacing, spacing, spacing, spacing)
	);
	
	fLinkText->MakeFocus();
	fLinkText->SetText(GetMagnetFromClipboard());
	fOpenButton->MakeDefault(true);
	CenterOnScreen();
	Run();
}

BString OpenMagnetWindow::GetMagnetFromClipboard() {
	const char *text = NULL;
	ssize_t numBytes;
	status_t status;
	BMessage *clip = (BMessage *)NULL;
	if (be_clipboard->Lock()) {
		if ((clip = be_clipboard->Data())) {
			status = clip->FindData("text/plain", B_MIME_TYPE, (const void **)&text, &numBytes);
		}
		be_clipboard->Unlock();
	}
	
	if (status == B_OK) {
		BString link(text);
		if (link.FindFirst("magnet:") == 0) {
			return link;
		}
	}
	
	return BString();
}

void OpenMagnetWindow::MessageReceived(BMessage* message)
{
	switch( message->what )
	{
	case MSG_MAGNET_OPEN:
		OnMagnetOpenRequest();
		break;
	default:
		BWindow::MessageReceived(message);
		break;
	}
}

void OpenMagnetWindow::OnMagnetOpenRequest()
{
	//
	//
	//
	BMessage* message = new BMessage(MSG_OPEN_MAGNET_LINK);
	message->AddString("URL", fLinkText->Text());
	
	be_app->PostMessage(message);
	Quit();
}

