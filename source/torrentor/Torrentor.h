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
//	File Name:		Torrentor.h
//	Authors:		Guido Pola <prodito@live.com>
//	Description:	
//------------------------------------------------------------------------------
#ifndef TORRENTOR_H
#define TORRENTOR_H


#include <cstdio>
#include <cstdlib>

//
// Include libtransmission headers.
//
#include <libtransmission/transmission.h>
#include <libtransmission/variant.h>
#include <libtransmission/utils.h>


//
//
//
#include <interface/Bitmap.h>
#include <interface/GroupView.h>
#include <interface/GroupLayoutBuilder.h>
#include <interface/StatusBar.h>
#include <interface/StringView.h>
#include <interface/View.h>

#include <storage/FilePanel.h>
#include <storage/Mime.h>
#include <storage/MimeType.h>
#include <storage/NodeInfo.h>
#include <storage/Path.h>


#include <support/ObjectList.h>

#include <Catalog.h>

//
#define TR_DEFAULT_AUTO_ADD_TORRENT_FOLDER		"/boot/home/Desktop"

#define TR_PREFS_KEY_AUTO_ADD_TORRENT_ENABLED	"torrentor-auto-add-torrent-enabled"
#define TR_PREFS_KEY_AUTO_ADD_TORRENT_FOLDER	"torrentor-auto-add-torrent-folder"


//
//
//
#include "TorrentObject.h"


#endif // TORRENTOR_H
