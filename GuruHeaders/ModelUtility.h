/************************************************************************
Author:		Dan Strycharske
Date:       02.12.12
File:       ModelUtility.h
Brief:      
/************************************************************************/

#ifndef MODEL_UTILITY_H
#define MODEL_UTILITY_H

#include "gl_inc.h"
#include "Utility.h"
#include "ModelDescriptor.h"


class Model;


ModelDescriptor CreateSphere( Model* model, float radius, GLushort slices, GLushort stacks, float color = 0.9f );
ModelDescriptor CreateBox( Model* model, float w, float h, float d, float color = 0.1f );  //specifies half-extents.
ModelDescriptor CreateCylinder( Model* model,float radius, float half_height, GLushort slices, float color = 0.5f );
ModelDescriptor CreateCapsule( Model* model, float radius, float half_height, GLushort slices, GLushort stacks, float color = 0.2f );

void getSphereVertsAndIndices( GLfloat* allVertData, unsigned int allDataSize, GLushort* indexList, float radius, GLushort slices, GLushort stacks, float color, CVector3 cpos=CVector3(0,0,0) );
void getCylinderVertsAndIndices( GLfloat* allVertData, unsigned int allDataSize, GLushort* indexList, float radius, float half_height, GLushort slices, float color, CVector3 cpos=CVector3(0,0,0));

void calculateSphereData( unsigned int& numVerts, unsigned int& numIndices, unsigned int& numTris, unsigned int slices, unsigned int stacks );
void calculateBoxData( unsigned int& numVerts, unsigned int& numIndices, unsigned int& numQuads);
void calculateCylinderData( unsigned int& numVerts, unsigned int& numIndices, unsigned int& numTris, unsigned int slices );
void calculateCapsuleData( unsigned int& numVerts, unsigned int& numIndices, unsigned int& numTris, unsigned int slices, unsigned int stacks );


#endif
