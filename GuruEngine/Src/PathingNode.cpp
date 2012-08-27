/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	PathingNode.h
brief:	

*/

#include "PathingNode.h"
#include "Graphics.h"
#include "CPicker.h"
#include "Movement.h"



/*********************************

	PATHING NODES

*********************************/
PathingArea::PathingArea() : mCurrentHierarchy(0)
{
	//Create an empty hierarchy so that we can begin to add nodes immediately.
}


PathingArea::~PathingArea()
{
	unsigned size = (unsigned)mHierarchies.size();
	for( unsigned death_iter = 0; death_iter < size; ++death_iter )
	{
		delete mHierarchies[death_iter];
	}
	mCurrentHierarchy = 0;
	guid_counter = 0;
}

void PathingArea::Init()
{
	mHierarchies.push_back( new PathingHierarchy() );
}

void PathingArea::AddNodeToCurrentHierarchy( PathingNode*	ptrNode )
{
	mHierarchies[mCurrentHierarchy]->AddNode( ptrNode );
}

PathingNode*	PathingArea::GetNodeByGUID( int pGUID )
{
	unsigned size = (unsigned)mHierarchies.size();
	for( unsigned hier_iter = 0; hier_iter < size; ++hier_iter )
	{
		PathingHierarchy*	hierarchy = mHierarchies[hier_iter];

		//Now loop through all of the nodes in the list and check the guid's.
		unsigned num_nodes = (unsigned)hierarchy->GetNumNodes();
		for( unsigned node_iter = 0; node_iter < num_nodes; ++node_iter )
		{
			if( hierarchy->GetNode( node_iter )->GetGUID() == pGUID )
			{
				return hierarchy->GetNode( node_iter );
			}
		}
	}

	//If it ever gets all the way through, that node doesn't exist.
	return NULL;
}








/*********************************

	PATHING NODES

*********************************/
PathingNode::PathingNode( ) 

: GameObject(guid_counter, 1, "ehllo"), mHierarchyIsConnected(false), mConnectedHierarchyNodePTR(NULL), 
  mConnectedHierarchyIndex(-1), mGUID(guid_counter++), mConnectivityBits(0), mVisibilityBits(0),
  mFinalWeight(0.f), mGivenWeight(0.f), mTacticalWeight(0.f), mUserDefinedWeight(0.f), mTempFinal(0.f), mTempGiven(0.f)
{
	float radius = 50.f;
	LPDIRECT3DDEVICE9	device	= CGraphics::GetSingleton().device;
	D3DXCreateSphere( device, radius, 25, 25, &mRenderObject, NULL );

	D3DXCreateBox( device, 20.f, 20.f, 70.f, &mGizmoXaxis, NULL );
	D3DXCreateBox( device, 20.f, 20.f, 70.f, &mGizmoYaxis, NULL );
	D3DXCreateBox( device, 20.f, 20.f, 70.f, &mGizmoZaxis, NULL );

	CreateSphere( radius);
	CreateBody( D3DXVECTOR3(0,0,0) );
}

PathingNode::PathingNode( const D3DXVECTOR3& pPosition )

:	GameObject(guid_counter, 1, "ehllo"), mHierarchyIsConnected(false), mConnectedHierarchyNodePTR(NULL), 
  mConnectedHierarchyIndex(-1), mGUID(guid_counter++), mConnectivityBits(0), mVisibilityBits(0),
  mFinalWeight(0.f), mGivenWeight(0.f), mTacticalWeight(0.f), mUserDefinedWeight(0.f), mTempFinal(0.f), mTempGiven(0.f)
{
	float radius = 50.f;
	LPDIRECT3DDEVICE9	device	= CGraphics::GetSingleton().device;
	D3DXCreateSphere( device, radius, 25, 25, &mRenderObject, NULL );

	D3DXCreateBox( device, 20.f, 20.f, 70.f, &mGizmoXaxis, NULL );
	D3DXCreateBox( device, 20.f, 20.f, 70.f, &mGizmoYaxis, NULL );
	D3DXCreateBox( device, 20.f, 20.f, 70.f, &mGizmoZaxis, NULL );

	CreateSphere( radius);
	CreateBody( pPosition );
}

