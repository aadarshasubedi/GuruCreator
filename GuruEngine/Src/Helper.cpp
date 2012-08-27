/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       Helper.cpp
Brief:      
/************************************************************************/

#include "Helper.h"
#include "AssetDescriptor.h"
#include <iostream>


GLuint	load_shader( GLuint type, char* source, GLint program, char	shader_info[16834] )
{
	GLuint shaderNum;
	GLsizei length;

	//Create the shader object.
	shaderNum = glCreateShader(type );

	//Load the shader object with the given source.
	glShaderSource( shaderNum, 1, const_cast<const GLcharARB**>(&source), NULL );

	//Compile given shader.
	glCompileShader( shaderNum );

	glGetInfoLogARB( shaderNum, 16834, &length, shader_info );

	std::cerr << shader_info << std::endl;

	//Attach to program.
	glAttachShader(program, shaderNum );

	return shaderNum;
}


GLuint	load_shader_source( const char* filename, GLuint type, GLint program, char program_string[16843], char	shader_info[16834] )
{
	FILE* fp;
	unsigned int len;

	errno_t error = fopen_s(&fp, filename, "r" );
	if( error ) { return 0; }

	if( fp != NULL )
	{	
		len = fread( program_string, 1, 16834, fp );
	}
	else
	{
		return 0;
	}
	program_string[len] = '\0';
	fclose(fp);

	//Here this call will compile, link, and get this shader ready and attach it to the
	// main program.
	return load_shader( type, program_string, program, shader_info );
}


GLuint loadTextureFromFile( const char* bitMapFile, GLuint& texID )
{
	//mTextureId represents the texture ID for this file being loaded in.

	//A temporary surface to tell us the fun stuff about the bmp like
	// number of channels, data representation, etc.
	SDL_Surface* tempSurf;

	//A texture format enumeration provided by GL
	//Setting a default,
	GLenum texture_format = GL_RGBA;
	GLint numColors;

	tempSurf = SDL_LoadBMP( bitMapFile );

	if( !tempSurf )
	{
		//There was an error in loading the texture. 
		//Throw an exception or output to the logger that there was a bad texture read.
		std::cout << "Bad texture load: " << bitMapFile << std::endl;
	}

	//In here, we figure out the texture, and then have openGL allocate room for it.
	//First check and see if the image is a power of 2, both width and height.

	if( (tempSurf->w & (tempSurf->h - 1)) != 0 )
	{
		//Not a power of 2.
	}

	if( (tempSurf->h & (tempSurf->h - 1)) != 0 )
	{
		//Not a power of 2.
	}


	//Now find out the color format, as well as the color order.

	numColors = tempSurf->format->BytesPerPixel;

	if( 4 == numColors )
	{
		//RGBA....  check if it's actually BGRA
		if( tempSurf->format->Rmask == 0x000000ff )
		{
			//Means red is first.
			//Already been set default to this, no alterations.
		}
		else
		{
			texture_format = GL_BGRA;
		}
	}
	else if( 3 == numColors )
	{
		//RGB....  or again, check if BGR.

		//RGBA....  check if it's actually BGRA
		if( tempSurf->format->Rmask == 0x000000ff )
		{
			//Means red is first.
			texture_format = GL_RGB;
		}
		else
		{
			texture_format = GL_BGR;
		}
	}
	else
	{
		//2 or less color channels so this will likely be bad.
	}



	//Now comes OpenGL's turn to allocate the room and give us the texture ID.
	glGenTextures( 1, &texID );

	//Bind the texture.
	glBindTexture( GL_TEXTURE_2D, texID );

	//And here tell it how to minify and maxify the texture.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, numColors, tempSurf->w, tempSurf->h, 0, texture_format, GL_UNSIGNED_BYTE, tempSurf->pixels );

	// Once the texture is in GL's pool, we can get rid of the SDL_Surface.

	if( tempSurf )
	{
		SDL_FreeSurface( tempSurf );
	}
	return texID;
}







void getRawTextureData(const char* bitMapFile, TextureDescriptor* desc)
{
	//If the descriptor is NULL, make a temp one on stack.	
	TextureDescriptor* loopDesc = desc;
	if( !loopDesc )
	{
		loopDesc = new TextureDescriptor();
	}

	//mTextureId represents the texture ID for this file being loaded in.

	//A temporary surface to tell us the fun stuff about the bmp like
	// number of channels, data representation, etc.
	SDL_Surface* tempSurf;
	tempSurf = SDL_LoadBMP( bitMapFile );

	if( !tempSurf )
	{
		//There was an error in loading the texture. 
		//Throw an exception or output to the logger that there was a bad texture read.
		std::cout << "Bad texture load: " << bitMapFile << std::endl;
	}

	//In here, we figure out the texture, and then have openGL allocate room for it.
	//First check and see if the image is a power of 2, both width and height.

	if( (tempSurf->w & (tempSurf->h - 1)) != 0 )
	{
		//Not a power of 2.
	}

	if( (tempSurf->h & (tempSurf->h - 1)) != 0 )
	{
		//Not a power of 2.
	}


	//Now find out the color format, as well as the color order.

	// Once the texture is in GL's pool, we can get rid of the SDL_Surface.
	desc->mWidth = tempSurf->w;
	desc->mHeight = tempSurf->h;
	desc->mNumColors = tempSurf->format->BytesPerPixel;
	desc->mPixelFormat = *(tempSurf->format);
	desc->mFlags = tempSurf->flags;
	if( desc->mPixels == NULL)
	{
		desc->mPixels = (void*)new char[tempSurf->w * tempSurf->h * desc->mNumColors];
		std::memcpy(desc->mPixels, tempSurf->pixels, tempSurf->w * tempSurf->h * desc->mNumColors);
	}

	if( tempSurf )
	{
		SDL_FreeSurface( tempSurf );
	}
}


GLuint loadTextureFromDescriptor( GLuint& texID, const TextureDescriptor& desc )
{
	//Now comes OpenGL's turn to allocate the room and give us the texture ID.
	glGenTextures( 1, &texID );

	//Bind the texture.
	glBindTexture( GL_TEXTURE_2D, texID );

	//And here tell it how to minify and maxify the texture.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	GLenum texture_format = GL_RGBA;

	if( 4 == desc.mNumColors )
	{
		//RGBA....  check if it's actually BGRA
		if( desc.mPixelFormat.Rmask == 0x000000ff )
		{
			//Means red is first.
			//Already been set default to this, no alterations.
		}
		else
		{
			texture_format = GL_BGRA;
		}
	}
	else if( 3 == desc.mNumColors )
	{
		//RGB....  or again, check if BGR.

		//RGBA....  check if it's actually BGRA
		if( desc.mPixelFormat.Rmask == 0x000000ff )
		{
			//Means red is first.
			texture_format = GL_RGB;
		}
		else
		{
			texture_format = GL_BGR;
		}
	}
	else
	{
		//2 or less color channels so this will likely be bad.
	}

	glTexImage2D( GL_TEXTURE_2D, 0, desc.mNumColors, desc.mWidth, desc.mHeight, 0, texture_format, GL_UNSIGNED_BYTE, desc.mPixels );

	return texID;
}

