//------------------------------------------------------------------------------
//	Copyright (c) 2012-2013, Guido Pola.
//
//	Permission is hereby granted, free of charge, to any person obtaining a
//	copy of this software and associated documentation files (the "Software"),
//	to deal in the Software without restriction, including without limitation
//	the rights to use, copy, modify, merge, publish, distribute, sublicense,
//	and/or sell copies of the Software, and to permit persons to whom the
//	Software is furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//	DEALINGS IN THE SOFTWARE.
//
//	File Name:		Application.cpp
//	Authors:		Guido Pola <prodito@live.com>
//	Description:	Main application class.
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>


#include <Alert.h>
#include <AppFileInfo.h>
#include <File.h>
#include <Notification.h>
#include <Roster.h>
#include <StringList.h>

#include "Torrentor.h"
#include "TorrentorMessages.h"
#include "IconUtil.h"
#include "MainWindow.h"
#include "AddMagnetLinkWindow.h"
#include "AddTorrentWindow.h"
#include "Application.h"
#include "TorrentPreferences.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Application"


// tr_torrentSetCompletenessCallback
// tr_torrentSetMetadataCallback
// tr_torrentSetRatioLimitHitCallback
// tr_torrentSetIdleLimitHitCallback

void TorrentCompletenessHandler(tr_torrent* torrent, tr_completeness completeness, bool wasRunning, void* data)
{
	//
	//
	//
	BMessage* msg = new BMessage(MSG_TORRENT_COMPLETENESS);
	
	msg->AddPointer("torrentHandle", torrent);
	msg->AddInt32("torrentCompleteness", static_cast<int32>(completeness));
	msg->AddBool("torrentRunning", wasRunning);
	
	be_app->PostMessage(msg);
	
	/*
	
	//static const char* _completeness[] = {
	//	"TR_LEECH",
	//	"TR_SEED",
	//	"TR_PARTIAL_SEED",	
	//};
	//
	//printf("TorrentCompletenessHandler: %s, wasRunning -> %s\n",
	//	_completeness[completeness], wasRunning ? "true" : "false");
	//
	// 
	//
	if( completeness == TR_LEECH || !wasRunning )
		return;
	
	BNotification* notification = new BNotification(B_INFORMATION_NOTIFICATION);
	
	
	notification->SetGroup("Torrentor!");
	notification->SetTitle("Download complete");
	notification->SetContent( tr_torrentInfo( torrent )->name );
	notification->Send();*/
}



TorrentorApp::TorrentorApp()
	:	BApplication("application/x-vnd.Torrentor"),
		fTorrentSession(NULL)
{	
	tr_formatter_mem_init(1024, "KiB", "MiB", "GiB", "TiB");
	tr_formatter_size_init(1024, "KiB", "MiB", "GiB", "TiB");
    tr_formatter_speed_init(1024, "KiB/s", "MiB/s", "GiB/s", "TiB/s");
 	
 	//
 	//
 	fTorrentPreferences = new TorrentPreferences;
 	
 	//
 	//
 	//
	fTorrentSession = tr_sessionInit(tr_getDefaultConfigDir("Torrentor"), false, fTorrentPreferences->Handle());	
	char * str 		= tr_variantToStr( fTorrentPreferences->Handle(), TR_VARIANT_FMT_JSON_LEAN, NULL );


	fTorrentPreferences->SetSession(fTorrentSession);
	
	//printf("configDir -> %s\n", configDir);


	// set up a rectangle and instantiate a new window
	fMainWindow = new MainWindow(BRect(0, 0, 800, 600));
			

	
	
	fprintf( stderr, "%s", str );
	tr_free( str );

	
	LoadTorrentList();
	
	// make window visible
	fMainWindow->Show();
	
	//
	//
	//
	//CheckForUpdates();
	_CheckMimeTypeAssociation();
}

TorrentorApp::~TorrentorApp()
{
	fTorrentPreferences->Save();
	tr_sessionClose(fTorrentSession);
	
	delete fTorrentPreferences;
}

