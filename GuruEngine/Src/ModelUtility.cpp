/************************************************************************
Author:		Dan Strycharske
Date:       02.12.12
File:       ModelUtility.cpp
Brief:      ModelUtility class implementation.
/************************************************************************/

#include "ModelDescriptor.h"
#include "ModelUtility.h"
#include "Model.h"
#include <assert.h>




void calculateSphereData( unsigned int& numVerts, unsigned int& numIndices, unsigned int& numTris, unsigned int slices, unsigned int stacks )
{	
	numVerts = 2 + (stacks-1)*slices;
	numTris = 2*slices*(stacks-2) + 2*slices;
	numIndices = 3 * (numTris);

}


void calculateBoxData( unsigned int& numVerts, unsigned int& numIndices, unsigned int& numQuads )
{
	numVerts = 8;
	numQuads = 6;
	numIndices = 4 * (numQuads);
}


void calculateCylinderData( unsigned int& numVerts, unsigned int& numIndices, unsigned int& numTris, unsigned int slices )
{
	numVerts = 2 + 2 * slices;
	numTris = 4 * slices;
	numIndices = 3 * numTris;
}


void calculateCapsuleData( unsigned int& numVerts, unsigned int& numIndices, unsigned int& numTris, unsigned int slices, unsigned int stacks )
{
	//2 spheres + cylinder.
	unsigned int sphereVerts, sphereInds, sphereTris, cylVerts, cylInds, cylTris;
	calculateSphereData( sphereVerts, sphereInds, sphereTris, slices, stacks);
	calculateCylinderData( cylVerts, cylInds, cylTris, slices );

	numVerts = 2 * sphereVerts + cylVerts;
	numTris = 2 * sphereTris + cylTris;
	numIndices = 2 * sphereInds + cylInds;
}


void generateSpherePoints( float radius, GLushort slices, GLushort stacks, std::vector<CVector3>* positions, std::vector<CVector3>* normals )
{
	//This will generate the physical world space points for the sphere, nothing more than position data and normals.

	//Setup values for generating the proper number of vertices
	unsigned int numVerts, ind, tris;
	calculateSphereData(numVerts, ind, tris, slices, stacks);

	//Angles for generating the spherical coords and thus the cartesian coords.
	float deltaAzi = (float)M_PI / (float)stacks;
	float azimuth = (float)(M_PI / 2.0f) - deltaAzi;

	float theta = 0.0f;  //0 at start.
	float deltaTheta = 2.0f * (float)M_PI / (float)slices;  // 360deg / slices = 

	//Generate the top point.
	positions->push_back(CVector3(0, radius, 0));
	normals->push_back(CVector3(0,1,0));

	unsigned int curNumVerts = 1;  //To keep track and do sanity check at end.

	//Generate the middle slices.
	for( GLushort stack = 0; stack < stacks - 1; ++stack )
	{
		for( GLushort slice = 0; slice < slices; ++slice )
		{
			float x = radius * cos(theta) * cos(azimuth);
			float y = radius * sin(azimuth);
			float z = radius * cos(azimuth) * sin(theta);
			positions->push_back(CVector3(x,y,z));
			normals->push_back( CVector3(x,y,z).normal() );
			theta += deltaTheta;
			++curNumVerts;
		}
		azimuth -= deltaAzi;
	}

	//Generate the bottom point
	positions->push_back(CVector3(0, -radius, 0));
	normals->push_back(CVector3(0,-1,0));
	++curNumVerts;

	assert(curNumVerts == numVerts);
}


