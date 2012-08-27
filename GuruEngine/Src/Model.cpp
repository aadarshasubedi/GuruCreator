/************************************************************************
Author:		Dan Strycharske
Date:       02.12.12
File:       Model.cpp
Brief:      
/************************************************************************/

#include "Model.h"
#include <vector>
#include "ModelComponent.h"
#include "Entity.h"


Model::Model()
{
	init();
}


Model::~Model()
{
	glDeleteBuffers(1, &mVboAllData);
	glDeleteBuffers(1, &mVboIndices);
}

	
/************************************************************************/
/* Public - Member methods
/************************************************************************/

void Model::DrawSelectionMode()
{
	glPushName(mParent->GetGUID());
	Draw(GL_FILL);
	glPopName();
}


#define BUFFER_OFFSET(i) ((char*)NULL + (i))
void Model::Draw(int fillmode)
{	
	if( mParent )
	{	
		CVector3 p = mParent->mRenderPosition;
		DQuaternion q = mParent->mOrientation;

		//@TODO: For testing rotation and other transform purposes
		//mParent->RotateRelativeTo(CVector3(0,20,0), CVector3(1,0,0), 2.5f);
		//q = mParent->mOrientation;

		float mat[16];

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		q.CreateMatrixGL(mat);
		glMultMatrixf( mat );
		glTranslatef(mParent->mPosition.x + p.x, mParent->mPosition.y + p.y, mParent->mPosition.z + p.z);
	}
	else
	{	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();	
		glTranslatef(0,0,0);
	}

	glPolygonMode(GL_FRONT_AND_BACK, fillmode);

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->mVboAllData );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, this->mVboIndices );

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(GL_FLOAT)*11, 0);
	glColorPointer(3, GL_FLOAT, sizeof(GL_FLOAT)*11, BUFFER_OFFSET(sizeof(GL_FLOAT)*3));
	glNormalPointer(GL_FLOAT, sizeof(GL_FLOAT)*11, BUFFER_OFFSET(sizeof(GL_FLOAT)*6));
	glTexCoordPointer(2, GL_FLOAT, sizeof(GL_FLOAT)*11, BUFFER_OFFSET(sizeof(GL_FLOAT)*9));
	////////////////////Now the actual draw call.//////////////////////////

	glDrawElements( mDrawType, mNumFaces*mNumVertsPerFace, GL_UNSIGNED_SHORT, 0 );

	///////////////////////////////////////////////////////////////////////
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBufferARB( GL_ARRAY_BUFFER, 0 );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );	

	glPopMatrix();
}



void Model::init()
{
	mVboAllData = 0;
	mVboIndices = 0;
	mDrawType = GL_FILL;
	mNumFaces = 0;
	mNumVertsPerFace = 0;

	//Set whenever the model has something in it to render.
	mLoaded = false;
}
