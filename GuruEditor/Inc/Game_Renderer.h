/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H


#include <vector>
#include "SDL.h"
#include "CRenderModule.h"
#include "ParticleGuru.h"


class CSprite2D;
class GameObject;
class AssetManager;
class Model;


class Game_Renderer : public CRenderModule
{
public:		
	Game_Renderer(RMOptions ops);

public:
	virtual ~Game_Renderer();

	//Virtual functions coming from CRenderModule.
    virtual void init();
    virtual void initWindow();
	virtual void initTextures();
	virtual void initShaders();
	virtual void initRenderObjects(); 

	//These same functions but more generic load/run-time.
	virtual void textureLoad( const char* fileLocale );  //Loads a single texture specified by fileLocale.
	virtual void textureDirLoad( const char* directory );   //Loads any supported textures from directory.  Useful for anim textures and level loads.

	virtual void vertShaderLoad( const char* fileLocale );  //Loads a single texture specified by fileLocale.
	virtual void vertShaderDirLoad( const char* directory );   //Loads any supported textures from directory.

	virtual void fragShaderLoad( const char* fileLocale );  //Loads a single texture specified by fileLocale.
	virtual void fragShaderDirLoad( const char* directory );   //Loads any supported textures from directory.

	virtual void programShaderLoad( const char* fileLocale );  //Loads and creates a program based on the 
	virtual void programShaderDirLoad( const char* directory );   //Loads any supported textures from directory.

	virtual void modelLoad( const char* fileLocale );  //Loads a single texture specified by fileLocale.
	virtual void modelDirLoad( const char* directory );   //Loads any supported textures from directory.
	
    virtual void BeginRender();
	virtual void render();
	virtual void EndRender();

public:
	//The various window functions for when scope is lost.
    virtual void close();
    virtual void refresh();

	//This closes everything down, and basically calls all the init functions again.
    virtual void reload();
	
	//Retrieve the Camera.  This should be public for sure.
	//Camera* GetCamera() { return &mCamera; }
	//Camera	mCamera;


	void AddTime( float amount );

	GLuint*	GetFirstTexture()
	{
		return (mNumTextures) ? mTextures : NULL;
	}


	//TriangleMan mThePlayer;
	GLuint* mTextures;
	GLsizei mNumTextures;

public:

	//A private helper to draw the axes when this is desired.
	void DrawWorldAxes();

	
	/////////////////////////////////////////////////////////////
	//2D drawing subset...........................
	//////////////////////////////////////////////////////////////
	//For drawing a simple colored box on screen for a frame.  Useful for things that don't live long.
	void DrawSAQ_Colored( float width, float height, const CVector3& pos, CVector3 color, unsigned zDepth = 0 );
	//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
	void DrawSAQ_Top_Colored( float width, float height, const CVector3& pos, CVector3 color );
	
	//For drawing a simple colored box on screen for a frame.  Useful for things that don't live long.
	void DrawSAQBorder_Colored( float width, float height, const CVector3& pos, CVector3 color, unsigned zDepth = 0 );
	//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
	void DrawSAQBorder_Top_Colored( float width, float height, const CVector3& pos, CVector3 color );

	//Draws a textured object at a given location and of a certain size.
	void DrawSAQ_Textured( float width, float height, const CVector3& pos, GLuint textureID, unsigned zDepth = 0 );
	//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
	void DrawSAQ_Top_Textured( float width, float height, const CVector3& pos, GLuint textureID );
	

	//These Triangle drawing functions are for equilateral triangles.
	//For drawing a simple colored box on screen for a frame.  Useful for things that don't live long.
	void DrawSATri_Colored( float inscribedRadius, const CVector3& pos, CVector3 colorl, unsigned zDepth = 0 );
	//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
	void DrawSATri_Top_Colored( float inscribedRadius, const CVector3& pos, CVector3 color );

	//Draws a textured object at a given location and of a certain size.
	void DrawSATri_Textured( float inscribedRadius, const CVector3& pos, GLuint textureID, unsigned zDepth = 0 );
	//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
	void DrawSATri_Top_Textured( float inscribedRadius, const CVector3& pos, GLuint textureID );
	
	//Draws a textured object at a given location and of a certain size.
	static void DrawSACircleBorder_Colored( float inscribedRadius, const CVector3& pos, const CVector3& color, unsigned grain = 8, unsigned lineType = GL_LINE_LOOP, unsigned zDepth = 0 );
	
	static void DrawSACircleFill_Colored( float inscribedRadius, const CVector3& pos, const CVector3& color, unsigned grain = 8, unsigned lineType = GL_POLYGON, unsigned zDepth = 0 );

	//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
	void DrawSACircle_Top_Colored( float inscribedRadius, const CVector3& pos, const CVector3& color, unsigned grain = 8 );

	//Draws a textured object at a given location and of a certain size.
	void DrawSACircle_Textured( float inscribedRadius, const CVector3& pos, GLuint textureID, unsigned zDepth = 0 );
	//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
	void DrawSACircle_Top_Textured( float inscribedRadius, const CVector3& pos, GLuint textureID );


	//More specific.
	void DrawSAQ_Sprite( CSprite2D sprite, unsigned zDepth = 0 );
	//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
	void DrawSAQ_Top_Sprite( CSprite2D sprite, const CVector3& pos );


	//Test function for rendering the "Triangle Man."
	void Draw_Triangle_Man( float inscribedRadius = 20, const CVector3& pos = CVector3(0,0,0) );


	//This is a special function that will draw a path as well as a filled circle on top of that path.  The circle will
	// go around the path over a period of 4 seconds.
	void Draw_Ball_On_Path( const CVector3& pos, float radiusPath, float radiusBall);  //The current time is in seconds.  


	static float ballTimeLimit;
	static float ballCurTime;

	static float triTimeLimit;
	static float triCurTime;


private: 
	//This renderer will hold all of the SDL Specific junk that I'm already using.
    //This is protected so any inheriting class based on SDL can manipulate the surface
    // still however they need.
	SDL_Surface* mScreenSurface;

    ParticleGuru* mParticleMaster;

    unsigned mNumObjects;
    std::list<GameObject*>	mObjects;

    //Material format.
    unsigned mBackColor[3];

    GLuint	mProgram;
    GLuint	mVertShader;
	GLuint  mFragShader;

	//Model* sphere;
	//Model* cube;
	//Model* cylinder;
	//Model* capsule;
};


#endif


