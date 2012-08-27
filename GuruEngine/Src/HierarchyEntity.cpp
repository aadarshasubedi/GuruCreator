/*
Author:	Dan Strycharske
Project: Inner Knight
Team:	Macrohard
DigiPen Institute of Technology owns this and our souls.

File:	HierarchyEntity.cpp
brief:	Implementation of the necessary derived mesh hierarchy class.
		This derives from ID3DXAllocateHierarchy object, which has 
		four methods which need to be implemented.  
		These methods will then be called internally by the 
		D3DXLoadMeshHierarchyFromX(..., mesh hierarchy* alloc, ...) function 
		via passing in one we've created to be used through this parameter.
*/

#include "HierarchyEntity.h"
#include "Utilities.h"
#include "Structs.h"

#ifdef DIRECTX
HRESULT CHierarchyEntity::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	*ppNewFrame = NULL;

	// Create a new frame
	Extra_Frame* pNewFrame = new Extra_Frame;
	
	// Initialize the frame's memory to all zeroes (or NULL if ref type) and set the
	// member matrices to be the Identity matrix.
	ZeroMemory( pNewFrame, sizeof(Extra_Frame) );


  	// If a name was included with the frame, make a deep copy of that string in the
  	// Frame struct.
	pNewFrame->Name = CUtilities::DuplicateCharString(Name);

	//Set all the transformation matrices to identity
	D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pNewFrame->m_combined_transformMat);
	
	//Set child and sibling pointers to null.  Since there is no mesh
	//associated with this frame yet, set the mesh container to null.
	pNewFrame->pFrameFirstChild = NULL;
	pNewFrame->pFrameSibling = NULL;
	pNewFrame->pMeshContainer = NULL;


	// Finally, we set the ppNewFrame [OUT] parameter to the newly created frame
	*ppNewFrame = pNewFrame; //<- for reference, Extra_Frame extends LPD3DXFRAME so no cast	
	
	return S_OK;
}