void generateSphereIndices( GLushort slices, GLushort stacks, std::vector<GLushort>* indexList )
{
	//Setup values for generating the proper number of indices for the index list.
	unsigned int numVerts=0, numTris=0, numIndices=0;
	calculateSphereData(numVerts, numIndices, numTris, slices, stacks);

	unsigned int indCnt = 0;
	unsigned int triCnt = 0;
	GLushort first, second, third, fourth;

	//Start with top triangle fan.
	for( GLushort slice = 0; slice < slices; ++slice )
	{
		first = 0;
		second = slice + 1;
		third = ((slice + 1) % slices) + 1;
		indexList->push_back(first);
		indexList->push_back(second);
		indexList->push_back(third);
		indCnt += 3;
		++triCnt;
	}

	//Now do the middle bands.
	GLushort base = 1;

	for( GLushort stack = 0; stack < stacks-2; ++stack )
	{
		for( GLushort slice = 0; slice < slices; ++slice )
		{
			first = base + slice;
			second = base + slice + slices;
			fourth = (base + slices + slice + 1) % ((stack+1)*slices + slices + 1);
			if( fourth == 0 )
			{
				fourth = base + slice + 1;
			}
			third = fourth - slices;

			//Triangle one
			indexList->push_back(first);
			indexList->push_back(second);
			indexList->push_back(third);
			indCnt += 3;
			++triCnt;

			//Triangle two
			indexList->push_back(third);
			indexList->push_back(second);
			indexList->push_back(fourth);
			indCnt += 3;
			++triCnt;
		}
		base += slices;
	}

	base = (GLushort)numVerts-1;

	//And finally the bottom triangle fan, except last triangle, easier to do that by hand than have crazy modulus func to do it.
	for( GLushort slice = 0; slice < slices-1; ++slice )
	{
		first = base;
		second = base - slices + slice;
		third = base - slices + (slice + 1);
		indexList->push_back(first);
		indexList->push_back(second);
		indexList->push_back(third);
		indCnt += 3;
		++triCnt;
	}

	indexList->push_back(base);
	indexList->push_back(base - 1);
	indexList->push_back(base - slices);
	indCnt += 3;
	++triCnt;

	assert( triCnt == numTris );
	assert( indCnt == numIndices );
}


void getSphereVertsAndIndices( GLfloat* allVertData, unsigned int allDataSize, GLushort* indexList, float radius, GLushort slices, GLushort stacks, float color, CVector3 cpos )
{	
	unsigned allIndex = 0;
	std::vector<CVector3> positions;
	std::vector<CVector3> norms;
	std::vector<GLushort> indices;
	generateSpherePoints(radius, slices, stacks, &positions, &norms);
	generateSphereIndices(slices, stacks, &indices);

	////Start by creating the top point.
	CVector3 pos, normal;

	unsigned int posSize = positions.size();
	for( unsigned int vCnt = 0; vCnt < posSize; ++vCnt )
	{   
		pos = positions[vCnt] + cpos;
		normal = norms[vCnt];

		allVertData[allIndex + 0] = pos.x;
		allVertData[allIndex + 1] = pos.y;
		allVertData[allIndex + 2] = pos.z;

		allVertData[allIndex + 3] = color+0.4f;
		allVertData[allIndex + 4] = color-0.1f;
		allVertData[allIndex + 5] = color+(vCnt%6)*0.1f;

		allVertData[allIndex + 6] = 1.0f;
		allVertData[allIndex + 7] = 1.0f;

		allVertData[allIndex + 8] = normal.x;
		allVertData[allIndex + 9] = normal.y;
		allVertData[allIndex + 10] = normal.z;

		allIndex += allDataSize;
	}

	//Now go through and copy over all the indices.
	unsigned int indSize = indices.size();
	for( unsigned int index = 0; index < indSize; ++index )
	{
		indexList[index] = indices[index];
	}
}


ModelDescriptor CreateSphere( Model* model, float radius, GLushort slices, GLushort stacks, float color )
{ 
	unsigned int numVerts = 2 + slices * (stacks-1);
	unsigned int numTris = 2*slices*(stacks-2) + 2*slices;
	unsigned int numIndices = 3 * numTris;

	int vertElemSize = 3;  // for x, y, z, w
	int colElemSize = 3;  // for r, g, b
	int normElemSize = 3;
	int texElemSize = 2;  // for u, v

	unsigned int allDataSize = vertElemSize+colElemSize+normElemSize+texElemSize;

	GLfloat* allVertData = NULL;
	try
	{
		allVertData = new GLfloat[numVerts * allDataSize];	
	}
	catch(std::exception)
	{
		delete [] allVertData;
	}

	//There will be 6 quads, each with 4 indices into vertices array.
	GLushort*  indexList	= new GLushort[numIndices];

	getSphereVertsAndIndices(allVertData, allDataSize, indexList, radius, slices, stacks, color);

	GLuint* alldata = 0;
	GLuint* allindices = 0;

	//Create a descriptor instead and return that.
	ModelDescriptor desc;

	if( model == NULL )
	{
		desc.mNumFaces = numTris;
		desc.mNumVertsPerFace = 3;
		desc.mDrawType = GL_TRIANGLES;
		alldata = &desc.mVboAllData;
		allindices = &desc.mVboIndices;
	}
	else
	{
		model->numFaces() = numTris;
		model->numVertsPerFace() = 3;
		model->drawType() = GL_TRIANGLES;
		alldata = &model->allData();
		allindices = &model->allIndices();
	}

	//Create and bind the buffers.
	glGenBuffersARB( 1, alldata );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, *alldata );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof( GLfloat ) * numVerts * allDataSize, allVertData, GL_STREAM_DRAW_ARB );
	//Create the vbo here for Index Data.

	glGenBuffersARB( 1, allindices );
	//Bind to work with it.
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *allindices );
	//Buffer our indices into it.
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLushort) * numTris * 3, indexList, GL_STREAM_DRAW_ARB );


	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );

	//At this point, all should be bound and well, so delete the temp arrays.
	delete [] indexList;
	delete [] allVertData;

	return desc;
}