void TorrentorApp::MessageReceived(BMessage* message)
{
	switch( message->what ) 
	{
	case MSG_OPEN_MAGNET_REQUEST:
		OpenMagnetLinkWindow();
		break;
	case MSG_OPEN_MAGNET_LINK:
		LoadTorrentFromMagnet(message);
		break;
	case MSG_OPEN_TORRENT_RESULT:
		OpenTorrentResult(message);
		break;
	case MSG_TORRENT_COMPLETENESS:
		OnTorrentComplete(message);
		break;
	case MSG_TORRENT_PAUSE_START_ALL:
		OnTorrentPauseStartAll(message);
		break;
	default:
		BApplication::MessageReceived(message);
		break;
	}

}

void TorrentorApp::ReadyToRun()
{
	SetPulseRate(500000);
}


void TorrentorApp::RefsReceived(BMessage* message)
{
	int32 index = 0;
	entry_ref ref;
	BStringList TorrentPathList;

	//
	//
	//
	while (message->FindRef("refs", index++, &ref) == B_OK) {
		BEntry FileEntry(&ref);
		BPath  FilePath;
		
		FileEntry.GetPath(&FilePath);
		TorrentPathList.Add(FilePath.Path());
	}
	LoadTorrentFromFiles(TorrentPathList);
}

void TorrentorApp::ArgvReceived(int32 argc, char** argv)
{
	for (int32 i = 1; i < argc; i++) {
		BString arg(argv[i]);
		if (arg.FindFirst("magnet:") == 0) {
			LoadTorrentFromMagnet(arg);
		}
	}
}

void TorrentorApp::Pulse()
{
	if( fMainWindow != NULL )
		fMainWindow->Update();
}

bool TorrentorApp::QuitRequested()
{
	return true;
}


//
//
//
void TorrentorApp::CreateSession()
{
	//fTorrentSession = tr_sessionInit("torrentor", configDir, true, &settings);
	//fTorrentSession = tr_sessionInit("torrentor", configDir, FALSE, &settings);
}


void TorrentorApp::_CheckMimeTypeAssociation()
{
	BMimeType torrent_mime("application/x-bittorrent");
	//BMimeType magnet_mime("application/x-vnd.Be.URL.magnet");
	
	char preferred[B_MIME_TYPE_LENGTH];
	
	if (torrent_mime.GetPreferredApp(preferred) != B_OK)
		return;
	
	if (strcmp("application/x-vnd.Torrentor", preferred) == 0)
		return;
		
	BAlert* updatePreferredAlert = new BAlert("", "Torrentor is not the default application to open torrent's files.\n\n"
		"Do you want to make Torrentor the default application to open torrent files?...",
    	"Cancel", "Accept", NULL, B_WIDTH_FROM_LABEL, B_EVEN_SPACING, B_IDEA_ALERT);

	//	
	if (updatePreferredAlert->Go() == 1)
		torrent_mime.SetPreferredApp("application/x-vnd.Torrentor");
}

#pragma mark Version checker

//
// @TODO: rewrite cURL code.
//
struct CURL_MemoryStruct {
  char *memory;
  size_t size;
};


static size_t CURL_WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct CURL_MemoryStruct *mem = (struct CURL_MemoryStruct *)userp;

  mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    //exit(EXIT_FAILURE);
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}
void version_info_from_string(version_info* versionInfo, BString version)
{
	char* buffer = version.LockBuffer(0);
	
	//
	// Here we use the old friend 'strtok'
	//
	char* pch = strtok(buffer,".");
	versionInfo->major = atoi(pch);
	pch = strtok(NULL,".");
	versionInfo->middle = atoi(pch);
	pch = strtok(NULL,".");
	versionInfo->minor = atoi(pch);

	version.UnlockBuffer();
}

