/*
Author:		Dan Strycharske
Date:
File:
Brief:

Here will be the pure virtual functions needing to be implemented by other derived classes.
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void close() = 0;
	virtual void reload() = 0;
	virtual void refresh() = 0;
*/

#include "CRenderModule.h"
#include "AssetManager.h"
#include "Camera.h"
#include "Engine.h"
#include "GPicker.h"
#include <cassert>
#include <cmath>


CRenderModule::CRenderModule( )
{
	mCamera = new Camera();

    //Set default resolution values and such.
    mModuleOptions.mWidth = 800;
    mModuleOptions.mHeight = 600;
    mModuleOptions.mRenderOptions = 0;
    mModuleOptions.mRenderOptions |= 1;/*Fullscreen = yes*/
}


CRenderModule::CRenderModule( RMOptions options ) : mCamera(new Camera(options))
{
    //Set default resolution values and such.
    mModuleOptions.mWidth = options.mWidth;
    mModuleOptions.mHeight = options.mHeight;
	mModuleOptions.mRenderOptions = options.mRenderOptions;
}


CRenderModule::~CRenderModule()
{
	delete mCamera;
}


Camera* CRenderModule::GetCamera()
{
	return mCamera;
}


//POinter to asset manager.  Public for now @TODO make protected and have accessor so Engine can set it.
void CRenderModule::SetAssetManager(AssetManager* manager)
{ 
	mAssetManager = manager; 
}


AssetManager* CRenderModule::manager() 
{ 
	return mAssetManager;
}