void generateCylinderPoints(float radius, float half_height, unsigned int slices, std::vector<CVector3>* positions, std::vector<CVector3>* normals)
{
	//Setup up theta.
	float theta = 0.0f;
	float deltaTheta = 2.0f * (float)M_PI / (float)slices;

	//assertion values
	//unsigned int numVerts = 2 + 2*slices;
	//unsigned int curNumVerts = 0;

	//Top fan center
	positions->push_back(CVector3(0,half_height,0));
	normals->push_back(CVector3(0,1,0));
	//++curNumVerts;

	//create top fan
	for( unsigned int slice = 0; slice < slices; ++slice)
	{
		float x = radius * cos(theta);
		float y = half_height;
		float z = radius * sin(theta);
		theta += deltaTheta;

		positions->push_back(CVector3(x,y,z));
		normals->push_back(CVector3(x,y,z).normal());
		//++curNumVerts;
	}

	theta = 0.0f;
	//Create bottom fan
	for( unsigned int slice = 0; slice < slices; ++slice)
	{
		float x = radius * cos(theta);
		float y = -half_height;
		float z = radius * sin(theta);
		theta += deltaTheta;

		positions->push_back(CVector3(x,y,z));
		normals->push_back(CVector3(x,y,z).normal());
		//++curNumVerts;
	}

	//Bottom middle point, bottom fan center.
	positions->push_back(CVector3(0,-half_height,0));
	normals->push_back(CVector3(0,1,0));
	//++curNumVerts;

	//assert(curNumVerts == numVerts);
}


void generateCylinderIndices(GLushort slices, std::vector<GLushort>* indices)
{
	//Assertion values
	//unsigned int numTris = 4*slices;
	//unsigned int numIndices = 3*numTris;
	unsigned int numVerts = 2 + 2*slices;
	//unsigned int curTriCnt = 0;
	//unsigned int curIndCnt = 0;

	GLushort first, second, third, fourth;

	//Hook up top fan.
	for( GLushort slice = 0; slice < slices; ++slice )
	{
		first = 0;
		second = slice + 1;
		third = (slice+1) % slices + 1;
		indices->push_back(first);
		indices->push_back(second);
		indices->push_back(third);

		//curIndCnt += 3;
		//++curTriCnt;
	}

	GLushort base = 1;

	//Hook up middle.
	for( GLushort slice = 0; slice < slices; ++slice )
	{		
		first = base + slice;
		second = base + slice + slices;
		fourth = (base + slices + slice + 1) % (slices + slices + 1);
		if( fourth == 0 )
		{
			fourth = base + slice + 1;
		}
		third = fourth - slices;

		//Triangle one
		indices->push_back(first);
		indices->push_back(second);
		indices->push_back(third);
		//curIndCnt += 3;
		//++curTriCnt;

		//Triangle two
		indices->push_back(third);
		indices->push_back(second);
		indices->push_back(fourth);
		//curIndCnt += 3;
		//++curTriCnt;
	}

	base = (GLushort)numVerts - 1;

	//Hook up bottom fan
	for( GLushort slice = 0; slice < slices-1; ++slice )
	{
		first = base;
		second = base - slices + slice;
		third = (base - slices + slice + 1) % (numVerts-1);
		indices->push_back(first);
		indices->push_back(second);
		indices->push_back(third);

		//curIndCnt += 3;
		//++curTriCnt;
	}

	first = base;
	second = base - 1;
	third = base - slices;
	indices->push_back(first);
	indices->push_back(second);
	indices->push_back(third);

	//curIndCnt += 3;
	//++curTriCnt;

	//assert(curTriCnt == numTris);
	//assert(curIndCnt == numIndices);
}


