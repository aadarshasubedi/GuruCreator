/*
author:  Dan Strycharske
date:	 05/22/08
file:	 NinjaObject.h
brief:	 This file holds the interface declaration for the ninja object.  This AI controlled
			bot will try to hide from enemies, and back stab them if he has a chance.


*/

#pragma once

#include <vector>

//Interface for drawing lines with a list of D3DXVECTOR3 as points.  These
//	will be translated into my vertex declaration and used in my shader to
//	draw the lines.


//This class will hold the directx device pointer.  
// It will have methods to draw using several different algorithms.
// It will contain a list of points defined using my vertex declaration.
// It will contain methods to add/delete, insert, and modify point positions.


enum PathType { LINE = 0, DEBOOR, CATMULL};

#ifdef DIRECTX

#include <d3dx9mesh.h>

class DX_DrawablePointList
{
public:
	LPD3DXMESH	gSphereMesh;
	typedef std::vector< LOTSVertex >	PointList;
	typedef std::vector< D3DXVECTOR3 >	D3DXPointList;
	typedef IDirect3DVertexBuffer9*		VertexBuffer;	
	typedef IDirect3DIndexBuffer9*		IndexBuffer;
	typedef LPDIRECT3DDEVICE9			DevicePTR;

public:
	//Default contstructor.
	DrawablePointList(DevicePTR device);

	//This constructor will take in a list of points, given as D3DXVECTOR3s.
	DrawablePointList(const PointList& pt_list, DevicePTR device );

	//This constructor will take in a list of points, given as LOTSVertex's.
	DrawablePointList(const D3DXPointList& pt_list, DevicePTR device );

	//The destructor will take care of releasing the buffers and the device
	// reference so no errors occur upon closing.
	~DrawablePointList();

	//This will initialize the shaders and presentation parameters.
	void Init( void );

	//This creates a pointlist given a list of d3dxvector3's.
	void CreatePointList( const D3DXPointList& pt_list );

	//The class is going to have a reference to the device, since it will need to 
	// have it in order to draw the path.
	inline LPDIRECT3DDEVICE9 GetDevicePTR() { return m_device; }
	
	//These functions will check to see if any points have been specified.  If no points
	//	are there the buffers will not be accessed.
	//
	void CreateAndBuildBuffers( D3DXPointList pDXPtList );

	//These methods will create the vertex buffers.  These
	// will be called 
	void UpdateBuffers( );
	void createVertexBuffer( unsigned i_size );
	void createIndexBuffer( unsigned i_size );
	void updateVertexBuffer();
	void updateIndexBuffer();


	//@todo:
	//This method will add a point to the list, and update the buffers to account for it.
	void InsertPointInList( const D3DXVECTOR3& point, unsigned insertIndex );
	
	//These two will simply call the InsertPointInList function, with 0 or list.size() as the index.
	void PushPointToBack( const D3DXVECTOR3& point );
	void PushPointToFront( const D3DXVECTOR3& point );

	//Now for the rendering functions, which will take a certain enumeration to dictate how the curve is 
	// rendered.
	void Render( );
	//These methods are for the smaller rendering, 
	//void RenderLine( unsigned firstIndex, unsigned secondIndex, bool drawArrows = false );

	//@todo:
	//This function will take a time parameter from 0 to 1, and will spit out the point along the curve
	// or line specified by the enumeration type.  One will return a d3dxvector3, the other a lotsvertex.
	D3DXVECTOR3	D3DXPointOnPath( PathType type, float time );
	LOTSVertex	VERTPointOnPath( PathType type, float time );


	//Getters for lots of the data I'll need above this class.
	unsigned		NumPoints();
	LOTSVertex		GetPoint( unsigned index );	
	D3DXVECTOR3		GetDXPoint( unsigned index );

	void			PathInit();
	bool			IsPathInit();


private:
	//Here's the list of vertices
	PointList				m_PList;

	//Pointer to the device.
	LPDIRECT3DDEVICE9		m_device;
	LPD3DXEFFECT			m_effect;

	//These buffers are for the directx drawing purposes.  I have methods to fill
	// these in appropriately for the type of curve, looping or start-finish.
	VertexBuffer			m_vBuf;
	IndexBuffer				m_iBuf;


	unsigned				mVertexBufSize;
	unsigned				mIndexBufSize;

	//This boolean will let me know if there is even a path defined at all.  If there
	// is no path, we won't even bother trying to render or process the path at all.
	bool					m_pathInit;

	//This will tell me if I've already created the buffers or not.
	bool					m_bufsCreated;
};
#endif
