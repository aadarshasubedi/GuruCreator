/************************************************************************
Author:		Dan Strycharske
Date:       02.12.12
File:       Model.h
Brief:      
/************************************************************************/


#ifndef Model_H
#define Model_H


/************************************************************************/
/* Includes
/************************************************************************/
#include "Utility.h"
#include "gl_inc.h"


/************************************************************************/
/* Forward Declarations
/************************************************************************/
class Entity;


class Model
{
private:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/
	//VBO is interleaved and holds pos, color, normal, and tex data.
	GLuint mVboAllData;
	GLuint mVboIndices;

	GLuint mDrawType;
	unsigned int mNumFaces;
	unsigned int mNumVertsPerFace;

	//Set whenever the model has something in it to render.
	bool mLoaded;

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
	void init();

public:
	/************************************************************************
	* Constructors/Destructor
	************************************************************************/
	Model();
	~Model();
	
	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
	void Draw(int fillmode);
	void DrawSelectionMode();

	/*************************************************************************
	 Accessors and Mutators
	*************************************************************************/
	GLuint& allData() { return mVboAllData; }
	GLuint getAllData() const { return mVboAllData; }
	void setAllData( const GLuint& alldata ) { mVboAllData = alldata; }

	GLuint& allIndices() { return mVboIndices; }
	GLuint getAllIndices() const { return mVboIndices; }
	void setAllIndices( const GLuint& indices ) { mVboIndices = indices; }

	GLuint& drawType() { return mDrawType; }
	GLuint drawType() const { return mDrawType; }
	void setDrawType(const GLuint& drawtype) { mDrawType = drawtype; }

	unsigned int& numFaces() { return mNumFaces; }
	unsigned int numFaces() const { return mNumFaces; }
	void setNumFaces(const unsigned int& numfaces) { mNumFaces = numfaces; }

	unsigned int& numVertsPerFace() { return mNumVertsPerFace; }
	unsigned int numVertsPerFace() const { return mNumVertsPerFace; }
	void setNumVertsPerFace(const unsigned int& numVertsFace) { mNumVertsPerFace = numVertsFace; }

	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/

	Entity* mParent;
};


#endif
