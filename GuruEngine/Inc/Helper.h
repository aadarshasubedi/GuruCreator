/*
Author:		Dan Strycharske
Date:
File:
Brief:		Shader source loading functions for OpenGL.
*/

#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <iostream>

#include "SDL.h"

#ifdef PURE_WINDOWS
	#include "glew.h"
#else
	#include "gl_inc.h"
	//#include "SDL_opengl.h"
#endif


class TextureDescriptor;


GLuint	load_shader_source( const char* filename, GLuint type, GLint program, char	program_string[16834], char	shader_info[16834]  );
GLuint	load_shader( GLuint type, char* source, GLint program, char	shader_info[16834]  );

void getRawTextureData(const char* bitMapFile, TextureDescriptor* desc = NULL);
GLuint loadTextureFromDescriptor( GLuint& texID, const TextureDescriptor& desc );  //By default you'd just get texture ID. 
GLuint loadTextureFromFile( const char* bitMapFile, GLuint& texID);
//GLuint loadTextureAlpha( const char* bitMapFile, const char* alphaBitMapFile, GLuint& texID );

#endif // HELPER_H_INCLUDED
