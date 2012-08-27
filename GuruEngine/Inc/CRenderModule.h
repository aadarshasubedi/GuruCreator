/*
Author:		Dan Strycharske
Date:		06.05.09
File:		CRenderer.h
Brief:      Base virtual Rendering class.  This will be a class that defines a window, the
            rendering definitions (vertex declarations, mesh type, etc. ).  This will be a
            singleton class, as only one rendering object will be valid at once.  For split
            screen, we'll split the window and render twice for now.

            The renderer contains the window rendering device, the graphics device parameters,
            and all of the specifications for rendering objects on screen.

            The virtual functions will be:

            init()    // for initializing render system.
            render()  // for rendering scene
            close()   // for game finish/exit
            reload()  // for game reset
            refresh() // for alt+tab

            Regardless of the renderer, the base resource location needs to be defined here.
            This directory will be split into the various renderers specific objects, and the
            global shared resoureces.
*/

#ifndef RENDERER_EMPTY_INCLUDED
#define RENDERER_EMPTY_INCLUDED

#include "Utility.h"
#include "gl_inc.h"
#include "GPicker.h"
#ifdef USE_LUA
#include "LunarHelper.h"
#endif


class Camera;
class AssetManager;
class Model;


struct VertexData
{
	GLfloat position[3];
	GLfloat color[3];
	GLfloat tex[2];
	GLfloat normal[3];
};


struct ParticleVertexData
{
	GLfloat position[3];
	GLfloat tex[2];
};


class CRenderModule
{
public:

	static void GLMatrixToFloatMatrix(GLfloat* glmatrix, float* matrix )
	{
		for( unsigned int iter = 0; iter < 16; ++iter )
		{
			matrix[iter] = glmatrix[iter];
		}
	}

	struct RenderModuleOptions
	{
	public:
		// window parameters
		int mWidth, mHeight;
		unsigned mBackgroundColor[3];

		// values set by derived renderer
		int mMaxLights;
		int mMaxTextures;
		int mMaxStencilIndices;

		int mRenderOptions; //This will be a bitmask for the various boolean values.
		//Wireframe
		//FullScreen;

		unsigned mFPS;
	};
	typedef RenderModuleOptions RMOptions;

	enum Modes
	{
		GURU_FULLSCREEN_MODE = 1,
		GURU_WIREFRAME_MODE = 2,
	};

	void SetWireframe() { mModuleOptions.mRenderOptions |= GURU_WIREFRAME_MODE; }
	void ToggleWireFrame() { mModuleOptions.mRenderOptions ^= GURU_WIREFRAME_MODE; }

    //This enumeration denotes the supported renderers in the application.  So far it's GL_SDL
    enum
    {
        RM_OPENGL,
        RM_OPENGL_SDL,
        RM_DIRECTX,
        RM_NUMBER
    };
	static int GetModuleType();

	std::vector< Model* > mModels;

public:
	//Constructors.  Default will use values I specify.  Otherwise use the module options coming in.  These
	// are loaded from a file, so this is already contained in the base engine class, the parsing at least.   
	CRenderModule( ); 
	CRenderModule( RenderModuleOptions options );
    virtual ~CRenderModule();

	//Retrieve the Camera.  This should be public for sure.
	Camera* GetCamera();

	//POinter to asset manager.  Public for now @TODO make protected and have accessor so Engine can set it.
	void SetAssetManager(AssetManager* manager);
	AssetManager* manager();
	
    //Here will be the pure virtual functions needing to be implemented by other derived classes.

	///This set are all all initialization time loading functions.  Other functions exist to unload singluar assets or systes, as well as check if they exist.
	// There will also be functions to load assets using locations, or loading all assets from a single or multiple pre-defined directories.
    virtual void init() = 0;
    virtual void initWindow() = 0;		
	virtual void initTextures() = 0;
	virtual void initShaders() = 0;
	virtual void initRenderObjects() = 0; 

	//These same functions but more generic load/run-time.
	virtual void textureLoad( const char* fileLocale ) = 0;  //Loads a single texture specified by fileLocale.
	virtual void textureDirLoad( const char* directory ) = 0;   //Loads any supported textures from directory.  Useful for anim textures and level loads.

	virtual void vertShaderLoad( const char* fileLocale ) = 0;  //Loads a single texture specified by fileLocale.
	virtual void vertShaderDirLoad( const char* directory ) = 0;   //Loads any supported textures from directory.

	virtual void fragShaderLoad( const char* fileLocale ) = 0;  //Loads a single texture specified by fileLocale.
	virtual void fragShaderDirLoad( const char* directory ) = 0;   //Loads any supported textures from directory.

	virtual void programShaderLoad( const char* fileLocale ) = 0;  //Loads and creates a program based on the 
	virtual void programShaderDirLoad( const char* directory ) = 0;   //Loads any supported textures from directory.

	virtual void modelLoad( const char* fileLocale ) = 0;  //Loads a single texture specified by fileLocale.
	virtual void modelDirLoad( const char* directory ) = 0;   //Loads any supported textures from directory.
		
	virtual void BeginRender() = 0;
    virtual void render() = 0;
	virtual void EndRender() = 0;

	//The various window functions for when scope is lost.
    virtual void close() = 0;
    virtual void refresh() = 0;

	//This closes everything down, and basically calls all the init functions again.
	virtual void reload() = 0;

protected:
    //This is to allow only one renderer to be allowed at a time.
	static int module_type;


	//The render module will define only a few things internally in the base.
	//These values are:  window resolution, fullscreen, vsync, desired fps,
	//                   anti-aliasing level, back buffer color, and root resource
	//                   directory.
	RenderModuleOptions   mModuleOptions;

	Camera*	mCamera;
	AssetManager* mAssetManager;
};




#endif // RENDERER_EMPTY_INCLUDED


