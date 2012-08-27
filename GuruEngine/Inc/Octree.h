/*

Dan Strycharske
PHY350

06/20/08
Octree.h

The interface for my dynamic octree which will be a major portion of the braod phase of the
	simulation.  This file also contains the interfaces for the octree nodes and other objects
	it will need to have.
*/

#ifndef	OCTREE_H
#define OCTREE_H

#include <vector>
#include <d3dx9math.h>


const unsigned NUM_OCT_CHILDREN		= 8;
const unsigned MAX_OCT_DEPTH		= 4;
const unsigned MAX_STRADDLE_NODES	= 5;
const unsigned MAX_SPLIT_PLANES		= 3;
const unsigned MAX_OBJS_IN_NODE		= 10;
const float	   RENDER_EPSILON		= 100.0f;

enum AXES { X = 0, Y, Z };

//Header includes

//Class forward declarations
class CPriorityQueue;
class GameObject;
class CAxisPlane;
class CAABB;
class RenderableHierarchy;


class CAxisPlane
{
public:
	//Creates a degenerate plane.  Not technically initialized.
	CAxisPlane() : mNormal(0,0,0), mDVal(0.f), mIsInitialized(false)
	{
	}

	//Creates a plane based off of a point on the plane and the axis its normal is
	// aligned with.
	CAxisPlane( const D3DXVECTOR3&	pPointOnPlane, unsigned pAxis ) : mNormal(0,0,0), mDVal(0.f), mIsInitialized(true)
	{
		mNormal[pAxis] = 1;

		//Now that we have the normal, we dot it with the incoming point to generate the d value.
		mDVal = D3DXVec3Dot( &mNormal, &pPointOnPlane );
	}

private:
	//Normal vector to the axis aligned plane.
	D3DXVECTOR3	mNormal;

	//The d value distance offset of the plane.
	float		mDVal;

	//Status flag for initialized or not.
	bool		mIsInitialized;
};


class CAABB
{
public:
	//Creates a zeroed out box.
	CAABB() : mMin(), mMax(), mIsInitialized(false) {}

	//Creates a box from min and max points given.
	CAABB( const D3DXVECTOR3& pMin, const D3DXVECTOR3& pMax ) : mMin(pMin), mMax(pMax), mIsInitialized(true) {}

	//Getters for the points.
	D3DXVECTOR3 GetMin()	{ return mMin; }
	D3DXVECTOR3 GetMax()	{ return mMax; }
	D3DXVECTOR3 GetCenter()	{ return (mMin + ((mMax-mMin) / 2)); }


private:
	//The two points.
	D3DXVECTOR3	mMin, mMax;

	//Initialized bool
	bool		mIsInitialized;
};




/*
class:	COctNode

brief:	
	This class will store all of the needed data for the partitioning.  That data is specified
	lower.  The nodes will be memory managed by the tree itself, allocating and deallocting as
	is necessary.  Along with all of the data it needs to actually partition and store the scene
	data, it will also contain rendering buffers so that I can view the scene partitioning.

	1) All objects contained in this node.
	2) Up to 3 axis oriented splitting planes.
	3) 8 Node pointers.
	4) AABB
	5) Flag for if it is up to date in the scene.
	6) Vertex buffer for the AABB.
	7) Index buffer for the AABB.


*/
class COctNode
{		
private:
	///HERE"S ALL THE MEMBERS OF THE STRUCT//////

	//A priority queue of objects contained in this cell, which will be sorted along which axis of that
	//	AABB has the greatest extent.
	CPriorityQueue*			mObjectsSorted;

	//Three planes.  A plane consists of a point on the plane and a normal vector.  This is a special kind of
	// plane called an axis plane, which will make it easier to generate the normal for the plane.
	CAxisPlane*				mSplitPlanes[MAX_SPLIT_PLANES];

	//An array of 8 node pointers.  HAVE TO MAKE SURE THEY"RE NOT NULL FOR ANY TRAVERSING PURPOSES.
	COctNode*				mChildNodes[NUM_OCT_CHILDREN];

	COctNode*				mParentNode;

	//Here's the objects AABB, defined by two points, the min and max.
	CAABB*					mBoundingBox;

	//This boolean lets the octree know if something has changed in this node and if it's up to date.
	bool					mNeedToUpdate;

