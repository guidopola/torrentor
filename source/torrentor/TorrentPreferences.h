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
//	File Name:		TorrentPreferences.h
//	Authors:		Guido Pola <prodito@live.com>
//	Description:	
//------------------------------------------------------------------------------
#ifndef TORRENTOR_TORRENT_PREFERENCES_H
#define TORRENTOR_TORRENT_PREFERENCES_H

struct tr_variant;
struct tr_session;

class TorrentPreferences
{
public:
	TorrentPreferences();
	~TorrentPreferences();
	
	void Save();
	
	//
	bool	StartWhenAddedEnabled();
	bool 	AutoAddTorrentEnabled();
	BString	AutoAddTorrentPath();
	bool	IncompleteFileNamingEnabled();
	BString DownloadFolder();
	bool	IncompleteFolderEnabled();
	BString IncompleteFolder();
	uint16	IncomingPort();
	bool	PortForwardingEnabled();
	uint16 	PeerLimit();
	uint16	PeerLimitPerTorrent();
	bool	DistributedHashTableEnabled();
	bool	PeerExchangeEnabled();
	bool	MicroTransportProtocolEnabled();
	bool	LocalPeerDiscoveryEnabled();
	tr_encryption_mode	EncryptionMode();
	
	//
	void	StartWhenAddedToggle();
	void 	IncompleteFileNamingToggle();
	void	IncompleteFolderToggle();
	void	SetDownloadFolder(BString path);
	void	SetIncompleteFolder(BString path);
	void	SetIncomingPort(uint16 port);
	uint16	SetIncomingRandomPort();
	void	PortForwardingToggle();
	void 	SetPeerLimit(uint16 limit);
	void	SetPeerLimitPerTorrent(uint16 limit);
	void	DistributedHashTableToggle();
	void	PeerExchangeToggle();
	void	MicroTransportProtocolToggle();
	void	LocalPeerDiscoveryToggle();
	void	SetEncryptionMode(tr_encryption_mode mode);
	
	void SetSession(tr_session* session) { fSession = session; }
	tr_variant*	Handle() { return &fHandle; }
private:
	tr_session*	fSession;
	tr_variant	fHandle;
};


#endif // TORRENTOR_TORRENT_PREFERENCES_H
