
#include "Octree.h"
#include "GameObject.h"
#include "World.h"
//#include "CPriorityQueue.h"
#include "PathingClasses.h"
#include <cstdlib>		//Using the rand and srand functions.

//class CPlane;
//class CAABB;


/*****************

COctNode
******************/
COctNode::COctNode() : mNextChildToAllocate(0)
{
	//Initialize all pointers to NULL, and zero out all other members.
	mObjectsSorted = NULL;
	
	for( unsigned iter = 0; iter <  MAX_SPLIT_PLANES; ++iter )
	{
		mSplitPlanes[iter] = NULL;
	}

	for( unsigned iter = 0; iter <  NUM_OCT_CHILDREN; ++iter )
	{
		mChildNodes[iter] = NULL;
	}

	mBoundingBox = NULL;
	mParentNode = NULL;
	mRenderVolume = NULL;
}

COctNode::COctNode(const D3DXVECTOR3 &pMin, const D3DXVECTOR3 &pMax) : mNextChildToAllocate(0), mNeedToUpdate(false)
{
	//Initialize all pointers to NULL, and zero out all other members.
	mObjectsSorted = NULL;

	for( unsigned iter = 0; iter <  MAX_SPLIT_PLANES; ++iter )
	{
		mSplitPlanes[iter] = NULL;
	}

	for( unsigned iter = 0; iter <  NUM_OCT_CHILDREN; ++iter )
	{
		mChildNodes[iter] = NULL;
	}

	mBoundingBox = NULL;
	mParentNode = NULL;
	mRenderVolume = NULL;

	//Create the bounding box with these initial points.
	CreateAABB( pMin, pMax );

	//Since we've created the AABB right away, we can also go ahead and create the
	// renderable hierarchy.
	CreateVisualBoxVolume();
}

COctNode::~COctNode()
{
	//We've allocated a lot of memeory for this node, so clear it all up.mObjectsSorted = NULL;
	for( unsigned planeIter = 0; planeIter < MAX_SPLIT_PLANES; ++planeIter )
	{
		if( mSplitPlanes[planeIter] )
		{
			delete mSplitPlanes[planeIter];
			mSplitPlanes[planeIter] = NULL;
		}		
	}

	for( unsigned iter = 0; iter <  NUM_OCT_CHILDREN; ++iter )
	{
		if( mChildNodes[iter] )
		{
			delete mChildNodes[iter];
			mChildNodes[iter] = NULL;
		}
	}

	if( mBoundingBox )
	{
		delete mBoundingBox;
		mBoundingBox = NULL;
	}
	if( mParentNode )
	{
		delete mParentNode;
		mParentNode = NULL;
	}
	if( mRenderVolume )
	{
		delete mRenderVolume;
		mRenderVolume = NULL;
	}
}



bool	COctNode::CreatePlane( const D3DXVECTOR3&	pPointOnPlane, unsigned pAxis )
{
	//@todo:  Easy to create a plane from this jive.  The normals will be dictated by the
	//			pAxis parameter, and the d value of the plane will be dealt with using the 
	//			pPointOnPlane parameter.
	if( !mSplitPlanes[pAxis] )
	{
		mSplitPlanes[pAxis] = new CAxisPlane( pPointOnPlane, pAxis );
		return true;
	}

	//This false return also means that the plane already existed, and so it was updated instead.
	return false;
}

bool	COctNode::CreateAABB(  const D3DXVECTOR3&	pMin, const D3DXVECTOR3&	pMax )
{
	if( !mBoundingBox )
	{
		mBoundingBox = new CAABB( pMin, pMax );
		return true;
	}

	//The false return means that the box was already created, and has now been updated.
	return false;
}

bool	COctNode::CreateChildNode(  const D3DXVECTOR3&	pMin, const D3DXVECTOR3&	pMax )
{
	if( !mChildNodes[mNextChildToAllocate] && mNextChildToAllocate < NUM_OCT_CHILDREN )
	{
		mChildNodes[mNextChildToAllocate] = new COctNode(pMin, pMax);
		++mNextChildToAllocate;
		return true;
	}

	//This means that something bad happened, perhaps a memory allocation problem.
	return false;
}