//Set the hierarchy node connected data.
void PathingNode::SetConnectedNodePTR( PathingNode* pConnectedNode )
{
	mConnectedHierarchyNodePTR = pConnectedNode;
}

void PathingNode::SetConnectedToHierarchy( const int&	pHierarchyIndex )
{
	mConnectedHierarchyIndex = pHierarchyIndex;
}

//Get the hierarchy node connected data.
bool PathingNode::IsConnectedToHierarchy( )
{
	return mHierarchyIsConnected;
}

PathingNode* PathingNode::GetConnectedNodePTR( )
{
	return mConnectedHierarchyNodePTR;
}

int PathingNode::GetConnectedHierarchyIndex( )
{
	return mConnectedHierarchyIndex;
}

//Set the connected index data to connected.
void	PathingNode::SetNodeConnected( const unsigned int& pIndex )
{
	//Here we need to do the bit shifting and setting of that bit.
	mConnectivityBits.set( pIndex );
}

//Set the connected index data to not connected.
void	PathingNode::SetNodeUnconnected( const unsigned int& pIndex )
{
	mConnectivityBits.reset(pIndex);
}

//Get the connected data for the given index of the node.
bool	PathingNode::IsConnectedToNode( const unsigned int&	pIndex )
{
	return mConnectivityBits.test( pIndex );
}

//Get the connected data for the given index of the node.
bool	PathingNode::IsVisibleToNode( const unsigned int&	pIndex )
{
	return mVisibilityBits.test( pIndex );
}

void	PathingNode::Render()
{
	//LPD3DXEFFECT effect = CGraphics::GetSingleton().effect;
	//Grab the effect if one exists.
	//D3DXMATRIX	world;
	//D3DXMatrixIdentity( &world, 0, -20, 0);

	if( this == CGraphics::GetSingleton().picker->GetPickedObject() )
	{

		//Set each gizmo axis to the transformed location it needs to be.

		
		D3DXMATRIX world, ident, rot, temp;
		D3DXMatrixIdentity( &ident );

		//The x gizmo will need to be translated 
		D3DXMatrixTranslation( &world, GetBodyPosition().x, GetBodyPosition().y, GetBodyPosition().z);

		D3DXMatrixRotationY( &rot, D3DX_PI / 2 );
		temp = rot * world;

		CGraphics::GetSingleton().device->SetTransform( D3DTS_WORLD, &temp );
		mGizmoXaxis->DrawSubset(0);
		CGraphics::GetSingleton().device->SetTransform( D3DTS_WORLD, &ident );

		D3DXMatrixRotationX( &rot, D3DX_PI / 2 );
		temp = rot * world;
		
		CGraphics::GetSingleton().device->SetTransform( D3DTS_WORLD, &temp );
		mGizmoYaxis->DrawSubset(0);
		CGraphics::GetSingleton().device->SetTransform( D3DTS_WORLD, &ident );

		D3DXMatrixRotationZ( &rot, D3DX_PI / 2 );
		temp = rot * world;
		
		CGraphics::GetSingleton().device->SetTransform( D3DTS_WORLD, &temp );
		mGizmoZaxis->DrawSubset(0);
		CGraphics::GetSingleton().device->SetTransform( D3DTS_WORLD, &ident );

		//mGizmoYaxis->DrawSubset(0);
		//mGizmoZaxis->DrawSubset(0);
	}
	else
	{
		mRenderObject->DrawSubset(0);
	}
}


void	PathingNode::ResetConnectivityBits()
{
	mConnectivityBits.reset();
}

void	PathingNode::SetVisibilityBit( unsigned bitToSet )
{
	mVisibilityBits.set( bitToSet );
}

void	PathingNode::ResetVisibilityBits( )
{
	mVisibilityBits.reset();
}

void	PathingNode::ResetVisibilityBit( unsigned bitToReset )
{
	mVisibilityBits.reset( bitToReset );
}






/*********************************

	PATHING HIERARCHY

*********************************/
PathingHierarchy::PathingHierarchy() 
: mConnectedNodeLines(MAX_BITS_PER_HIERARCHY), mVisibleNodeLines(MAX_BITS_PER_HIERARCHY), 
  mbRenderConnectionsList(true), mbRenderVisibilityList(true)
{
}