void TorrentorApp::CheckForUpdates()
{
	//
	//
	//
	CURL* curlHandle = curl_easy_init();
	
	if( curlHandle == NULL )
		return;
		
	CURL_MemoryStruct chunk;

	chunk.memory = (char*)malloc(1);
	chunk.size = 0;

		
	//
	//
	//
	curl_easy_setopt(curlHandle, CURLOPT_URL, "https://raw.github.com/Prodito/Torrentor/master/VERSION");
	curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, CURL_WriteMemoryCallback);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_perform(curlHandle);
	curl_easy_cleanup(curlHandle);

	//
	// Now we need to get the app's current version.
	//
	app_info appInfo;
	BFile appFile;
	BAppFileInfo appFileInfo;
	version_info appVersionInfo;
	version_info webVersionInfo;
	BString appVersion;

	
	be_app->GetAppInfo(&appInfo);
	appFile.SetTo(&appInfo.ref, B_READ_WRITE);
	appFileInfo.SetTo(&appFile);
	appFileInfo.GetVersionInfo(&appVersionInfo, B_APP_VERSION_KIND);
	version_info_from_string(&webVersionInfo, "0.0.4");
	
	//
	//
	//
	appVersion << "Version " << webVersionInfo.major << "." << webVersionInfo.middle << "." << webVersionInfo.minor;
	
	//
	//
	//
	if( appVersionInfo.major  < webVersionInfo.major  ||
	    appVersionInfo.middle < webVersionInfo.middle ||
	    appVersionInfo.minor  < webVersionInfo.minor   )
	{
		//
		//
		//
		BNotification* notification = new BNotification(B_INFORMATION_NOTIFICATION);
		
		
		notification->SetGroup("Torrentor!");
		notification->SetTitle("Update available");
		notification->SetContent( appVersion );
		notification->SetOnClickApp("application/x-vnd.Be.URL.http");
		notification->AddOnClickArg("http://haikuware.com/remository/view-details/internet-network/clients/torrentor");
		notification->Send();
	}
}

#pragma mark -

//
//
//
const tr_session* TorrentorApp::Session() const
{
	return fTorrentSession;
}

void TorrentorApp::LoadTorrentList()
{
	int torrentCount = 0;
	tr_torrent** torrentList;
	tr_ctor* ctor = tr_ctorNew(fTorrentSession);
	

	torrentList = tr_sessionLoadTorrents(fTorrentSession, ctor, &torrentCount);
	
	//
	//
	//
	for( int i = 0; i < torrentCount; i++ )
	{
		TorrentObject* torrentObject = new TorrentObject();
		
		//
		//
		//
		if( !torrentObject->LoadFromHandle(torrentList[i]) )
		{
			tr_torrentStop(torrentList[i]);
			delete torrentObject;
			continue;
		}
		
		tr_torrentSetCompletenessCallback(torrentList[i], TorrentCompletenessHandler, NULL);
		
		//
		//
		//
		fTorrentList.AddItem(torrentObject);
		
		//
		// If there's a MainWindow created, add torrent.
		//
		if( fMainWindow != NULL )
			fMainWindow->AddTorrent(torrentObject);			
	}
	
	tr_free( torrentList );
    tr_ctorFree( ctor );
}

//
//
//
void TorrentorApp::LoadTorrentFromFiles(const BStringList& TorrentPathList) 
{
	int32 stringCount = TorrentPathList.CountStrings();
	

	//
	for( int32 i = 0; i < stringCount; i++ ) 
	{	
		TorrentObject* torrentObject = new TorrentObject();
		
		//
		//
		//
		if( !torrentObject->LoadFromPath(Session(), TorrentPathList.StringAt(i)) )
		{
			delete torrentObject;
			continue;
		}		
		
		AddTorrentWindow* addTorrentWindow = new AddTorrentWindow(torrentObject);
		addTorrentWindow->Show();
	}
}

void TorrentorApp::OpenTorrentResult(BMessage* message)
{
	bool addTorrent = false;
	bool startTorrent = false;
	TorrentObject* torrentObject = NULL;
	
	message->FindBool("add", &addTorrent);
	message->FindBool("start", &startTorrent);
	
	//
	// We always need the torrent object.
	//
	if( message->FindPointer("torrent", reinterpret_cast<void**>(&torrentObject)) != B_OK )
		return;


	//
	//
	//
	if( addTorrent == false ) 
	{
		torrentObject->Remove(true);
		delete torrentObject;
		
		return;
	}
	
	fTorrentList.AddItem(torrentObject);
		
	if( startTorrent )
		torrentObject->StartTransfer();
		
	//
	// If there's a MainWindow created, add torrent.
	//
	if( fMainWindow != NULL )
		fMainWindow->AddTorrent(torrentObject);
}

