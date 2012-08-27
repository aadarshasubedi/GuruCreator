/*
author:  Dan Strycharske
date:	 05/22/08
file:	 PathingClasses.cpp
brief:	 This file holds the interface declarations for all of my classes involved in generating,
			


*/

#include "PathingClasses.h"
#include "CommDefs.hpp"


DrawablePointList::DrawablePointList(DevicePTR device) 
: m_PList(0), m_device(device), m_effect(NULL), m_vBuf(NULL), m_iBuf(NULL), m_pathInit(false), m_bufsCreated(false)
{
	Init();
}

DrawablePointList::DrawablePointList(const PointList& pt_list, DevicePTR device )
: m_PList(pt_list), m_device(device), m_effect(NULL), m_vBuf(NULL), m_iBuf(NULL), m_pathInit(true), m_bufsCreated(false)
{
	Init();
}

DrawablePointList::DrawablePointList(const D3DXPointList& pt_list, DevicePTR device )
: m_PList(0), m_device(device), m_effect(NULL), m_vBuf(NULL), m_iBuf(NULL), m_pathInit(true), m_bufsCreated(false)
{
	CreatePointList( pt_list );
	Init();
}

DrawablePointList::~DrawablePointList()
{
	//Release all of directx's structs and pointers.
	SAFE_RELEASE( m_device );
	SAFE_RELEASE( m_vBuf );
	SAFE_RELEASE( m_iBuf );
}

void DrawablePointList::Init( void )
{
	//If the point list is not empty, add these to the vertex buffer
	//This will declare my vertex class for use in the pipeline.

	//First check to make sure we have a valid device.
	if( m_device )
	{
		InitVertexDeclarations( m_device );

		//Create the bounding sphere mesh.
		D3DXCreateSphere( m_device, 2.0f, 5, 5, &gSphereMesh, NULL );

		//Here I will create a shaderflag and create the effect from my shader.fx
		DWORD shaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG;
		HRESULT hr = D3DXCreateEffectFromFile(m_device, L"DanShader.fx", NULL, NULL, shaderFlags, NULL, &m_effect, NULL );
		
		if( FAILED(hr) )
		{
			return;
		}

		//Here I will actually create the vertex buffers.  If there are no points in the
		// list, the buffers will not be created, and upon updating if they are empty
		// the creation will be handled then.
		unsigned size = (unsigned)m_PList.size();
		if( size > 0 )
		{
			//Here I'll create the buffers to the current size of the list.
			//CreateAndBuildBuffers( size );
			//m_bufsCreated = true;
		}
		else
		{
			//This means no points have been specified yet, so do nothing.
		}
	}
}

void DrawablePointList::CreatePointList(const DrawablePointList::D3DXPointList &pt_list)
{
	//Convert all d3dxvector3's here into lotsvertex's..  this is easy since there
	// is only the position to truly worry about.
	//
	//Grab the incoming list's size to be used for resizing and loop.
	unsigned size = (unsigned)pt_list.size();

	//Resize and clear the point list, then begin copying the data.
	m_PList.clear();
	m_PList.resize( size );

	for( unsigned loop_iter = 0; loop_iter < size; ++loop_iter )
	{
		//Copy the position data over...
		m_PList[loop_iter] = LOTSVertex( pt_list[loop_iter] );

		//...and anything else that might be useful.
	}
}

void DrawablePointList::CreateAndBuildBuffers( D3DXPointList pDXPtList )
{
	//Since the list was checked for none zero number of elements, I know here that
	// the point list is valid and so can work with the data in it.
	//

	CreatePointList( pDXPtList );

	//Here, allocate the vertex buffer itself, setting the number of elements to the
	// the number of points in the list.
	createVertexBuffer( (unsigned)pDXPtList.size() );

	//The index buffer will hold the same number of elements as the vertex buffer.
	//createIndexBuffer( (unsigned)pDXPtList.size() / 2 );

	//These functions will actually lock/unlock the buffers and set the data inside of
	// them from the point list.
	updateVertexBuffer();
	//The index buffer is trivial, simply being 0 to size in that order.
	//updateIndexBuffer();

	PathInit();
}

void DrawablePointList::UpdateBuffers()
{
}

void DrawablePointList::createVertexBuffer( unsigned i_size )
{

	HRESULT hr = m_device->CreateVertexBuffer( i_size * sizeof(LOTSVertex), D3DUSAGE_DYNAMIC, D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &m_vBuf, 0 );

	mVertexBufSize = i_size;

	if( SUCCEEDED(hr) )
	{
		//This is good, so we can actually update the vertex buffer now.
		updateVertexBuffer();
	}
}

