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
//	File Name:		TorrentPreferences.cpp
//	Authors:		Guido Pola <prodito@live.com>
//	Description:	
//------------------------------------------------------------------------------
#include <libtransmission/transmission.h>

#include "Torrentor.h"
#include "TorrentPreferences.h"

TorrentPreferences::TorrentPreferences() 
	:	fSession(NULL)
{
	tr_variantInitDict(&fHandle, 2);
	
	//
	//
	//
	tr_variantDictAddBool(&fHandle, TR_KEY_watch_dir_enabled, false);
 	//tr_variantDictAddStr(&fHandle, TR_PREFS_KEY_AUTO_ADD_TORRENT_FOLDER, TR_DEFAULT_AUTO_ADD_TORRENT_FOLDER);
	
	tr_sessionLoadSettings(&fHandle, tr_getDefaultConfigDir("Torrentor"), "Torrentor");
}

TorrentPreferences::~TorrentPreferences()
{
	tr_variantFree(&fHandle);
}


void TorrentPreferences::Save()
{
	tr_sessionGetSettings(fSession, &fHandle);
	tr_sessionSaveSettings(fSession, tr_getDefaultConfigDir("Torrentor"), &fHandle);
}

#pragma mark - Get config value

bool TorrentPreferences::StartWhenAddedEnabled()
{
	return !tr_sessionGetPaused(fSession);	
}

bool TorrentPreferences::AutoAddTorrentEnabled()
{
	bool result = false;
	
	if (tr_variantDictFindBool(&fHandle, TR_KEY_watch_dir_enabled, &result))
		return result;
	
	return false;
}

BString	TorrentPreferences::AutoAddTorrentPath()
{
	const char* result = B_EMPTY_STRING;
	
	tr_variantDictFindStr(&fHandle, TR_KEY_watch_dir, &result, NULL);
	
	return result;
}


bool TorrentPreferences::IncompleteFileNamingEnabled()
{
	return tr_sessionIsIncompleteFileNamingEnabled(fSession);
}

BString TorrentPreferences::DownloadFolder()
{
	return tr_sessionGetDownloadDir(fSession);	
}

bool TorrentPreferences::IncompleteFolderEnabled()
{
	return tr_sessionIsIncompleteDirEnabled(fSession);
}

BString TorrentPreferences::IncompleteFolder()
{
	return tr_sessionGetIncompleteDir(fSession);	
}

uint16 TorrentPreferences::IncomingPort()
{
	return tr_sessionGetPeerPort(fSession);	
}

bool TorrentPreferences::PortForwardingEnabled()
{
	return tr_sessionIsPortForwardingEnabled(fSession);	
}

uint16 TorrentPreferences::PeerLimit()
{
	return tr_sessionGetPeerLimit(fSession);	
}

uint16 TorrentPreferences::PeerLimitPerTorrent()
{
	return tr_sessionGetPeerLimitPerTorrent(fSession);	
}

bool TorrentPreferences::DistributedHashTableEnabled()
{
	return tr_sessionIsDHTEnabled(fSession);
}

bool TorrentPreferences::PeerExchangeEnabled()
{
	return tr_sessionIsPexEnabled(fSession);	
}

bool TorrentPreferences::MicroTransportProtocolEnabled()
{
	return tr_sessionIsUTPEnabled(fSession);
}

bool TorrentPreferences::LocalPeerDiscoveryEnabled()
{
	return tr_sessionIsLPDEnabled(fSession);
}

tr_encryption_mode TorrentPreferences::EncryptionMode()
{
	return tr_sessionGetEncryption(fSession);
}


#pragma mark - Set config value

void TorrentPreferences::StartWhenAddedToggle()
{
	tr_sessionSetPaused(fSession, StartWhenAddedEnabled());
}

void TorrentPreferences::IncompleteFileNamingToggle()
{
	tr_sessionSetIncompleteFileNamingEnabled(fSession, !IncompleteFileNamingEnabled());
}

void TorrentPreferences::IncompleteFolderToggle()
{
	tr_sessionSetIncompleteDirEnabled(fSession, !IncompleteFolderEnabled());
}

void TorrentPreferences::SetDownloadFolder(BString path)
{
	tr_sessionSetDownloadDir(fSession, path.String());
}

void TorrentPreferences::SetIncompleteFolder(BString path)
{
	tr_sessionSetIncompleteDir(fSession, path.String());
}

void TorrentPreferences::SetIncomingPort(uint16 port)
{
	tr_sessionSetPeerPort(fSession, port);
}

uint16 TorrentPreferences::SetIncomingRandomPort()
{
	return tr_sessionSetPeerPortRandom(fSession);	
}

void TorrentPreferences::PortForwardingToggle()
{
	tr_sessionSetPortForwardingEnabled(fSession, !PortForwardingEnabled());
}

void TorrentPreferences::SetPeerLimit(uint16 limit)
{
	tr_sessionSetPeerLimit(fSession, limit);
}

void TorrentPreferences::SetPeerLimitPerTorrent(uint16 limit)
{
	tr_sessionSetPeerLimitPerTorrent(fSession, limit);
}

void TorrentPreferences::DistributedHashTableToggle()
{
	tr_sessionSetDHTEnabled(fSession, !DistributedHashTableEnabled());
}

void TorrentPreferences::PeerExchangeToggle()
{
	tr_sessionSetPexEnabled(fSession, !PeerExchangeEnabled());	
}

void TorrentPreferences::MicroTransportProtocolToggle()
{
	tr_sessionSetUTPEnabled(fSession, !MicroTransportProtocolEnabled());
}

void TorrentPreferences::LocalPeerDiscoveryToggle()
{
	tr_sessionSetLPDEnabled(fSession, !LocalPeerDiscoveryEnabled());
}

void TorrentPreferences::SetEncryptionMode(tr_encryption_mode mode)
{
	tr_sessionSetEncryption(fSession, mode);
}


