/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef DBITMAP_H_INCLUDED
#define DBITMAP_H_INCLUDED


#ifdef PURE_WINDOWS

#include <SDL.h>

#else

#include "SDL.h"
//#include "SDL_opengl.h"

#endif


#include <iostream>
#include <fstream>


//Define bitmap header struct.
/*
Bytes 0-1: Magic word. It contains the letters 'B' and 'M'.

Bytes 2-5: Double word. File size.
Bytes 6-10: No idea. Leave this zeroed.

Bytes 11-14: Double word. Start of the bitmap.
Bytes 15-18: Double word. Size of the header. Normally set to 40. The header is considered to start at offset 15.
Bytes 19-22: Double word. Width.
Bytes 23-26: Double word. Height.

Bytes 27-28: Word. Planes. Must be 1.
Bytes 29-30: Word. Bits per pixel. You'll need this as 24.

Bytes 31-34: Double word. Compression. Leave it as zero.
Bytes 35-38: Double word. Compressed size. Since there's no compression, this value will be the same as file size.
Bytes 39-42: Double word. Horizontal resolution. See below.
Bytes 43-46: Double word. Vertical resolution. Pixels per meter, according to Wikipedia. The values seem to vary, but 2835 should do for both.
Bytes 47-50: Double word. Number of colors in the palette. Leave as zero.
Bytes 51-54: Double word. "Important colors". Leave as zero.
Bytes 55-EOF: Bitmap.
*/


struct DBitMapHeader
{
	unsigned char magicHead; //2 bytes for 'B' and 'M'.
	unsigned int fileSize;
	unsigned int keepZero;
	unsigned int keepZero2; 		//default to 0
	unsigned int bitmapStart;		//default 55
	unsigned int headerSize; 		//default to 40
	unsigned int width;
	unsigned int height;
	unsigned int planes;			//default to 1
	unsigned int bytesPerPixel;    //24 for rgb.
	unsigned int compression;       //default 0
	unsigned int compessionSize;   //default to fileSize.
	unsigned int horizResolution;  //default 2835
	unsigned int vertResolution;   //default 2835
	unsigned int numColors;	    //default 0.
	unsigned int importantColors;  //default 0.
};


class DBitmap
{
private:
	//Bitmap has the pixel data and header.
	FILE* mBmpFile;
	char  mFileName[256];

	DBitMapHeader mHeaderInfo;
	//SDL_Surface* mScreen;

public:
	DBitmap();
	DBitmap(const char* name);
	DBitmap(const char* name, SDL_Surface* in_screen );
	~DBitmap();

	void WriteScreenToBitmap( const char* filename, SDL_Surface* screen );
	void WriteHeaderToFile(SDL_Surface* screen);
	void WriteColorDataToFile(SDL_Surface* screen);

};


#endif // DBITMAP_H_INCLUDED