PathingHierarchy::~PathingHierarchy()
{
	unsigned size = (unsigned)this->GetNumNodes();
	for( unsigned i = 0; i < size; ++i )
	{
		delete mNodes[i];
	}
	//guid_counter = 0;
}

void PathingHierarchy::InitBuffers()
{
	if( mConnectedNodeLines.OneTimeCreateIndexBuffer( MAX_BITS_PER_HIERARCHY*MAX_BITS_PER_HIERARCHY ) &&
		mConnectedNodeLines.OneTimeCreateVertexBuffer( MAX_BITS_PER_HIERARCHY ) )
	{
		//All went well with the buffer creation.
	}

	if( mVisibleNodeLines.OneTimeCreateIndexBuffer( MAX_BITS_PER_HIERARCHY*MAX_BITS_PER_HIERARCHY ) &&
		mVisibleNodeLines.OneTimeCreateVertexBuffer( MAX_BITS_PER_HIERARCHY ) )
	{
		//All went well with the buffer creation.
	}
}

void PathingHierarchy::ClearAllNodeConnections()
{
	//Clear and reallocate the buffer.
	mConnectedNodeLines.ClearIndexBuffer();

	mConnectedNodeLines.OneTimeCreateIndexBuffer( MAX_BITS_PER_HIERARCHY*MAX_BITS_PER_HIERARCHY );

	//Now all the nodes in the hierarchy must clear their bitsets for connectivity.
	unsigned size = GetNumNodes();
	for( unsigned nodeIter = 0; nodeIter < size; ++nodeIter )
	{
		//Clear the bitsets.
		GetNode(nodeIter)->ResetConnectivityBits();
	}
}

void PathingHierarchy::ClearAllNodeVisibility()
{
	mVisibleNodeLines.ClearIndexBuffer();

	mVisibleNodeLines.OneTimeCreateIndexBuffer( MAX_BITS_PER_HIERARCHY*MAX_BITS_PER_HIERARCHY );

	//Now all the nodes in the hierarchy must clear their bitsets for connectivity.
	unsigned size = GetNumNodes();
	for( unsigned nodeIter = 0; nodeIter < size; ++nodeIter )
	{
		//Clear the bitsets.
		GetNode(nodeIter)->ResetVisibilityBits();
	}
}


void PathingHierarchy::Render()		
{ 
	if( mbRenderConnectionsList )
	{
		mConnectedNodeLines.Render(); 
	}

	if( mbRenderVisibilityList )
	{
		mVisibleNodeLines.Render();
	}
}

	
void	PathingHierarchy::UpdateRenderPath( int x, int y, CPicker*	pPicker)
{
	// 
	D3DXVECTOR3 point = pPicker->PickAndCreateWaypoint( x, y );

	//Use this point to create the new waypoint node.
	if( GetNumNodes() < MAX_BITS_PER_HIERARCHY )
	{
		point.y += 50.f;

		AddNode( new PathingNode( point ) );

		mConnectedNodeLines.updateDataInBuffer( GetNumNodes()-1, point, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
		mVisibleNodeLines.updateDataInBuffer( GetNumNodes()-1, point, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) );		
	}

	///When a node is created, it should also loop through the other points and 
	// do a simple ray check to the center to see if there's a line-of-sight.
}


void	PathingHierarchy::UpdateLines( PathingNode*	node )
{
	//This should go through all the connection indices, adding two at a time to the index buffer.
	//  This should grow the index buffer for sure....

	unsigned size = mConnectedNodeLines.GetNumLinesSet();
	size *= 2;

	//Now update the index buffer itself
	unsigned connectionSize = (unsigned)node->NumBits();
	for( unsigned connectIter = 0; connectIter < connectionSize; ++connectIter )
	{
		if( connectIter == (unsigned)node->GetGUID() ) { continue; }

		//If the current connection bit is set, add it.
		if( node->IsConnectedToNode( connectIter ) )
		{
			mConnectedNodeLines.AddLineToRenderList( node->GetGUID(), connectIter, size );
			size += 2;
		}
	}
}


