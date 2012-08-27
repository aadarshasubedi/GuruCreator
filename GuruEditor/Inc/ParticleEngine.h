/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H


#include "ParticleGuru.h"


class ParticleEngine : public ParticleGuru
{	
	//This function will create the vbos and set the screen aligned quad.  No buffering for
	// the particle buffer since it will be filled during render.
	virtual void Init();

    //This function will push all particles into the master list, along with setting any necessary states.
	virtual void Begin(bool clearMasterList = true, bool sortParticles = false, CVector3 camPos = CVector3(0,0,0) );

    //The particle sorting will be performed here before any data sent to graphics card.
    //Here vertex attributes and any data sent to the shader will be done here.
    //The actual render call will also occur here.
	virtual void Render();

    //This will end the rendering and de-set any states set, and clear the master particles list.
	virtual void End();		

    //These are helper functions to open files,
    void    LoadVertexShader( const std::string& file );
    void    LoadFragmentShader( const std::string& file );

    //This performs the creating, linking and storing of the program within the particle guru.
    void    LoadFullShaderProgram( const std::string& vertFile, const std::string& fragFile );

	virtual ParticleSystem* CreateDemoSystem();

    GLuint GetParticlesVBO() const;
    GLuint GetQuadVBO_verts() const;
    GLuint GetQuadVBO_indices() const;
    GLuint GetShaderProgram() const;

    GLuint* GetTexturesListPtr();
    GLuint  GetTextureRef( unsigned index );
    void    AddTextureRef( GLuint texture );

private:	
	//The VBO that will hold the particle attributes.
	GLuint  mVboParticles;

	//The VBOs that will hold the vertex/index buffers for the Screen-Aligned Quad.
	GLuint  mVboQuadVerts;
	GLuint  mVboQuadIndices;

	//The shader program used for default particle rendering.  This can be changed for specific cases.
	GLuint  mShaderProgram;
	GLuint  mVertShader;
	GLuint  mFragShader;

	//The attributes for the shader.
	GLint   mPosAttrib;
	GLint   mTexAttrib;
	GLint   mPartPosAttrib;
	GLint   mPartOldAttrib;
	GLint   mColAttrib;
	GLint   mSizeAttrib;
	GLint   mRotAttrib;
	GLint   mAddAttrib;
	GLint   mAspectAttrib;
	GLint   mTextureUniform;

	//An array of the maximum number of textures the particle guru can load and store at one time.
	GLuint   mTextures[MAX_PARTICLE_TEXTURES];
};

#endif