void getCylinderVertsAndIndices( GLfloat* allVertData, unsigned int allDataSize, GLushort* indexList, float radius, float half_height, GLushort slices, float color, CVector3 cpos )
{	
	unsigned allIndex = 0;

	std::vector<CVector3> positions;
	std::vector<CVector3> norms;
	std::vector<GLushort> indices;
	generateCylinderPoints(radius, half_height, slices, &positions, &norms);
	generateCylinderIndices(slices, &indices);

	//Start by creating the top point.
	CVector3 pos, normal;

	//Same as spheres, just run through the points, generating all the rest as needed.
	unsigned int posSize = positions.size();
	for( unsigned int vCnt = 0; vCnt < posSize; ++vCnt )
	{   
		pos = positions[vCnt] + cpos;
		normal = norms[vCnt];

		allVertData[allIndex + 0] = pos.x;
		allVertData[allIndex + 1] = pos.y;
		allVertData[allIndex + 2] = pos.z;

		allVertData[allIndex + 3] = color+(0.01f*vCnt);
		allVertData[allIndex + 4] = color+0.3f;
		allVertData[allIndex + 5] = color+(0.5f*vCnt);

		allVertData[allIndex + 6] = 1.0f;
		allVertData[allIndex + 7] = 1.0f;

		allVertData[allIndex + 8] = normal.x;
		allVertData[allIndex + 9] = normal.y;
		allVertData[allIndex + 10] = normal.z;

		allIndex += allDataSize;
	}

	//Now go through and copy over all the indices.
	unsigned int indSize = indices.size();
	for( unsigned int index = 0; index < indSize; ++index )
	{
		indexList[index] = indices[index];
	}
}


ModelDescriptor CreateCylinder( Model* model, float radius, float half_height, GLushort slices, float color )
{
	//Generate the vertex data needed for each of the 8 points.
	// 8 vertices, each with an x,y,z component (3 vals).
	unsigned int numVerts = 2 + 2*slices;
	unsigned int numTris = 4*slices;
	unsigned int numIndices = 3*numTris;

	int vertElemSize = 3;  // for x, y, z, w
	int colElemSize = 3;  // for r, g, b
	int normElemSize = 3;
	int texElemSize = 2;  // for u, v

	int allDataSize = vertElemSize+colElemSize+texElemSize+normElemSize;

	GLfloat* allVertData = NULL;
	try
	{
		allVertData = new GLfloat[numVerts * allDataSize];	
	}
	catch(std::exception e)
	{
		delete [] allVertData;
	}

	//There will be 6 quads, each with 4 indices into vertices array.
	//GLfloat* allVertData = NULL;
	GLushort* indexList = NULL;
	try
	{
		indexList = new GLushort[numIndices];
	}
	catch(std::exception e)
	{
		delete [] indexList;
	}


	getCylinderVertsAndIndices(allVertData, allDataSize, indexList, radius, half_height, slices, color);

	GLuint* alldata = 0;
	GLuint* allindices = 0;

	//Create a descriptor instead and return that.
	ModelDescriptor desc;

	if( model == NULL )
	{
		desc.mNumFaces = numTris;
		desc.mNumVertsPerFace = 3;
		desc.mDrawType = GL_TRIANGLES;
		alldata = &desc.mVboAllData;
		allindices = &desc.mVboIndices;
	}
	else
	{
		model->numFaces() = numTris;
		model->numVertsPerFace() = 3;
		model->drawType() = GL_TRIANGLES;
		alldata = &model->allData();
		allindices = &model->allIndices();
	}

	//model->numFaces() = numTris;
	//model->numVertsPerFace() = 3;
	//model->drawType() = GL_TRIANGLES;

	//Create and bind the buffers.
	glGenBuffersARB( 1, alldata );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, *alldata);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof( GLfloat ) * numVerts * allDataSize, allVertData, GL_STREAM_DRAW_ARB );	

	//Create the vbo here for Index Data.
	glGenBuffersARB( 1, allindices );
	//Bind to work with it.
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *allindices );
	//Buffer our indices into it.
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLushort) * numTris * 3, indexList, GL_STREAM_DRAW_ARB );

	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );

	//At this point, all should be bound and well, so delete the temp arrays.
	delete [] indexList;
	delete [] allVertData;

	return desc;
}