void	PathingHierarchy::UpdateNodeInBuffer( PathingNode*	node )
{
	//Grab the render path, shove it the id and position data.

	mConnectedNodeLines.updateDataInBuffer( node->GetID(), node->GetPosition(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
	mVisibleNodeLines.updateDataInBuffer( node->GetID(), node->GetPosition(), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) );
}



void	PathingHierarchy::GenerateVisibilityLines( bool connect )
{
	//Looping through each node, check the rest of the list 
	unsigned size = GetNumNodes();
	unsigned index = 0;
	unsigned connectIndex = 0;
	for( unsigned nodeIter = 0; nodeIter < size; ++nodeIter )
	{
		//GetNode(nodeIter)->ResetVisibilityBits();

		//Now starting with the next index on, check visibility.
		for( unsigned visIter = 0; visIter < size; ++visIter )
		{
			bool notVisible = false;

			//Check to make sure it's not the same node we're considering here.
			if( nodeIter == visIter )	
			{ 

			}
			else
			{
				//Get all of the picker and ray data necessary to determine these intersections.
				CPicker* picker = CGraphics::GetSingleton().picker;
				D3DXVECTOR3	iterNodepos = GetNode(nodeIter)->GetPosition();
				D3DXVECTOR3	visNodepos  = GetNode(visIter)->GetPosition();
				D3DXVECTOR3 pDir        = visNodepos - iterNodepos;
				D3DXVec3Normalize( &pDir, &pDir );

				std::pair<D3DXVECTOR3, D3DXVECTOR3>	pBoxCollisionPoints;


				//Since we know it's not the same node, we can check all the environment to make
				// sure it doesn't collide with anything.  If it does, we won't include the node 
				// as being visible.  If not, we do a simple distance check to also make sure they're
				// within visibility range, and then if these two pass, it gets added.
				unsigned NumberOfObjects = (unsigned)CGraphics::GetSingleton().objsList.size();
				for( unsigned objIter = 0; objIter < NumberOfObjects; ++objIter )
				{
					//Do the check.  If any return true, break out of this for loop.
					if( CGraphics::GetSingleton().objsList[objIter]->GetBox() )
					{		
						notVisible = picker->DetectRayBox( iterNodepos, pDir, CGraphics::GetSingleton().objsList[objIter]->GetBox(), pBoxCollisionPoints );	
						if( notVisible )
						{
							break;
						}
					}
					D3DXVECTOR3	spherePoint;
					if( CGraphics::GetSingleton().objsList[objIter]->GetSphere() )
					{		
						notVisible = picker->DetectRaySphere( iterNodepos, pDir, CGraphics::GetSingleton().objsList[objIter]->GetSphere(), spherePoint );	
						if( notVisible )
						{
							break;
						}
					}
				}
			}
				

			//This means that there was an object in the way, so break out and continue.
			if( notVisible )	{ continue; }

			//Take the first node, and use the picker's ray-sphere detection to see
			//  if there's a line of sight.

			//if( picker->DetectRaySphere( iterNodepos, pDir, GetNode(visIter)->GetSphere(), D3DXVECTOR3() ) )
			//{
			if( connect )
			{
				GetNode(nodeIter)->SetNodeConnected( visIter );
				
				//Add this line to the visibility lines being drawn.
				mConnectedNodeLines.AddLineToRenderList( nodeIter, visIter, connectIndex );
				connectIndex += 2;
			}
			//PathingNode*	node = GetNode(nodeIter);

				//This means a line of sight is established, so set this bit.
			GetNode(nodeIter)->SetVisibilityBit( visIter );

			//Add this line to the visibility lines being drawn.
			mVisibleNodeLines.AddLineToRenderList( nodeIter, visIter, index );
			index += 2;
			
			//}
		}
	}

	for( unsigned nodeIter = 0; nodeIter < size; ++nodeIter )
	{			
		//For this node all visibility and connectivity has been generated, so now use these values to set
		//	the visible and connection weights for the node.
		PathingNode*	node = GetNode(nodeIter);
		unsigned numVisibleNodes = node->NumVisibleBits();
		unsigned numConnectNodes = node->NumConnectivityBits();

		//If the number of connected nodes is 1, then this is a dead end.
		if( numConnectNodes == 2 )
		{
			node->SetDeadEnd( true );
		}

		//If the number of connected nodes is 2, then it's a pinch point.
		else if( numConnectNodes == 3 )
		{
			node->SetPinchPoint( true );
		}

		float numCurNodesInHierarchy = (float)PathingArea::GetSingleton().GetCurrentHierarchy()->GetNumNodes();

		node->SetVisibleWeight( 1.1f - ((float)numVisibleNodes / (float)numCurNodesInHierarchy) );
		node->SetConnectionsWeight( 1.1f - ((float)numConnectNodes / (float)numCurNodesInHierarchy) );
		node->SetTacticalWeight( BASE_TACTICAL_VALUE * node->GetVisibleWeight() * node->GetConnectionsWeight() );

		//PathingArea::GetSingleton().GetCurrentHierarchy()->DetermineLOSNodesFromObject( 
	}
}


	//This will take in a gameobject * and return the index of the node that is closest to this object.
int			PathingHierarchy::NearestNodeToObject( GameObject*	pObj )
{
	float prevDist = FLT_MAX, curDist = FLT_MAX;
	int index = -1;

	//Go through all of the nodes in the scene, and keeping track of the prev distance, 
	//  find shortest one by comparing and seeing if current is less than previous dist.
	unsigned size = GetNumNodes();
	for( unsigned nodeIter = 0; nodeIter < size; ++nodeIter )
	{
		D3DXVECTOR3	tempVec = GetNode(nodeIter)->GetPosition() - pObj->GetPosition();
		curDist = D3DXVec3Dot( &tempVec, &tempVec );

		//Here I'm simply checking the distance squared, since there really is no need
		//	to have the sqrt for these checks, since they're all relative.
		if( curDist < prevDist )
		{
			prevDist = curDist;
			index = nodeIter;
		}
	}

	//Return the index that last had a smaller current distance.
	return index;
}


//This function will get an entire bit set together of all the "dangerous" or Line of Sight (LOS) nodes to
//	a particular game object specified.
void	PathingHierarchy::DetermineLOSNodesFromObject( GameObject* pObj, std::bitset<MAX_BITS_PER_HIERARCHY>&	rLOSbits )
{
	//First determine the closest node to this gameobject.
	PathingNode*	node = GetNode( NearestNodeToObject( pObj ) );

	//Once we have that node, we check all of the nodes that are visible to it and set them true in the LOS bits.
	unsigned size = node->NumBits();
	for( unsigned bitIter = 0; bitIter < size; ++bitIter )
	{
		//If this bit is set in the node, set it in the LOS bits too.
		if( node->IsVisibleToNode( bitIter ) )
		{
			rLOSbits.set( bitIter );
		}

		//if( node->IsConnectedToNode( bitIter ) )
		//{
		//	rLOSbits.set( bitIter );
		//}
	}
}

void	PathingHierarchy::DetermineSafeNodesFromLOSBits( bitset<MAX_BITS_PER_HIERARCHY>&	rSafeBits, const bitset<MAX_BITS_PER_HIERARCHY>&	rLOSbits )
{
	//Invert the rLOSbits.

	bitset<64> tempSet;
	tempSet.set();

	rSafeBits = rLOSbits ^ tempSet;

	tempSet = tempSet << GetNumNodes();

	rSafeBits = rSafeBits ^ tempSet;

	//tempSet.reset();
}

void	PathingHierarchy::ClearAndRegenerateBitsets()
{
	//Here, I want to set the picker's mode to be that of selecting objects.
	//All this needs to do is clear the list.  It should click, but not be toggled on or anything.
	//  one time shot at clearing the index buffer.
	ClearAllNodeVisibility();
	ClearAllNodeConnections();

	//Go through all of the objects that have movement components and clear all of their A* data too.
	unsigned size = (unsigned) CGraphics::GetSingleton().objsList.size();
	for( unsigned objIter = 0; objIter < size; ++objIter )
	{
		//clear anything with a movement component.
		if( CGraphics::GetSingleton().objsList[objIter]->GetMovement() )
		{
			CGraphics::GetSingleton().objsList[objIter]->GetMovement()->ClearAStarValues();
		}
	}
	GenerateVisibilityLines( true );
}


unsigned	PathingNode::NumBits()		
{ 
	return MAX_BITS_PER_HIERARCHY;
}

unsigned	PathingNode::NumConnectivityBits()		
{ 
	return (unsigned)mConnectivityBits.count();
}

unsigned	PathingNode::NumVisibleBits()
{ 
	return (unsigned)mVisibilityBits.count();
}


