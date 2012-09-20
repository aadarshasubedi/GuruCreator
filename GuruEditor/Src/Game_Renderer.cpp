/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

//
#include "Game_Renderer.h"
#include "Game_Engine.h"
#include "GameObject.h"
#include "Game_Picker.h"
#include "InputGuru.h"
#include "ParticleEngine.h"
#include "Model.h"
#include "ModelUtility.h"
#include "RenderComponent.h"
#include "AssetManager.h"
#include "AssetDescriptor.h"
#include "Helper.h"
#include "AssetHandler.h"
#include "glut.h"


float Game_Renderer::ballTimeLimit = 8.0f;
float Game_Renderer::ballCurTime = 0.0f;

float Game_Renderer::triTimeLimit = 12.0f;
float Game_Renderer::triCurTime = 0.0f;


Game_Renderer::Game_Renderer( RMOptions ops ) : CRenderModule(ops)
{
	mParticleMaster = NULL;
	mNumTextures = 0;
	mTextures = NULL;
}


Game_Renderer::~Game_Renderer()
{
	delete mParticleMaster;

	glDeleteTextures(mNumTextures, mTextures);
	delete mTextures;
}


void Game_Renderer::init()
{
	//Tells SDL that opengl will be communicated with.
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); //Set SDL to have to swap buffers.
	mScreenSurface = SDL_SetVideoMode( mModuleOptions.mWidth, mModuleOptions.mHeight, 0, SDL_OPENGL);

#ifdef WIN32

    //Intialize and bind all of the GLEW's library functions and GL's functions so that the GL state can be used and accessed.
	GLenum error = glewInit();

	if( error )
	{
		//Figure out what the error was.
	}

#endif

	//////
	////// OPENGL state setup.
	////
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );

	//Set the blending type for alpha values, used for particles mostly.
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//clear buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//mCamera->ChangeProjectionType( true );
	//mCamera->SetZoomFactor2D( 6.0f );
	mCamera->mWinHeight = mModuleOptions.mHeight;
	mCamera->mWinWidth = mModuleOptions.mWidth;

	//Set up modelview matrix.
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	mTextures = new GLuint[2];

	TextureDescriptor desc("./../GuruResource/textures/MyTex.bmp");
	getRawTextureData("./../GuruResource/textures/MyTex.bmp", &desc);

	//Now instead of calling load here, call the AssetManager to load, and get back the Handler which
	// will contain the texture id.
	mAssetManager->CreateTextureAsset(desc);
	auto texHandle = mAssetManager->GetTextureAsset(desc.mName);

	GLuint newTexID = texHandle->getAsset()->mTextureId;

	if( newTexID > 0 )
	{
		glActiveTexture( newTexID );
	}

	initRenderObjects();
}


void Game_Renderer::initWindow()
{
	//Set the backbuffer's clear color (background basically)
	glClearColor( 0.2f, 0.25f, 0.35f, 0.f );

	//Camera load.
	mBackColor[0] = mBackColor[1] = mBackColor[2] = 0xFFFFFFFF;

	//Set up the window and size.
	glViewport( 0, 0, mModuleOptions.mWidth, mModuleOptions.mHeight );
}


void Game_Renderer::initTextures()
{
}


void Game_Renderer::initShaders()
{
}


void Game_Renderer::initRenderObjects()
{
}


void Game_Renderer::AddTime( float  )
{
}


void Game_Renderer::BeginRender()
{
	//Firstly clear the buffer before rendering the new frame.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	mCamera->SetModelView();
	mCamera->SetPerspective();
}


void Game_Renderer::EndRender()
{
	SDL_GL_SwapBuffers();
}


void Game_Renderer::render()
{
	//Draw the world axes for global reference.
	DrawWorldAxes();

	int fillType = GL_FILL;
	if( mModuleOptions.mRenderOptions & GURU_WIREFRAME_MODE )
	{
		fillType = GL_LINE;
	}

	std::for_each(begin(mModels), end(mModels), [&](Model* model)
	{
		model->Draw(fillType);		
	});
}


void Game_Renderer::close()
{
}


void Game_Renderer::reload()
{
}


void Game_Renderer::refresh()
{
}


void Game_Renderer::textureLoad( const char*  )  //Loads a single texture specified by fileLocale.
{
}


void Game_Renderer::textureDirLoad( const char*  )   //Loads any supported textures from directory.  Useful for anim textures and level loads.
{
}


void Game_Renderer::vertShaderLoad( const char*  )  //Loads a single vertex shader specified by fileLocale.
{
}


void Game_Renderer::vertShaderDirLoad( const char*  )   //Loads any supported vertex shaders from directory.
{
}