HRESULT CHierarchyEntity::CreateMeshContainer(
										LPCSTR Name, 
										CONST D3DXMESHDATA * pMeshData, 
										CONST D3DXMATERIAL * pMaterials, 
										CONST D3DXEFFECTINSTANCE * pEffectInstances,
										DWORD NumMaterials, 
										CONST DWORD * pAdjacency, 
										LPD3DXSKININFO pSkinInfo, 
										LPD3DXMESHCONTAINER * ppNewMeshContainer )
{

	//1. Create new mesh container.
	//2. Make sure it's of proper mesh type.

	//3. Fill out these fields of the D3DXMESHCONTAINER:
	/*
		>Name				(LPSTR)
		pMeshData			(D3DXMESHDATA)	-	{ D3DXMESHDATATYPE type; }
		>pMaterials			(LPD3DXMATERIAL)-	{ D3DMATERIAL9 MatD3D; LPSTR pTextureFilename; }
		>pEffects			(LPD3DXEFFECTINSTANCE)	-	{ LPSTR pEffectFilename; DWORD NumDefaults; LPD3DXEFFECTDEFAULT pDefaults;
		>NumMaterials		(DWORD*)		-   use GetNumMaterials()
		>pAdjacency			(DWORD*)		-	just copy over
		pSkinInfo			(LPD3DXSKININFO*) - just copy over
		pNextMeshContainer	(D3DXMESHCONTAINER*)	-	set to NULL for now.
	*/
	
	//Creation of new mesh container
	Extra_Mesh* pNewMesh = new Extra_Mesh;

	ZeroMemory( pNewMesh, sizeof(Extra_Mesh) );
	*ppNewMeshContainer = NULL;

	//Now, start copying members over.
	pNewMesh->Name=CUtilities::DuplicateCharString(Name);

	//Check incoming mesh type.  if not standard, reject and destroy container.
	//If valid, set mesh type to be standard
	//Then get number of faces and adjacency info and copy it over.
	if (pMeshData->Type != D3DXMESHTYPE_MESH )
	{
		DestroyMeshContainer( pNewMesh );
		return E_FAIL;
	}

	//Sets new mesh type to direct 3d mesh type, gets number of faces and allocates
	//an array for the adjacency info, enough for all the faces times 3 for each pt.
	pNewMesh->MeshData.Type = D3DXMESHTYPE_MESH;
	DWORD numFaces = pMeshData->pMesh->GetNumFaces();
	pNewMesh->pAdjacency = new DWORD[numFaces*3];
	memcpy( pNewMesh->pAdjacency, pAdjacency, sizeof(DWORD)*numFaces*3 );

	//check for effects, if there, handle them....  we're not doing that just yet.
	if (pEffectInstances)
	{
		//... handle that stuff here
	}

	//Now grab the directx device and then clone the mesh into the new container.
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pMeshData->pMesh->GetDevice(&pDevice);

	if ( pDevice == NULL )
	{
		FAILED(E_FAIL);
	}	

	D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
	if (FAILED(pMeshData->pMesh->GetDeclaration(Declaration)))
		return E_FAIL;

	pMeshData->pMesh->CloneMesh(D3DXMESH_MANAGED, 
		Declaration, pDevice, 
		&pNewMesh->MeshData.pMesh);

	//Here get number of materials and allocate a material and texture buffer.
	//Then loop through the materials and put them into said buffer, while also
	//grabbing the texture files and setting them into the mesh as well.
	pNewMesh->NumMaterials = max(NumMaterials,1);
	pNewMesh->m_Materials = new D3DMATERIAL9[ pNewMesh->NumMaterials ];
	pNewMesh->m_Textures = new LPDIRECT3DTEXTURE9[ pNewMesh->NumMaterials ];
	ZeroMemory( pNewMesh->m_Textures, sizeof(LPDIRECT3DTEXTURE9)*pNewMesh->NumMaterials );

	if (NumMaterials > 0)
	{
		//loop through them materials and set them into the array.

		for (DWORD index=0; index < NumMaterials; ++index)
		{
			//Set texture initially to null, will be checked later to see if it has a real texture.
			pNewMesh->m_Textures[index] = NULL;

			//Get the mat from the incoming materials array.
			pNewMesh->m_Materials[index] = pMaterials[index].MatD3D;

			//Here we check to see if it has a texture file associated with it
			if (pMaterials[index].pTextureFilename )
			{
				
				WCHAR wszTexName[256];

				//Make sure material list has a texture at given index, and that the name is valid
				if (!pMaterials[index].pTextureFilename || !wszTexName)
					FAILED(E_FAIL);				

				//Make sure it can be converted into a wchar string.
				if ( MultiByteToWideChar( CP_ACP, 0, pMaterials[index].pTextureFilename, -1, wszTexName, (int)256 ) == 0 )
					FAILED(E_FAIL);		

				//If all good, create a texture from it and set into the meshes texture list at present index.
				if ( FAILED( D3DXCreateTextureFromFile(pDevice, wszTexName, &pNewMesh->m_Textures[index] ) ) )
				{
					pNewMesh->m_Textures[index] = NULL;
				}
			}
		}

	}
	else
	{
		//No materials, so set up a default one for now.
		ZeroMemory( &pNewMesh->m_Materials[0], sizeof(D3DMATERIAL9) );
		pNewMesh->m_Materials[0].Diffuse.r = 0.75f;
		pNewMesh->m_Materials[0].Diffuse.g = 0.0f;
		pNewMesh->m_Materials[0].Diffuse.b = 1.0f;
		pNewMesh->m_Materials[0].Specular = pNewMesh->m_Materials[0].Diffuse;
		pNewMesh->m_Textures[0] = NULL;
	}

	//Now check skin info and grab them bones people.
	if (pSkinInfo)
	{
		pNewMesh->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		//Now grab the number of bones, their offset matrices, and then set them in the big array of array of matrices
		UINT numBones = pSkinInfo->GetNumBones();
		pNewMesh->m_BoneOffsets = new D3DXMATRIX[ numBones ];
		//pNewMesh->m_AllBoneFrameMats = new D3DXMATRIX*[ numBones ];

		for (UINT index=0; index < numBones; ++index)
		{
			pNewMesh->m_BoneOffsets[index] = *( pNewMesh->pSkinInfo->GetBoneOffsetMatrix(index) );
		}
	}
	else
	{
		//Otherwise there's no bone info, so anything related and relevant null out.
		//pNewMesh->m_AllBoneFrameMats = NULL;
		//pNewMesh->m_BoneOffsets = NULL;
		pNewMesh->pSkinInfo = NULL;
		pNewMesh->m_SkinMesh = NULL;
	}

	//Get rid of that device copy.
	SAFE_RELEASE(pDevice);

	//Set the needed container to this newly created one
	*ppNewMeshContainer = pNewMesh;

	return S_OK;	

}


HRESULT CHierarchyEntity::DestroyFrame( LPD3DXFRAME pDeadFrame )
{
	//Cast dead frame to extra frame to ensure all fields and memory 
	//are released when the object is destroyed.
	Extra_Frame *pFrame = (Extra_Frame*)pDeadFrame;

	//Deletes dynamically allocated name array
	SAFE_DELETE_ARRAY(pFrame->Name);
	//Delete the frame pointer, and thus the frame.
	SAFE_DELETE(pFrame);
	
	return S_OK;
}


HRESULT CHierarchyEntity::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerToFree )
{
	// Convert to our extended type. OK as we know for sure it is:
    Extra_Mesh* pMeshContainer = (Extra_Mesh*)pMeshContainerToFree;
	
	// name
	SAFE_DELETE_ARRAY(pMeshContainer->Name)

	// material array
	SAFE_DELETE_ARRAY(pMeshContainer->m_Materials)

	// release the textures before deleting the array
	if(pMeshContainer->m_Textures)
	{
		for(UINT i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			SAFE_RELEASE(pMeshContainer->m_Textures[i]);
		}
	}

	// texture array
	SAFE_DELETE_ARRAY(pMeshContainer->m_Textures)

	// adjacency data
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency) 
	
	// release skin mesh
	SAFE_RELEASE(pMeshContainer->m_SkinMesh)
	
	// release the main mesh
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh)
	
	// release skin information
	SAFE_RELEASE(pMeshContainer->pSkinInfo)
	
	// finally delete the mesh container itself
	SAFE_DELETE(pMeshContainer);

    return S_OK;
}

#else

#endif