

/*
Author:	Dan Strycharske
Project: Inner Knight
Team:	Macrohard
DigiPen Institute of Technology owns this and our souls.

File:	Structs.h
brief:	This file contains the extra_meshcontainer and extra_frame structs
		necessary for the XFileObject.  Frame is extended to have a combined
		transformation matrix that can be applied once at the end of each frame.

		The MeshContainer is extended by adding pointers to materials, pointer-to-
		pointer to textures, a skinmesh pointer, a matrix for the bone offsets, and 
		a group of matrices for the current frame.

*/

#if !defined( STRUCTS_H_ )
#define STRUCTS_H_

// Includes necessary for Direct3D and Animation
#include "CommDefs.hpp"
#include "VQS.h"

#include <iostream>

#ifdef DIRECTX 
#include <d3dx9.h>
#include <d3dx9anim.h>
#include <d3dx9.h>



typedef struct EXTENDED_D3DXMESHCONTAINER : public D3DXMESHCONTAINER
{
	//pointer to pointer to direct 3d textures.
	IDirect3DTexture9**		m_Textures;

	//Pointer to material list.
	D3DMATERIAL9*			m_Materials;


	//Pointer to skinMesh, if there is one.
	ID3DXMesh*				m_SkinMesh;

	//Matrices
	D3DXMATRIX*				m_BoneOffsets;
	
}Extra_Mesh;


typedef struct EXTENDED_D3DXFRAME:	public D3DXFRAME
{
	//Matrix of total combined transforms on the frame, world transform essentially.
	D3DXMATRIX	m_combined_transformMat;

	//
	vqs			m_vqsTransformToRoot;

	//This will hold the updated interpolated keyframe at any given point in time,
	//	so that the frame can be grabbed, it's interpolated transform can be grabbed.
	vqs			m_vqsTransform;

}Extra_Frame;

#else //End of DirectX implementation.

//Begin general implementation
typedef struct GuruFrame
{
	//local orientation
	//to root orientation
	//Combined matrix for graphics cards.
	Matrix4		m_combined_transformMat;
	vqs			m_vqsTransformToRoot;

	//This will hold the updated interpolated keyframe at any given point in time,
	//	so that the frame can be grabbed, it's interpolated transform can be grabbed.
	vqs			m_vqsTransform;
}Extra_Frame;

typedef struct MeshContainer
{

}Extra_Mesh;

#endif

#endif //End of header.

