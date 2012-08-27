/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#include "DBitmap.h"


DBitmap::DBitmap()
{
}


DBitmap::DBitmap(const char* name)
{
	strcpy_s( mFileName, name );
}


DBitmap::DBitmap(const char* name, SDL_Surface* in_screen )
{
	strcpy_s( mFileName, name );
	WriteScreenToBitmap( mFileName, in_screen );
}


DBitmap::~DBitmap()
{
}


void DBitmap::WriteScreenToBitmap( const char* , SDL_Surface* screen )
{
	//Open up the file for writing.
	//mBmpFile = fopen( filename, "wb" );

	//mBmpFile = fopen( "/home/dan/Untitled.bmp", "rb" );

	//call the WriteHeaderToFile after getting the necessary data from teh screen.
	WriteHeaderToFile(screen);

	//Then call WriteColorDataToFile, using the given screen pixels and stride.
	WriteColorDataToFile(screen);

	//fprintf( mBmpFile, "%c", '\0' );
	//fclose( mBmpFile );
}


void DBitmap::WriteHeaderToFile(SDL_Surface* )
{
	std::cout << sizeof( struct DBitMapHeader ) << std::endl;

	mHeaderInfo.magicHead = 'B'; //2 bytes for 'B' and 'M'.
	//mHeaderInfo.magicHead2 = 'M';
	mHeaderInfo.fileSize = 70; //screen->w * screen->format->BytesPerPixel + screen->h;
	mHeaderInfo.keepZero = 0; 		//default to 0
	mHeaderInfo.bitmapStart = 54;		//default 54
	mHeaderInfo.headerSize = 40; 		//default to 40
	mHeaderInfo.width = 2;
	mHeaderInfo.height = 2;
	mHeaderInfo.planes = 1;			//default to 1
	mHeaderInfo.bytesPerPixel = 24;    //24 for rgb.
	mHeaderInfo.compression = 0;       //default 0
	mHeaderInfo.compessionSize = 16;   //default to fileSize.
	mHeaderInfo.horizResolution = 2835;  //default 2835
	mHeaderInfo.vertResolution  = 2835;   //default 2835
	mHeaderInfo.numColors = 0;	    //default 0.
	mHeaderInfo.importantColors = 0;  //default 0.

	std::fstream outbmp;
	outbmp.open( "./Image01.bmp", std::ios_base::out );

	outbmp << (short)0x00004D42;
	outbmp << (long)0x00000046;
	outbmp << (short)0x00000000;
	outbmp << (short)0x00000000;
	outbmp << (long)0x00000036;
	outbmp << (long)0x00000028;
	outbmp << (long)0x00000002;
	outbmp << (long)0x00000002;
	outbmp << (short)0x00000001;
	outbmp << (short)0x00000018;
	outbmp << (long)0x00000000;
	outbmp << (long)0x00000010;
	outbmp << (long)0x0000130B;
	outbmp << (long)0x0000130B;
	outbmp << (long)0x00000000;
	outbmp << (long)0x00000000;

	outbmp.close( );
}


void DBitmap::WriteColorDataToFile(SDL_Surface* )
{
}