void TorrentorApp::LoadTorrentFromMagnet(BMessage* message)
{
	BString magnetUrl;
	if( message->FindString("URL", &magnetUrl) != B_OK )
		return;
	
	LoadTorrentFromMagnet(magnetUrl);
}

void TorrentorApp::LoadTorrentFromMagnet(BString magnetUrl)
{
	TorrentObject* torrentObject = new TorrentObject();
		
	if( !torrentObject->LoadFromMagnet(Session(), magnetUrl) )
	{
		delete torrentObject;
		return;
	}
	
	AddTorrentWindow* addTorrentWindow = new AddTorrentWindow(torrentObject);
	addTorrentWindow->Show();
		
	//
	//
	//
	//fTorrentList.AddItem(torrentObject);
	//	
	//
	// If there's a MainWindow created, add torrent.
	//
	//if( fMainWindow != NULL )
	//	fMainWindow->AddTorrent(torrentObject);	
}

//
//
//
void TorrentorApp::OpenMagnetLinkWindow()
{
	AddMagnetLinkWindow* MagnetWindow = new AddMagnetLinkWindow();
	
	MagnetWindow->Show();
}

void TorrentorApp::OnTorrentComplete(BMessage* message)
{
	tr_torrent* torrent	= NULL;
	/*tr_completeness completeness =*/
	int32 completeness = 0;
	bool wasRunning = false;
	
	//
	//
	//
	if( message->FindPointer("torrentHandle", reinterpret_cast<void**>(&torrent)) != B_OK )
		return;
	
	if( message->FindInt32("torrentCompleteness", &completeness) != B_OK )
		return;
	
	if( message->FindBool("torrentRunning", &wasRunning) != B_OK )
		return;
	//
	//
	//
	if( completeness == TR_LEECH || !wasRunning )
		return;
		
	//
	//
	//
	TorrentObject* torrentObject = fTorrentList.EachElement(TorrentorApp::_findTorrentObject, reinterpret_cast<void*>(torrent));
	
	if( torrentObject == NULL )
		return;

	
	//
	//
	//

		
	//
	// Update the mime info.
	//
	for (unsigned int i = 0; i < torrentObject->Info()->fileCount; i++)
	{
		//
		//
		//
		BString filePath;
		filePath << torrentObject->DownloadFolder() 
				 << '/' 
				 << torrentObject->Info()->files[i].name;
			 
		//
		//
		//
		if (torrentObject->Info()->files[i].dnd == false)
			update_mime_info(filePath.String(), false, false, B_UPDATE_MIME_INFO_NO_FORCE);
	}
	
		
	//
	// Notify the user about the recently completed file/torrent.
	//
	BNotification* notification = new BNotification(B_INFORMATION_NOTIFICATION);
	
	
	BMimeType mime;
	torrentObject->MimeType(mime);
	notification->SetIcon(GetIconFromMime(mime.Type()));
	
	
	notification->SetGroup("Torrentor!");
	notification->SetTitle("Download complete");
	notification->SetContent( torrentObject->Name() );
	notification->Send();
}

void TorrentorApp::OnTorrentPauseStartAll(BMessage* message)
{
	bool pauseAll = false;
	
	if( message->FindBool("pause", &pauseAll) != B_OK )
		return;
		
	//
	//
	//
	fTorrentList.EachElement(TorrentorApp::_torrentPauseStartEach, reinterpret_cast<void*>(&pauseAll));
}

TorrentObject* TorrentorApp::_torrentPauseStartEach(TorrentObject* torrentObject, void* pauseParam)
{	
	bool pause = *reinterpret_cast<bool*>(pauseParam);
	
	if( pause )
		torrentObject->StopTransfer();
	else
		torrentObject->StartTransfer();
		
	return NULL;
}

TorrentObject* TorrentorApp::_findTorrentObject(TorrentObject* torrentObject, void* torrentParam)
{
	tr_torrent* torrent = static_cast<tr_torrent*>(torrentParam);
	
	if( torrentObject->Handle() == torrent )
		return torrentObject;
	
	return NULL;
}