void DrawablePointList::createIndexBuffer( unsigned i_size )
{
	HRESULT hr = m_device->CreateIndexBuffer( i_size * sizeof(WORD), D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_iBuf, 0 );

	mIndexBufSize = i_size;

	if( SUCCEEDED(hr) )
	{
		//This is good, so we can actually update the vertex buffer now.
		updateIndexBuffer();
	}
}

void DrawablePointList::updateVertexBuffer()
{
	//Check the vertex buffer to make sure it's been allocated.  If it hasn't, check the point_list size and if it's now
	// non-zero then I can build the buffer and update it.
	if( m_vBuf )
	{
		//So this means the buffer has already been allocated, now I can lock and push data in.		
		LOTSVertex * vertex = 0;

 		m_vBuf->Lock(0, 0, (void**)&vertex, 0);

		for ( unsigned index = 0; index < mVertexBufSize; ++index)
		{
			vertex[index].position = m_PList[index].position;
			vertex[index].color    = m_PList[index].color;
		}

		m_vBuf->Unlock();
	}
}

void DrawablePointList::updateIndexBuffer()
{
	if( m_iBuf )
	{
		WORD* index = 0;

		m_iBuf->Lock( 0, 0, (void**)&index, 0);

		//unsigned size = (unsigned)m_PList.size();

		for ( unsigned i = 0, k = 1, j = 0; j < mIndexBufSize; i+=2, k+=2, ++j)
		{
			index[i] = (WORD)i;
			index[k] = (WORD)k;
		}

		m_iBuf->Unlock();
	}
}

void DrawablePointList::InsertPointInList( const D3DXVECTOR3& point, unsigned insertIndex )
{
	if( 0 == insertIndex )
	{
		m_PList.insert( m_PList.begin(), LOTSVertex(point) );
	}
	else if( m_PList.size() == insertIndex )
	{
		m_PList.insert( m_PList.end(), LOTSVertex(point) );
	}
	//Here, go through and get the iterator to this index value, and
	// then insert the point there.
	else
	{
	}

	//PathInit();
}

void DrawablePointList::PushPointToBack(const D3DXVECTOR3 &point)
{
	InsertPointInList( point, NumPoints() );
}

void DrawablePointList::PushPointToFront(const D3DXVECTOR3 &point)
{
	InsertPointInList( point, 0 );
}


void DrawablePointList::Render()
{
	if( IsPathInit() )
	{
		//Okay, so here I'll set up the stream source and all that jazz and render this stuff using my
		// shader.

		// Clear the back buffer.
		//m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0, 255, 0), 1.0f, 0);

		// Begin the scene.  Start rendering.
		//m_device->BeginScene();


		//...All my rendering code goes here.//set the vertex declaration
		m_device->SetVertexDeclaration( LOTSVertex::Decl );

		//Set up stream source
		m_device->SetStreamSource( 0, m_vBuf, 0, sizeof(LOTSVertex) );
		m_device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


		//set render state		
		m_device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

			
				//Make call to draw primitive. 
		m_device->DrawPrimitive( D3DPT_LINELIST, 0,  mVertexBufSize );
		m_device->DrawPrimitive( D3DPT_POINTLIST, 0, mVertexBufSize );


		//(XObject)->RenderIt( g_D3DDevice );
		//(XObject)->MoveFrame( 0.1f, (XObject)->Frame()->TransformationMatrix );

		// End the scene.  Stop rendering.
	  // m_device->EndScene();

	   // Display the scene.
	  // m_device->Present(NULL, NULL, NULL, NULL);
	}
}




//////////////// HELPERS
/////////////////////////////////////////////////
unsigned DrawablePointList::NumPoints()
{	
	return (unsigned)m_PList.size(); 
}

LOTSVertex	DrawablePointList::GetPoint(unsigned int index)
{
	return m_PList[index];
}

D3DXVECTOR3	DrawablePointList::GetDXPoint( unsigned index )
{	
	return D3DXVECTOR3( m_PList[index].position.x, m_PList[index].position.y, m_PList[index].position.z );
}

void DrawablePointList::PathInit()
{
	if( 0 < m_PList.size() )
	{
		m_pathInit = true;
	}
	else
	{
		m_pathInit = false;
	}
}

inline bool DrawablePointList::IsPathInit()
{
	return m_pathInit;
}
//////////////// END HELPERS
/////////////////////////////////////////////////