//If the bounding volume has been defined, then we can grab the center of it using this function.
D3DXVECTOR3	COctNode::GetCenter()
{
	//Check and make sure the box isn't null.
	if( mBoundingBox )
	{
		//@todo:  Implement this function in the CAABB class.
		//Now grab the center using the boxes helper function.
		return mBoundingBox->GetCenter();
	}

	return D3DXVECTOR3();
}

//This will return the box pointer so that it we can access it's data.
CAABB*	COctNode::GetBox()
{
	//Of course, check for NULL pointers.
	if( mBoundingBox )
	{
		return mBoundingBox;
	}

	return NULL;
}

void	COctNode::CreateVisualBoxVolume()
{
	if( !mRenderVolume )
	{
		mRenderVolume = new RenderableHierarchy(8);
	}
	else
	{
		//Means we've already done this, and so we should call instead
		//  some sort of update on the list instead.
		return;
	}

	//Generate the 8 points of the bounding box.  Do this explicitly.
	D3DXVECTOR3 points[8];
	
	points[0] = mBoundingBox->GetMin();
	points[6]  = mBoundingBox->GetMax();

	points[1] = D3DXVECTOR3( points[6].x, points[0].y, points[0].z );
	points[2] = D3DXVECTOR3( points[6].x, points[0].y, points[6].z );
	points[3] = D3DXVECTOR3( points[0].x, points[0].y, points[6].z );

	points[4] = D3DXVECTOR3( points[0].x, points[6].y, points[0].z );
	points[5] = D3DXVECTOR3( points[6].x, points[6].y, points[0].z );
	points[7] = D3DXVECTOR3( points[0].x, points[6].y, points[6].z );

	srand( (int)(points[6].x * points[0].y + 20.f) );
	float RandRedCol = 0.1f * (float)(rand() % 10);

	srand( (int)(points[6].z * points[0].x + 30.f) );
	float RandGreenCol = 0.1f * (float)(rand() % 10);

	srand( (int)(points[6].y * points[0].z + 50.f) );
	float RandBlueCol = 0.1f * (float)(rand() % 10);

	//Here I'm setting the index buffer.
	for( unsigned ptIter = 0; ptIter < NUM_OCT_CHILDREN; ++ ptIter )
	{
		mRenderVolume->updateDataInBuffer( ptIter, points[ptIter], D3DXCOLOR( RandRedCol, RandGreenCol, RandBlueCol, 1.0f ) );
	}

	//Here I'm setting up the index buffer.  This will also be explicit setting of each line.

	//The indexing for the lines goes like this.
	//
	//0 = min
	//
	//6 = max
	//
	//0-1, 1-2, 2-3, 3-0
	//
	//0-4, 1-5, 2-6, 3-7
	//
	//4-5, 5-6, 6-7, 7-4
	//
	//
	unsigned index = 0;
	mRenderVolume->AddLineToRenderList( 0,1, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 1,2, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 2,3, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 3,0, index);
	index += 2;

	mRenderVolume->AddLineToRenderList( 0,4, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 1,5, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 2,6, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 3,7, index);
	index += 2;

	mRenderVolume->AddLineToRenderList( 4,5, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 5,6, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 6,7, index);
	index += 2;
	
	mRenderVolume->AddLineToRenderList( 7,4, index);
	index += 2;

	mRenderVolume->SetIndexedList();
}

//This is an obvious helper function that calls the renderableHierarchy's render function.  If all
	// is set right then it should draw the edges of teh bounding volume for this node.
void	COctNode::Render()
{
	if( mRenderVolume )
	{
		mRenderVolume->Render();
	}
}

//This getter returns a pointer to the child node at the given index, or NULL if no child exists.
COctNode*	COctNode::GetChild( unsigned index )
{
	if( !mChildNodes[index] || index < 0 || index >= NUM_OCT_CHILDREN )	{ return NULL; }

	return mChildNodes[index];
}


inline CPriorityQueue*	COctNode::GetHeap()
{ 
	return mObjectsSorted;
}

inline CAxisPlane*	COctNode::GetSplitPlane( unsigned pIndex )	
{ 
	if( mSplitPlanes[pIndex] )
	{ 
		return mSplitPlanes[pIndex];
	} 
}