ModelDescriptor CreateCapsule( Model* model, float radius, float half_height, GLushort slices, GLushort stacks, float color )
{
	//Generate the vertex data needed for each of the 8 points.
	// 8 vertices, each with an x,y,z component (3 vals).
	unsigned capVerts=0, capTris=0, capIndices=0;
	calculateCapsuleData( capVerts, capIndices, capTris, slices, stacks);

	int vertElemSize = 3;  // for x, y, z, w
	int colElemSize = 3;  // for r, g, b
	int normElemSize = 3;
	int texElemSize = 2;  // for u, v

	int allDataSize = vertElemSize+colElemSize+texElemSize+normElemSize;

	GLfloat* allVertData = new GLfloat[capVerts * allDataSize];	
	GLushort*  indexList	= new GLushort[capIndices];

	//Essentially, generate a sphere, then add it vert data.  then generate the cylinder.  then the last sphere.
	//We're gonna generate 3 separate arrays for each shape, then memcpy into the capsules arrays, cleaning up the temps.
	unsigned cylVerts=0, cylTris=0, cylIndices=0;
	calculateCylinderData( cylVerts, cylIndices, cylTris, slices );

	unsigned sphVerts=0, sphTris=0, sphIndices=0;
	calculateSphereData( sphVerts, sphIndices, sphTris, slices, stacks);

	unsigned int dataInc = 0;
	unsigned int indexInc = 0;

	getSphereVertsAndIndices(allVertData, allDataSize, indexList, radius, slices, stacks, color, CVector3(0,half_height,0));
	dataInc += sphVerts*allDataSize;
	indexInc += sphIndices;

	getCylinderVertsAndIndices(allVertData+dataInc, allDataSize, indexList+indexInc, radius, half_height, slices, color);
	dataInc += cylVerts*allDataSize;
	indexInc += cylIndices;

	getSphereVertsAndIndices(allVertData+dataInc, allDataSize, indexList+indexInc, radius, slices, stacks, color, CVector3(0,half_height,0) * -1.0f);

	GLuint* alldata = 0;
	GLuint* allindices = 0;

	//Create a descriptor instead and return that.
	ModelDescriptor desc;

	if( model == NULL )
	{
		desc.mNumFaces = capTris;
		desc.mNumVertsPerFace = 3;
		desc.mDrawType = GL_TRIANGLES;
		alldata = &desc.mVboAllData;
		allindices = &desc.mVboIndices;
	}
	else
	{
		model->numFaces() = capTris;
		model->numVertsPerFace() = 3;
		model->drawType() = GL_TRIANGLES;
		alldata = &model->allData();
		allindices = &model->allIndices();
	}
	
	//Create and bind the buffers.
	glGenBuffersARB( 1, alldata );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, *alldata);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof( GLfloat ) * capVerts * allDataSize, allVertData, GL_STREAM_DRAW_ARB );	

	//Create the vbo here for Index Data.
	glGenBuffersARB( 1, allindices );
	//Bind to work with it.
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *allindices );
	//Buffer our indices into it.
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLushort) * model->numFaces() * model->numVertsPerFace(), indexList, GL_STREAM_DRAW_ARB );

	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );

	//At this point, all should be bound and well, so delete the temp arrays.
	delete [] indexList;
	delete [] allVertData;

	return desc;
}


