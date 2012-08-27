

#include "XFileObject.h"
#include "hierarchyentity.h"
#include "Structs.h"
#include <assert.h>

#pragma region Contructors, Destructors, Initializers

CXFileObject::CXFileObject() 
: 
mFrame(0), 
mAnimationController(), 
mNumBones(0), 
mbDrawSkeleton( true ),
mbDrawMesh( true ),
mWireframeMode(false)
{
}

CXFileObject::~CXFileObject()
{
	if (mFrame)
	{
		// Create a mesh heirarchy class to control the removal of memory for the frame heirarchy
#ifdef DIRECTX
		CHierarchyEntity Alloc;
		D3DXFrameDestroy(mFrame, &Alloc);
#else
		delete mFrame;
		mFrame = NULL;
#endif
	}
}

#ifdef DIRECTX
CXFileObject::CXFileObject( LPDIRECT3DDEVICE9 device, char* filename ) 
: 
mFrame(0), 
mNumBones(0), 
mMeshFirst(0), 
mRenderableSkeleton( device ),
mbDrawSkeleton( true ),
mbDrawMesh( true ),
mWireframeMode(false)
{
	//copy the filename over.
	memcpy( (void*)mFilename, (void*)filename, MAX_FILENAME_SIZE );
}


HRESULT CXFileObject::LoadXFileIn( LPDIRECT3DDEVICE9 device)
{
	HRESULT hr = NULL;

	if( device )
	{
		CHierarchyEntity allocator;

		WCHAR	tTempFilename[MAX_FILENAME_SIZE];
		CUtilities::FillWideStringFromCharString( mFilename, tTempFilename, MAX_FILENAME_SIZE );

		//now load the hierarchy in using 
		hr = D3DXLoadMeshHierarchyFromX( tTempFilename, D3DXMESH_MANAGED, device, &allocator, NULL, &mFrame, NULL ); 

		//Set the drawable mesh container to my internal pointer for easier access.
		mMeshFirst = (Extra_Mesh*)mFrame->pFrameFirstChild->pMeshContainer;

		if (mMeshFirst->pSkinInfo && mMeshFirst->MeshData.pMesh)
		{
			D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
			if (FAILED(mMeshFirst->MeshData.pMesh->GetDeclaration(Declaration)))
				return 0;

			//Clone the base mesh to the skinned mesh so we can set the vertex weights
			//	and stuff when rendering.
			mMeshFirst->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED, 
				Declaration, device, 
				&mMeshFirst->m_SkinMesh );
		}

		//Parse the animation file and get all the animations and keyframes.
		mAnimationController.GetParser()->ParseFile( mFilename );

		//Here, go through the animation controller and set all of the animations bone reference pointers.
		std::vector<D3DXVECTOR3>	tSkeletonList(0);
		CUtilities::ComputeBaseSkeleton( mFrame, NULL, tSkeletonList );

		mAnimationController.SetCurrentAnimationSet( 0 );
		mAnimationController.SetBoneReferences( mFrame );

		//@todo:
		//Go through initial matrix transforms, convert and set vqs for bones.  This way, all initial data is good
		//	and so the interpolated data should also be good.

		//Compute the lines for the skeleton of the model
		mRenderableSkeleton.CreateAndBuildBuffers( tSkeletonList );
	}

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return hr;
}

#pragma endregion Contructors, Destructors, Initializers

void CXFileObject::Render( LPDIRECT3DDEVICE9 device, LPD3DXEFFECT effect )
{
	//Check the device for validity.
	if ( device )
	{
		//Make sure the frame is valid too.
		if ( mFrame )
		{
			std::vector<D3DXVECTOR3>	tSkeletonList(0);
			//Call advance time.
			if( !mAnimationController.IsPaused() )
			{
				mAnimationController.AdvanceAnimationInTime( 100 );
				CUtilities::ComputeVQSSkeleton( mFrame, NULL, tSkeletonList );
				mRenderableSkeleton.CreatePointList( tSkeletonList );		
				mRenderableSkeleton.updateVertexBuffer();
			}
						
			if( effect )
			{
				UINT iPass, cPasses;		
				effect->Begin( &cPasses, 0);

				for ( iPass = 0; iPass < cPasses; ++iPass)
				{
					effect->BeginPass( iPass );

					if( mbDrawMesh )
					{
						D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
						mMeshFirst->MeshData.pMesh->GetDeclaration(Declaration);

						//This section of code updates the mesh from the currently running animation.
						if (mMeshFirst->pSkinInfo && mMeshFirst )
						{
							UINT bones = mMeshFirst->pSkinInfo->GetNumBones();

							for (UINT i=0; i < bones; ++i)
							{
								LPD3DXFRAME tCurKeyFrame = NULL;
								D3DXMATRIX tOffsetMat = *mMeshFirst->pSkinInfo->GetBoneOffsetMatrix(i);
								CUtilities::FindBoneReference( mFrame, tCurKeyFrame, (char*)mMeshFirst->pSkinInfo->GetBoneName(i) );
								D3DXMATRIX mCurFrameMat = ((Extra_Frame*)tCurKeyFrame)->m_vqsTransformToRoot.toMatrix();
								D3DXMatrixMultiply( &mBoneMats[i], mMeshFirst->pSkinInfo->GetBoneOffsetMatrix(i), &mCurFrameMat );
							}

							if( mWireframeMode )
							{
								device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
							}
							else
							{
								device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
							}

							device->SetVertexDeclaration( (IDirect3DVertexDeclaration9*)Declaration );

							void *srcPtr;
							mMeshFirst->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&srcPtr);

							void *destPtr;
							mMeshFirst->m_SkinMesh->LockVertexBuffer(0, (void**)&destPtr);

							// Update the skinned mesh 
							mMeshFirst->pSkinInfo->UpdateSkinnedMesh(mBoneMats, NULL, srcPtr, destPtr) ;

							// Unlock the meshes vertex buffers
							mMeshFirst->m_SkinMesh->UnlockVertexBuffer() ;
							mMeshFirst->MeshData.pMesh->UnlockVertexBuffer() ;
						}

						//End mesh update.

						//Renders the mesh actually with any materials or tectures, 
						if (mMeshFirst->pSkinInfo)
						{
							mMeshFirst->m_SkinMesh->DrawSubset(0);
						}

						else
						{
							mMeshFirst->MeshData.pMesh->DrawSubset(0);
						}
					}

					
					if( mbDrawSkeleton )
					{					
						//Update new bone structure.
						mRenderableSkeleton.Render();				
					}

					effect->EndPass();
				}

				effect->End();
			}			
		}
	}
}
#endif

//@todo:
// Make an update vqs frames function which will go through the frame and update
//	all of the frames interpolated matrices (AdvanceTime and UpdateFrameMats ).