void Game_Renderer::fragShaderLoad( const char*  )  //Loads a single fragment specified by fileLocale.
{
}


void Game_Renderer::fragShaderDirLoad( const char*  )   //Loads any supported fragments from directory.
{
}


void Game_Renderer::programShaderLoad( const char*  )  //Loads and creates a program based on the 
{
}


void Game_Renderer::programShaderDirLoad( const char*  )   //Loads any supported programs from directory.
{
}


void Game_Renderer::modelLoad( const char*  )  //Loads a single model specified by fileLocale.
{
}


void Game_Renderer::modelDirLoad( const char*  )   //Loads any supported models from directory.
{
}


void Game_Renderer::DrawWorldAxes()
{
	//Set up the origin point, and draw lines from it
	// to each of the axis vectors.

	glEnable( GL_POINT_SMOOTH );
	glPointSize( 5 );
	glColor3f( 1.0f, 0.0f, 0.0f );
	glBegin( GL_POINTS );

		glVertex3f( 0.f, 0.f, 0.f );
		glVertex3f( 20.f, 0.f, 0.f );
		glVertex3f( 0.f, 20.f, 0.f );
		glVertex3f( 0.f, 0.f, 20.f );

	glEnd();
	glDisable( GL_POINT_SMOOTH );

	glEnable( GL_LINE_SMOOTH );
	glLineWidth( 1.5f );
	glBegin( GL_LINES );

		glColor3f( 0.2f, 0.1f, 0.9f );
		glVertex3f( 0.f, 0.f, 0.f );
		glVertex3f( 20.f, 0.f, 0.f); //"x-axis" extended so not unit length.

		glColor3f( 0.9f, 0.9f, 0.9f );
		glVertex3f( 0.f, 0.f, 0.f );
		glVertex3f( 0.f, 20.f, 0.f); //"y-axis" extended so not unit length.

		glColor3f( 0.1f, 0.9f, 0.0f );
		glVertex3f( 0.f, 0.f, 0.f );
		glVertex3f( 0.f, 0.f, 20.f); //"z-axis" extended so not unit length.

	glEnd();
	glDisable( GL_LINE_SMOOTH );


	//Now draw the grid.  this is a series of lines with right now a length of 5000 (in both directions.)
	const int gridWidth = 500;
	//x-z plane...  iterate on x, every 25 units (25x25) so z is same coordinate.
	for( int i = -gridWidth; i <= gridWidth; i += 25 )
	{
		//draw line from (i,0,0) to (i,0,5000)	
		glBegin( GL_LINES );

			glColor3f( 0.2f, 0.1f, 0.9f );
			glVertex3f( i, 0.f, -gridWidth );
			glVertex3f( i, 0.f, gridWidth); //"x-axis" extended so not unit length.

		glEnd();
	}
	for( int i = -gridWidth; i <= gridWidth; i += 25 )
	{
		//draw line from (i,0,0) to (i,0,5000)
		//draw line from (i,0,0) to (i,0,5000)	
		glBegin( GL_LINES );

			glColor3f( 0.2f, 0.1f, 0.9f );
			glVertex3f( -gridWidth, 0.f, i );
			glVertex3f( gridWidth, 0.f, i); //"x-axis" extended so not unit length.

		glEnd();
	}
	for( unsigned int i = 0; i <= 5000; i += 25 )
	{
		//draw line from (i,0,0) to (i,0,5000)
	}

}


//For drawing a simple colored box on screen for a frame.  Useful for things that don't live long.
void Game_Renderer::DrawSAQ_Colored( float width, float height, const CVector3& pos, CVector3 color, unsigned  )
{
	//Set up GL for rendering a single quad, using 'color' for each vertex.
	// each vertex is merely using the pos x and y coordinates and offsetting them by the width and height.

	glBegin( GL_QUADS );
		glColor3f( color.x, color.y, color.z );
		
		//Create 4 points.
		glVertex3f(pos.x + width, pos.y + height, 0.0f );
		glVertex3f(pos.x - width, pos.y + height, 0.0f );
		glVertex3f(pos.x - width, pos.y - height, 0.0f );
		glVertex3f(pos.x + width, pos.y - height, 0.0f );

	glEnd();

}


//This variation doesn't use the pos as world position, but as the pixel coordinates.  For HUD related objects.
void Game_Renderer::DrawSAQ_Top_Colored( float , float , const CVector3& , CVector3  )
{
}