/*****************

CDynamicOctree
******************/
bool	CDynamicOctree::InitialTreeCreate( std::vector< GameObject* >	pObjects )
{
	//Allocate the root, giving it the min and max points of the aabb.
	mRoot = new COctNode( World::GetSingleton().GetMinPoint(), World::GetSingleton().GetMaxPoint() );

	//First off, create the three planes, and create the 8 children of this node.
	mRoot->CreatePlane( D3DXVECTOR3(0,0,0), PHYSICS::X );
	mRoot->CreatePlane( D3DXVECTOR3(0,0,0), PHYSICS::Y );
	mRoot->CreatePlane( D3DXVECTOR3(0,0,0), PHYSICS::Z );

	//The AABB should already be created at this point, so we can define all of the children now.
	//  This could be done in a loop, but for now hardcode all 8 to be the various min, max values of the 
	//  boxes dimensions.
	//Create the 8 points of the box, and use them to create the new nodes.

	if( mRoot->GetBox() )
	{
		D3DXVECTOR3	center = mRoot->GetCenter();
		D3DXVECTOR3 minPt, maxPt(1000,1000,1000);
		minPt = mRoot->GetBox()->GetMin();
		maxPt = mRoot->GetBox()->GetMax();

		D3DXVECTOR3	offset(RENDER_EPSILON, RENDER_EPSILON, RENDER_EPSILON);

		mRoot->CreateChildNode( minPt + offset, center - offset );
		mRoot->CreateChildNode( D3DXVECTOR3(center.x, minPt.y, minPt.z) + offset, D3DXVECTOR3(maxPt.x, center.y, center.z) - offset );
		mRoot->CreateChildNode( D3DXVECTOR3(minPt.x, minPt.y, center.z) + offset, D3DXVECTOR3(center.x, center.y, maxPt.z) - offset );
		mRoot->CreateChildNode( D3DXVECTOR3(center.x, minPt.y, center.z) + offset, D3DXVECTOR3(maxPt.x, center.y, maxPt.z) - offset );

		mRoot->CreateChildNode( D3DXVECTOR3(minPt.x, center.y, minPt.z) + offset, D3DXVECTOR3(center.x, maxPt.y, center.z) - offset );
		mRoot->CreateChildNode( D3DXVECTOR3(center.x, center.y, minPt.z) + offset, D3DXVECTOR3(maxPt.x, maxPt.y, center.z) - offset );
		mRoot->CreateChildNode( D3DXVECTOR3(minPt.x, center.y, minPt.z) + offset, D3DXVECTOR3(center.x, maxPt.y, maxPt.z) - offset );
		mRoot->CreateChildNode( center + offset, maxPt - offset );
	}
	else
	{
		return false;
	}

	return true;
}

//Find a node in the tree.

//Split node, which returns a bool if split successfully.  If returns false, that means the
// scene is not well situated for this.
bool	CDynamicOctree::SplitNode( COctNode* pNodeToSplit )
{
	return false;
}

//This checks how many objects are within this node.  If it's beyond a threshold value, then 
// the node is sent to get split.
bool	CDynamicOctree::ShouldNodeBeSplit( COctNode* pNode )
{
	return false;
}

//This finds an object in the tree, and removes it completely.  
void	CDynamicOctree::RemoveObject( GameObject*	pUpdatedObject )
{
}

void	CDynamicOctree::InsertObject( GameObject*	pObject )
{
}

//This will be the update to the GameObject*, which will leverage the remove and insert functions.
void	CDynamicOctree::UpdateAndReinsert( GameObject*	pObject )
{
}

//Tests whether the object is inside this node.
bool	CDynamicOctree::TestObjectInNode( GameObject*	pObject, COctNode* mTestingNode )
{
	return false;
}
//This will traverse through the entire tree, and render each node.  The nodes hopefully were given
// a random color so as it will be easier to see the different octants.
void	CDynamicOctree::RenderTree()
{
	if( mRoot )
	{
		//Render the root.
		mRoot->Render();

		//Now go through all of the children and render their boxes as well.
		for( unsigned childIter = 0; childIter < 8; ++childIter )
		{
			if( mRoot->GetChild( childIter ) )
			{
				mRoot->GetChild( childIter )->Render();
			}
		}
	}
}


