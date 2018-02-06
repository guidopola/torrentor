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
//	File Name:		TorrentObject.h
//	Authors:		Guido Pola <prodito@live.com>
//	Description:	
//------------------------------------------------------------------------------
#ifndef TORRENTOR_TORRENT_OBJECT_H
#define TORRENTOR_TORRENT_OBJECT_H

class BHandler;
class BMimeType;
class BString;


class TorrentObject
{
public:
	TorrentObject();
	~TorrentObject();
	
	bool LoadFromHandle(tr_torrent* TorrentHandle);
	bool LoadFromPath(const tr_session* Session, const char* TorrentPath);
	bool LoadFromMagnet(const tr_session* Session, const char* MagnetURL);

	
	void Update();
	
	//
	//
	//
	bool IsActive() const { return Statistics()->activity != TR_STATUS_STOPPED; }
	bool IsComplete() const { return Progress() >= 1.0; }
	
	
	float Progress() const { return Statistics()->percentComplete; }
	
	bool IsAnyErrorOrWarning() const;
	
	int SecondsDownloading() const;
	int SecondsSeeding() const;
	
	//
	//
	//
	void SetMetadataCallbackHandler(BHandler* MessageHandler);
	
	//
	//
	//
	//bool IsSeeding() const;
	
	//
	//
	//
	//bool IsChecking() const;
	
	bool IsMagnet() const;
	
	//
	// Return true if the torrent file is a folder.
	//
	bool IsFolder() const { return Info()->isFolder; }
	
	//
	//
	//
	void StopTransfer();
	
	//
	//
	//
	void StartTransfer();
	
	void Remove(bool DeleteFiles = false) const;
	
	
	BString Name() const;
	BString DownloadFolder() const;
	BString ErrorMessage() const;
	
	//
	//
	//
	void MimeType(BMimeType& mime);
	
	//
	//
	//
	const tr_stat* Statistics() const { return tr_torrentStatCached(fTorrentHandle); }
	
	//
	//
	//
	const tr_info* Info() const { return fInfo; }
	
	//
	//
	//
	const tr_torrent* Handle() const { return fTorrentHandle; }
	
	//
	//
	//
	const BHandler* MetadataHandler() const { return fMetadataHandler; }
	
protected:
	//
	static void TorrentMetadataCallback(tr_torrent* torrent, void* data);
private:
	tr_torrent* 	fTorrentHandle;
	const tr_stat*	fStatistics;
	const tr_info*	fInfo;
	BHandler*		fMetadataHandler;
};


#endif // TORRENTOR_TORRENT_OBJECT_H