ModelDescriptor CreateBox( Model* model, float w, float h, float d, float color )
{
	//Generate the vertex data needed for each of the 8 points.
	// 8 vertices, each with an x,y,z component (3 vals).
	int numVerts = 8;

	int vertElemSize = 3;  // for x, y, z, w
	int colElemSize = 3;  // for r, g, b
	int normElemSize = 3;
	int texElemSize = 2;  // for u, v

	int allDataSize = vertElemSize+colElemSize+texElemSize+normElemSize;

	unsigned allIndex = 0;
	GLfloat* allVertData = new GLfloat[numVerts * allDataSize];

	//There will be 6 quads, each with 4 indices into vertices array.
	GLushort*  indices	= new GLushort[6 * 4];
	GLushort currentIndex = 0;

	CVector3 pos, normal;

	pos = CVector3( -w, -h, -d );
	allVertData[allIndex] =     pos.x;
	allVertData[allIndex+1] =   pos.y;
	allVertData[allIndex+2] =   pos.z;

	allVertData[allIndex+3] = color;
	allVertData[allIndex+4] = color+0.2f;
	allVertData[allIndex+5] = color;

	allVertData[allIndex+6] = 0.0f;
	allVertData[allIndex+7] = 1.0f;

	normal = CVector3( -1, -1, -1 );
	allVertData[allIndex+8] = normal.x;
	allVertData[allIndex+9] = normal.y;
	allVertData[allIndex+10] = normal.z;

	allIndex += allDataSize;

	pos = CVector3( -w, -h, d );
	allVertData[allIndex] =     pos.x;
	allVertData[allIndex+1] =   pos.y;
	allVertData[allIndex+2] =   pos.z;

	allVertData[allIndex+3] = color+0.2f;
	allVertData[allIndex+4] = color;
	allVertData[allIndex+5] = color;

	allVertData[allIndex+6] = 1.0f;
	allVertData[allIndex+7] = 0.0f;

	normal = CVector3( -1, -1, 1 );
	allVertData[allIndex+8] = normal.x;
	allVertData[allIndex+9] = normal.y;
	allVertData[allIndex+10] = normal.z;

	allIndex += allDataSize;

	pos = CVector3( -w, h, d );
	allVertData[allIndex] =     pos.x;
	allVertData[allIndex+1] =   pos.y;
	allVertData[allIndex+2] =   pos.z;

	allVertData[allIndex+3] = color;
	allVertData[allIndex+4] = color;
	allVertData[allIndex+5] = color+0.2f;

	allVertData[allIndex+6] = 0.0f;
	allVertData[allIndex+7] = 0.0f;

	normal = CVector3( -1, 1, 1 );
	allVertData[allIndex+8] = normal.x;
	allVertData[allIndex+9] = normal.y;
	allVertData[allIndex+10] = normal.z;

	allIndex += allDataSize;

	pos = CVector3( -w, h, -d );
	allVertData[allIndex] =     pos.x;
	allVertData[allIndex+1] =   pos.y;
	allVertData[allIndex+2] =   pos.z;

	allVertData[allIndex+3] = color+0.2f;
	allVertData[allIndex+4] = color;
	allVertData[allIndex+5] = color+0.2f;

	allVertData[allIndex+6] = 1.0f;
	allVertData[allIndex+7] = 1.0f;

	normal = CVector3( -1, 1, -1 );
	allVertData[allIndex+8] = normal.x;
	allVertData[allIndex+9] = normal.y;
	allVertData[allIndex+10] = normal.z;

	allIndex += allDataSize;

	pos = CVector3( w, -h, -d );
	allVertData[allIndex] =     pos.x;
	allVertData[allIndex+1] =   pos.y;
	allVertData[allIndex+2] =   pos.z;

	allVertData[allIndex+3] = color;
	allVertData[allIndex+4] = color+0.2f;
	allVertData[allIndex+5] = color+0.2f;

	allVertData[allIndex+6] = 1.0f;
	allVertData[allIndex+7] = 1.0f;


	normal = CVector3( 1, -1, -1 );
	allVertData[allIndex+8] = normal.x;
	allVertData[allIndex+9] = normal.y;
	allVertData[allIndex+10] = normal.z;

	allIndex += allDataSize;

	pos = CVector3( w, -h, d );
	allVertData[allIndex] =     pos.x;
	allVertData[allIndex+1] =   pos.y;
	allVertData[allIndex+2] =   pos.z;

	allVertData[allIndex+3] = color+0.2f;
	allVertData[allIndex+4] = color+0.2f;
	allVertData[allIndex+5] = color;

	allVertData[allIndex+6] = 1.0f;
	allVertData[allIndex+7] = 0.0f;

	normal = CVector3( 1, 1, -1 );
	allVertData[allIndex+8] = normal.x;
	allVertData[allIndex+9] = normal.y;
	allVertData[allIndex+10] = normal.z;

	allIndex += allDataSize;

	pos = CVector3( w, h, d );
	allVertData[allIndex] =     pos.x;
	allVertData[allIndex+1] =   pos.y;
	allVertData[allIndex+2] =   pos.z;

	allVertData[allIndex+3] = 0.1f;
	allVertData[allIndex+4] = 0.1f;
	allVertData[allIndex+5] = 0.5f;

	allVertData[allIndex+6] = 0.0f;
	allVertData[allIndex+7] = 1.0f;

	normal = CVector3( 1, 1, 1 );
	allVertData[allIndex+8] = normal.x;
	allVertData[allIndex+9] = normal.y;
	allVertData[allIndex+10] = normal.z;

	allIndex += allDataSize;

	pos = CVector3( w, h, -d );
	allVertData[allIndex] =     pos.x;
	allVertData[allIndex+1] =   pos.y;
	allVertData[allIndex+2] =   pos.z;

	allVertData[allIndex+3] = color;
	allVertData[allIndex+4] = color;
	allVertData[allIndex+5] = color;

	allVertData[allIndex+6] = 0.0f;
	allVertData[allIndex+7] = 0.0f;

	normal = CVector3( 1, 1, -1 );
	allVertData[allIndex+8] = normal.x;
	allVertData[allIndex+9] = normal.y;
	allVertData[allIndex+10] = normal.z;

	//Set up the index array so it will draw properly the quads.
	{
		//Left face.
		indices[currentIndex] 	= 0;
		indices[currentIndex+1] = 1;
		indices[currentIndex+2] = 2;
		indices[currentIndex+3] = 3;

		currentIndex += 4;

		//Front face.
		indices[currentIndex] 	= 7;//3;
		indices[currentIndex+1] = 6;//2;
		indices[currentIndex+2] = 2;
		indices[currentIndex+3] = 3;

		currentIndex += 4;

		//Rigth face.
		indices[currentIndex] = 7;
		indices[currentIndex+1] = 6;
		indices[currentIndex+2] = 5;
		indices[currentIndex+3] = 4;

		currentIndex += 4;

		//back face.
		indices[currentIndex] = 0;
		indices[currentIndex+1] = 1;
		indices[currentIndex+2] = 5;
		indices[currentIndex+3] = 4;

		currentIndex += 4;

		//Top face.
		indices[currentIndex] = 1;
		indices[currentIndex+1] = 2;
		indices[currentIndex+2] = 6;
		indices[currentIndex+3] = 5;

		currentIndex += 4;

		//Bottom face.
		indices[currentIndex] = 3;
		indices[currentIndex+1] = 0;
		indices[currentIndex+2] = 4;
		indices[currentIndex+3] = 7;
	}

	GLuint temp;
	glGenBuffers( 1, &temp );


	GLuint* alldata = 0;
	GLuint* allindices = 0;

	//Create a descriptor instead and return that.
	ModelDescriptor desc;

	if( model == NULL )
	{
		desc.mNumFaces = 6;
		desc.mNumVertsPerFace = 4;
		desc.mDrawType = GL_QUADS;
		alldata = &desc.mVboAllData;
		allindices = &desc.mVboIndices;
	}
	else
	{
		model->numFaces() = 6;
		model->numVertsPerFace() = 4;
		model->drawType() = GL_QUADS;
		alldata = &model->allData();
		allindices = &model->allIndices();
	}

	//model->numFaces() = 6;
	//model->numVertsPerFace() = 4;
	//model->drawType() = GL_QUADS;

	//Create and bind the buffers.
	glGenBuffersARB( 1, alldata );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, *alldata);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof( GLfloat ) * numVerts * allDataSize, allVertData, GL_STREAM_DRAW_ARB );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Create the vbo here for Index Data.
	glGenBuffersARB( 1, allindices );
	//Bind to work with it.
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *allindices );

	//Buffer our indices into it.
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLushort) * 6 * 4, indices, GL_STREAM_DRAW_ARB );
	
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );


	//At this point, all should be bound and well, so delete the temp arrays.
	delete [] indices;
	delete [] allVertData;
	
	return desc;
}
