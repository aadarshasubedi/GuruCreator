/************************************************************************
Author:		Dan Strycharske
Date:       02.09.12
File:       AssetDescriptor.cpp
Brief:      
/************************************************************************/

#include "AssetDescriptor.h"


TextureDescriptor::TextureDescriptor() : AssetDescriptor(TEXTURE)
{
	init();
}


TextureDescriptor::TextureDescriptor(std::string filename) : AssetDescriptor(filename, TEXTURE), mFilename(filename)
{
	init(mFilename);
}


TextureDescriptor::~TextureDescriptor()
{
	delete mPixels;
}


void TextureDescriptor::init(std::string filename)
{
	mFilename = filename;
	mPixelFormat = SDL_PixelFormat();
	mWidth = 0;
	mHeight = 0;
	mNumColors = 0;
	mFlags = 0;
	mPixels = NULL;
	mId = 0;
}