//For drawing a simple colored box on screen for a frame.  Useful for things that don't live long.
void Game_Renderer::DrawSAQBorder_Colored( float width, float height, const CVector3& pos, CVector3 color, unsigned  )
{	
	glBegin( GL_LINE_LOOP );

		//Do one line, from origin + physics upperBound.x to origin + physics lowerBound.x
		glColor3f( 0.0f, 0.0f, 0.0f );
		
		glVertex3f(pos.x + width, pos.y + height, 0.0f );
		glVertex3f(pos.x - width, pos.y + height, 0.0f );
		glVertex3f(pos.x - width, pos.y - height, 0.0f );
		glVertex3f(pos.x + width, pos.y - height, 0.0f );

	glEnd();
}


//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
void DrawSAQBorder_Top_Colored( float , float , const CVector3& , CVector3  )
{
}


//Draws a textured object at a given location and of a certain size in world coordinates.
void Game_Renderer::DrawSAQ_Textured( float , float , const CVector3& , GLuint , unsigned  )
{
}


//This variation draws a textured object at a given location in pixel coordinates scaled.
void Game_Renderer::DrawSAQ_Top_Textured( float , float , const CVector3& , GLuint  )
{
}


//For drawing a simple colored box on screen for a frame.  Useful for things that don't live long.
void Game_Renderer::DrawSATri_Colored( float inscribedRadius, const CVector3& pos, CVector3 color, unsigned  )
{	
	//Creating an equilateral is like such:

	//P1 (top) : (c.x, c.y + radius, 0.0 )
	//P1 (left) : (c.x - halfSide, c.y - littleRadius, 0.0 )
	//P2 (right) : (c.x + halfSide, c.y - littleRadius, 0.0 )

	float halfSide = 3.0f * inscribedRadius / sqrt( 3.0f );
	float littleRadius = halfSide * sqrt( 3.0f ) / 6.0f;

	glBegin( GL_TRIANGLES );
		
		glColor3f( color.x, color.y, color.z );

		glVertex3f( pos.x, pos.y + inscribedRadius, 0.0f );
		glVertex3f( pos.x - halfSide, pos.y - littleRadius, 0.0f);
		glVertex3f( pos.x + halfSide, pos.y - littleRadius, 0.0f);

	glEnd();
}


//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
void Game_Renderer::DrawSATri_Top_Colored( float , const CVector3& , CVector3  )
{
}


//Draws a textured object at a given location and of a certain size.
void Game_Renderer::DrawSATri_Textured( float , const CVector3& , GLuint , unsigned  )
{
}


void Game_Renderer::DrawSATri_Top_Textured( float , const CVector3& , GLuint  )
{
}


//Draws a textured object at a given location and of a certain size.
void Game_Renderer::DrawSACircleBorder_Colored( float inscribedRadius, const CVector3& pos, const CVector3& color, unsigned grain, unsigned lineType, unsigned  )
{	
	float inc = 2.0f * (float)(M_PI / grain);

	CVector3* pts = new CVector3[grain];

	//Using the radius and sin/cos we can get a sampling of points along it.
	for( unsigned iter = 0; iter < grain; ++iter )
	{
		pts[iter] = CVector3( inscribedRadius * sin(iter*inc) + pos.x, inscribedRadius * cos(iter*inc) + pos.y, 0.0f );
	}

	glBegin( lineType );
		glColor3f( color.x, color.y, color.z );
		for( unsigned iter = 0; iter < grain; ++iter )
		{
			glVertex3f( pts[iter].x, pts[iter].y, pts[iter].z );
		}
	glEnd();

	delete [] pts;
}


//Draws a textured object at a given location and of a certain size.
void Game_Renderer::DrawSACircleFill_Colored( float inscribedRadius, const CVector3& pos, const CVector3& color, unsigned grain, unsigned , unsigned  )
{	
	float inc = 2.0f * (float)(M_PI / grain);

	CVector3* pts = new CVector3[grain];

	//Using the radius and sin/cos we can get a sampling of points along it.
	for( unsigned iter = 0; iter < grain; ++iter )
	{
		pts[iter] = CVector3( inscribedRadius * sin(iter*inc) + pos.x, inscribedRadius * cos(iter*inc) + pos.y, 0.0f );
	}

	glBegin( GL_POLYGON );
		glColor3f( color.x, color.y, color.z );
		for( unsigned iter = 0; iter < grain; ++iter )
		{
			glVertex3f( pts[iter].x, pts[iter].y, pts[iter].z );
		}
	glEnd();

	delete [] pts;
}


//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
void Game_Renderer::DrawSACircle_Top_Colored( float , const CVector3& , const CVector3& , unsigned  )
{
}


//Draws a textured object at a given location and of a certain size.
void Game_Renderer::DrawSACircle_Textured( float , const CVector3& , GLuint , unsigned  )
{
}


//This variation doesn't use the pos as raw position, but as the pixel position on window.  For HUD related objects.
void Game_Renderer::DrawSACircle_Top_Textured( float , const CVector3& , GLuint  )
{
}
