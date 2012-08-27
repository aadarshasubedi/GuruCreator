/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       ModelDescriptor.h
Brief:      
/************************************************************************/

#ifndef ModelDescriptor_H
#define ModelDescriptor_H


/************************************************************************/
/* Includes
/************************************************************************/

#include "ComponentDescriptor.h"
#include "gl_inc.h"


/************************************************************************/
/* Forward Declarations
/************************************************************************/


class ModelDescriptor : public ComponentDescriptor
{
private:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/

	/************************************************************************/
	/* Private - Member methods
	/************************************************************************/

protected:
	/************************************************************************/
	/* Protected - Member Variables
	/************************************************************************/

	/************************************************************************/
	/* Protected - Member methods
	/************************************************************************/

public:
	/************************************************************************
	* Constructors/Destructor
	************************************************************************/
	ModelDescriptor();
	virtual ~ModelDescriptor();

	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
	virtual ComponentDisciple* Create(Entity* obj);
	
	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/
	//VBO is interleaved and holds pos, color, normal, and tex data.
	GLuint mVboAllData;
	GLuint mVboIndices;

	GLuint mDrawType;
	unsigned int mNumFaces;
	unsigned int mNumVertsPerFace;

	ComponentDisciple* mParentComp;

	//Set whenever the model has something in it to render.
	bool mLoaded;
};


#endif
