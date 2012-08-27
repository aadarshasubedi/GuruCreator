
/*
Author:	Dan Strycharske
Project: Inner Knight
Team:	Macrohard
DigiPen Institute of Technology owns this and our souls.

File:	HierarchyEntity.h
brief:	This is our class that derives from ID3DXAllocateHierarchy.  We implement
		this abstract base class with callback functions to load in the hierarchy
		of all our meshes, sort materials and associate textures properly.

*/

#if !defined( HIERARCHYENTITY_H_ )
#define HIERARCHYENTITY_H_

#include <basetyps.h> //<- STDMETHOD, STDMETHODCALLTYPE, THIS_



#define __WITH_DX__ "#ifdef DIRECTX : public ID3DXAllocateHierarchy #endif"


#ifdef DIRECTX
// include our extended structure definitions
#include "d3d9.h"
#include "d3dx9.h"
#include "d3dx9anim.h"


//
//class CHierarchyEntity : public ID3DXAllocateHierarchy
//{
//public:
//	//callback to create a frame and initialize it
//	STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );
//
//	//callback to create a mesh container and initiatlize it
//	STDMETHOD( CreateMeshContainer )(	THIS_ LPCSTR Name, 
//										CONST D3DXMESHDATA * pMeshData, 
//										CONST D3DXMATERIAL * pMaterials, 
//										CONST D3DXEFFECTINSTANCE * pEffectInstances,
//										DWORD NumMaterials, 
//										CONST DWORD * pAdjacency, 
//										LPD3DXSKININFO pSkinInfo, 
//										LPD3DXMESHCONTAINER * ppNewMeshContainer );
//
//	//callback to destroy and deallocate frame	
//    STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pDeadFrame );
//
//	//callback to destroy and deallocate mesh container	
//    STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree );
//
//protected:
//private:
//
//};

#else

//In DirectX this is a specialized allocator class for certain objects.
class CHierarchyEntity 
#ifdef DIRECTX
	: public ID3DXAllocateHierarchy
#endif
{
	//Here, it will serve as the loader/converter and allocator object, rather than separate those two.
	//It will not be based on any specific file type, but rather will use a generic transform system
	// and mesh system to load and/or convert most readily used file types to the generic system that
	// everyone can use.  DirectX is just so specific it needs it's own.

#ifdef DIRECTX
	public:
		//callback to create a frame and initialize it
		STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );
	
		//callback to create a mesh container and initiatlize it
		STDMETHOD( CreateMeshContainer )(	THIS_ LPCSTR Name, 
											CONST D3DXMESHDATA * pMeshData, 
											CONST D3DXMATERIAL * pMaterials, 
											CONST D3DXEFFECTINSTANCE * pEffectInstances,
											DWORD NumMaterials, 
											CONST DWORD * pAdjacency, 
											LPD3DXSKININFO pSkinInfo, 
											LPD3DXMESHCONTAINER * ppNewMeshContainer );
	
		//callback to destroy and deallocate frame	
	    STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pDeadFrame );
	
		//callback to destroy and deallocate mesh container	
	    STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree );
#endif
};

#endif

#endif