	//Here's the rendering hierarchy that will draw the bounding volume for me.
	RenderableHierarchy*	mRenderVolume;

	//Counter of how many children have been allocated.
	unsigned				mNextChildToAllocate;

public:
	//Default constructor
	COctNode();

	COctNode( const D3DXVECTOR3& pMin, const D3DXVECTOR3& pMax ); 

	~COctNode();

	//After a node is created, it should be initialized with the current list of objects that are coming 
	// in from the level above.
	bool	Init( std::vector< GameObject* >	pObjects );

	//This will basically call heap_sort on the priority queue using the GreaterThan functor, only now
	// it will specify an axis that will be tested for the sorting.
	void	SortByAxis(	unsigned pAxis );

	//Set a specific axis' splitting plane.
	void	SetSplitPlane( CAxisPlane* pNewPlane, unsigned pAxisIndex );

	//Getters.
	CPriorityQueue*	GetHeap();
	CAxisPlane*			GetSplitPlane( unsigned pIndex );

	//Tests an object against a plane of the node.
	bool	ObjectInsideHalfSpace( GameObject*	pObject, unsigned pAxisIndex );

	//This function will create the specified axis split plane for this node.
	bool	CreatePlane( const D3DXVECTOR3&	pPointOnPlane, unsigned pAxis );

	bool	CreateAABB( const D3DXVECTOR3&	pMin, const D3DXVECTOR3&	pMax );

	//Creates a child node based on certain bounding values.
	bool	CreateChildNode( const D3DXVECTOR3&	pMin, const D3DXVECTOR3&	pMax );

	//If the bounding volume has been defined, then we can grab the center of it using this function.
	D3DXVECTOR3	GetCenter();

	//This will return the box pointer so that it we can access it's data.
	CAABB*	GetBox();

	//This function will take the AABB, after making sure it exists, and uses it's points in a vertex
	// buffer.  Then some clever indexing will create all of the lines for the box.
	void	CreateVisualBoxVolume();

	//This is an obvious helper function that calls the renderableHierarchy's render function.  If all
	// is set right then it should draw the edges of teh bounding volume for this node.
	void	Render();

	//This getter returns a pointer to the child node at the given index, or NULL if no child exists.
	COctNode*	GetChild( unsigned index );

};





/*

class:	CDynamicOctree

brief:	
	This class will store all of the nodes in the scene and manage the memory usage of the
	tree as a whole.  It will of course hold the root of the tree.

	The tree will have the typical functionality of an octree.  This includes:
	
	1) Finding a node.
	2) Adding a node.
	3) Deleting a node.
	
	On top of this functionality, there are going to be several new functions that I'll implement
	in order to improve the splitting of the node when it comes to it.

	4) SplitNode
	5) DetermineSplittingPlane.
*/
class CDynamicOctree
{
private:
	//The root of the tree.
	COctNode*		mRoot;

	//The current depth of the tree.
	unsigned		mCurrentDepth;

	//
	
public:

	//Constructor
	CDynamicOctree() {}

	//Destructor
	~CDynamicOctree() 
	{ 
		if( mRoot )
		{
			delete mRoot;
			mRoot = NULL;
		}
	}

	//Create the octree from a list of game object pointers.	
	bool	InitialTreeCreate( std::vector< GameObject* >	pObjects );

	//Split node, which returns a bool if split successfully.  If returns false, that means the
	// scene is not well situated for this.
	bool	SplitNode( COctNode* pNodeToSplit );

	//This checks how many objects are within this node.  If it's beyond a threshold value, then 
	// the node is sent to get split.
	bool	ShouldNodeBeSplit( COctNode* pNode );

	//This finds an object in the tree, and removes it completely.  
	void	RemoveObject( GameObject*	pUpdatedObject );

	void	InsertObject( GameObject*	pObject );

	//This will be the update to the GameObject*, which will leverage the remove and insert functions.
	void	UpdateAndReinsert( GameObject*	pObject );
	
	//Tests whether the object is inside this node.
	bool	TestObjectInNode( GameObject*	pObject, COctNode* mTestingNode );

	//This will traverse through the entire tree, and render each node.  The nodes hopefully were given
	// a random color so as it will be easier to see the different octants.
	void	RenderTree();



	/// NOW STARTS THE FUNCTIONALITY FOR KEEPING THE TREE UP TO DATE AND DYNAMIC. ///


};


#endif


