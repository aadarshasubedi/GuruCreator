
/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	PathingNode.h
brief:	

*/
#ifndef PATHINGNODE_H
#define PATHINGNODE_H

#include <d3dx9math.h>
#include <d3dx9mesh.h>
#include <bitset>
#include <vector>
#include "Utilities.h"
#include "Singleton.h"
#include "GameObject.h"
#include "CPicker.h"
#include "PathingClasses.h"
using std::bitset;

class PathingArea;
class PathingNode;
class PathingHierarchy;

typedef std::vector<GameObject*>	NODEPTR_LIST;
static int guid_counter = 0;

class PathingArea : public Singleton<PathingArea>
{
	//This class will be similar to a world class that defines the walkable area for the
	//	AI bots and has helper functions to find walls, test for lines of sight and other things.
	//  
	//For this I think we'll use a waypoint list 
	//
	//

public:
	PathingArea();
	virtual ~PathingArea();

	//Initializer
	void	Init();

	//Creates a new hierarchy.

	//Gets a specified hierarchy.
	PathingHierarchy*		GetCurrentHierarchy( unsigned index)		{ return mHierarchies[index]; }

	//Gets the current hierarchy.
	PathingHierarchy*		GetCurrentHierarchy()		{ return mHierarchies[mCurrentHierarchy]; }

	//Adds a node to the current hierarchy.
	void		AddNodeToCurrentHierarchy( PathingNode*		ptrNode );

	//Adds a node to the specified hierarchy.

	//Gets a node by its GUID....  exhaustive search through all hierarchies.
	PathingNode*		GetNodeByGUID( int	pGUID );

private:
	//List of hierarchies.
	std::vector<PathingHierarchy*>		mHierarchies;

	int		mCurrentHierarchy;

	int		mCurrentGUID;
};



class PathingNode : public GameObject
{
public:
	PathingNode( );
	PathingNode( const D3DXVECTOR3& pPosition );	

	///
	///
	void SetBodyPosition( const D3DXVECTOR3& pPosition ) { SetPosition(pPosition); }
	D3DXVECTOR3	GetBodyPosition()	{ return this->GetPosition(); }

	//Set the hierarchy node connected data.
	void SetConnectedNodePTR( PathingNode* pConnectedNode );
	void SetConnectedToHierarchy( const int&	pHierarchyIndex );

	//Get the hierarchy node connected data.
	bool IsConnectedToHierarchy( );
	PathingNode* GetConnectedNodePTR( );
	int GetConnectedHierarchyIndex( );

	//Set the connected index data to connected.
	void	SetNodeConnected( const unsigned& pIndex );
	
	//Set the connected index data to not connected.
	void	SetNodeUnconnected( const unsigned& pIndex );

	//Get the connected data for the given index of the node.
	bool	IsConnectedToNode( const unsigned&	pIndex );

	bool	IsVisibleToNode( const unsigned int&	pIndex );

	//Get GUID
	int		GetGUID() { return mGUID; }

	//Render the mesh that represents the node.
	virtual void	Render();

	unsigned		NumConnectivityBits();
	unsigned		NumVisibleBits();
	unsigned		NumBits();

	std::bitset<MAX_BITS_PER_HIERARCHY>*	ConnectedBitsPtr()			{ return & mConnectivityBits; }

	void	ResetConnectivityBits();

	void	SetVisibilityBit( unsigned bitToSet );
	void	ResetVisibilityBits( );
	void	ResetVisibilityBit( unsigned bitToReset );

	LPD3DXMESH	GetRenderObject()	{ return mRenderObject; }


	//Here start the A star specific
	PathingNode*	HasParentNode()						{ return mParentNode; }
	void			SetPreviousNode( PathingNode* node )	{ mParentNode = node; }


	//These functions are the getters and setters for all of the various weights the node
	//	takes into account.

	//Given weight, which is essentially the euclidean distance up to this node.
	float	GetGivenWeight()						{ return mGivenWeight; }
	void	SetGivenWeight( float pWeight )			{ mGivenWeight = pWeight; }
	
	//Final weight, which is the weight taking in to account its distance from the goal.
	float	GetFinalWeight()						{ return mFinalWeight; }
	float	Weight()								{ return mFinalWeight; }
	void	SetFinalWeight( float pWeight )			{ mFinalWeight = pWeight; }
	
	//Given weight, which is essentially the euclidean distance up to this node.
	float	GetVisibleWeight()						{ return mVisibleWeight; }
	void	SetVisibleWeight( float pWeight )		{ mVisibleWeight = pWeight; }
	
	//Final weight, which is the weight taking in to account its distance from the goal.
	float	GetConnectionsWeight()					{ return mConnectionsWeight; }
	void	SetConnectionsWeight( float pWeight )	{ mConnectionsWeight = pWeight; }
	
	//Tactical weight, which is a value summed from many other weights and values.
	//	Takes into account current entit
	float	GetTacticalWeight()	{ return mTacticalWeight; }
	void	SetTacticalWeight( float pWeight )	{ mTacticalWeight = pWeight; }

	float	GetUserDefinedWeight()	{ return mUserDefinedWeight; }
	void	SetUserWeight( float pWeight )	{ mUserDefinedWeight = pWeight; }

	float	GetTempFinal()		{ return mTempFinal; }
	void	SetTempFinal( float weight ) { mTempFinal = weight; }

	float	GetTempGiven()		{ return mTempGiven; }
	void	SetTempGiven( float weight ) { mTempGiven = weight; }

	bool	GetOnOpenList()		{ return mOnOpen; }
	void	SetOnOpenList( bool mOnList )	{ mOnOpen = mOnList; }

	bool	GetOnClosedList()		{ return mOnClosed; }
	void	SetOnClosedList( bool mOnList )	{ mOnClosed = mOnList; }

	//Getters and setters for the tactical booleans
	bool	IsDeadEnd()				{ return mIsDeadEnd; }
	void	SetDeadEnd( bool end )	{ mIsDeadEnd = end; }

	bool	IsPinchPoint()			{ return mIsPinchPoint; }
	void	SetPinchPoint(bool pt)	{ mIsPinchPoint = pt; }

private:
	//The directx vector3 representing the position of the node
	//D3DXVECTOR3		mPosition;

	//Needs to know if this is a connector to another hierarchy of nodes.
	bool			mHierarchyIsConnected;
	//

	//Pointer to the PathingNode in the other hierarchy that it's connected to.  Null
	//  if not connected.
	PathingNode*	mConnectedHierarchyNodePTR;

	//The int index value of the hierarchy this node is connected to in the 
	//  pathing areas list of waypointlists.  -1 if not connected to another list in the
	//	hierarchy.
	int				mConnectedHierarchyIndex;
	
	//Unique identifier int 1 - 32.
	int				mGUID;

	//Need 32 bits for the connectivity data of the node to the others.
	bitset<MAX_BITS_PER_HIERARCHY>		mConnectivityBits;

	//Need 32 bits for the visibility data of the node to the others.
	bitset<MAX_BITS_PER_HIERARCHY>		mVisibilityBits;

	//This mesh will be the visual representation of the node, and will be a sphere
	//  so picking them will be easy.
	LPD3DXMESH							mRenderObject;

	//Several render meshes so that i can render a gizmo that's easily interactable
	//		with the picker.
	LPD3DXMESH							mGizmoXaxis;
	LPD3DXMESH							mGizmoYaxis;
	LPD3DXMESH							mGizmoZaxis;

	PathingNode*	mParentNode;

	//Euclidean distance up to this node along path.
	float mGivenWeight;

	//Final weight taking into account the distance to the goal node.
	float mFinalWeight;

	//Visibility weight, modifies the tactical weight based on how many nodes can see it.
	float mVisibleWeight;

	//Connectivity weight, modifies the tactical weight based how many nodes it's connected with.
	float mConnectionsWeight;

	//This will be for the tactical defined weight of an object.
	float mTacticalWeight;

	//This will be a user defined weight.  This can be for anything, but
	//  allows even more weight control.
	float mUserDefinedWeight;

	//These values are used to hold temporary weights in case the node is already on the closed list
	//	with lower values.  We don't want to update and then check, so these are placeholders until
	//	we found out they are the new values.
	float mTempGiven;
	float mTempFinal;

	//Easy way to let us know if it's on the open or closed list.
	bool mOnOpen, mOnClosed;

	//These are for the tactical knowledge of a path node.

	//This easily let's us know if a node is a dead end.
	bool	mIsDeadEnd;

	//Let's us know if this node is a pinch point, as in this point can be surrounded.
	bool	mIsPinchPoint;

	//Let's us know if this node is on an isolated end of a pinch point, like inside of a room
	//	or hallway.
	bool	mIsIsolatedPoint;

};

class PathingHierarchy
{
	//Contains a list of all the PathingNode lists in the area or scene.

	//

public:
	PathingHierarchy();
	~PathingHierarchy();

	//Adds a node to the list.
	void	AddNode( PathingNode*	ptrNode )	{ mNodes.push_back( ptrNode ); }

	unsigned GetNumNodes()	{ return (unsigned)mNodes.size(); }

	PathingNode*	GetNode( unsigned index )	{ return (PathingNode*)mNodes[index]; }

	void			UpdateRenderPath( int x, int y, CPicker*	pPicker );
	void			UpdateLines( PathingNode*	node );

	void			Render();

	RenderableHierarchy*		ConnectedListPtr()		{ return &mConnectedNodeLines; }
	RenderableHierarchy*		VisibleListPtr()		{ return &mVisibleNodeLines; }

	NODEPTR_LIST				GetAllNodes()	{ return mNodes; }

	void			UpdateNodeInBuffer( PathingNode*	node );

	void			InitBuffers();

	void			ClearAllNodeConnections();

	void			ClearAllNodeVisibility();

	void			GenerateVisibilityLines(bool connect = false);


	//Toggles for the rendering of the line lists.
	void			ToggleRenderConnections() { mbRenderConnectionsList = !mbRenderConnectionsList; }
	void			ToggleRenderVisibility() { mbRenderVisibilityList = !mbRenderVisibilityList; }
	void			ToggleRenderConnections(bool isVis) { mbRenderConnectionsList = isVis; }
	void			ToggleRenderVisibility(bool isVis) { mbRenderVisibilityList = isVis; }

	void			CreateAndSetCurvePath();
	RenderableHierarchy*	GetCurve()		{ return &mCurveLines; }

	D3DXVECTOR3		CurvePoint( unsigned index )	{ return mCurveLines.GetSingleCurvePoint(index); }

	bool			CurveInitialized()		{ return mCurveLines.IsCurveInitialized(); }

	//This will take in a gameobject * and return the index of the node that is closest to this object.
	int				NearestNodeToObject( GameObject*	pObj );

	//This function will get an entire bit set together of all the "dangerous" or Line of Sight (LOS) nodes to
	//	a particular game object specified.
	void			DetermineLOSNodesFromObject( GameObject* pObj, std::bitset<MAX_BITS_PER_HIERARCHY>&	rLOSbits );
	void			DetermineSafeNodesFromLOSBits( bitset<MAX_BITS_PER_HIERARCHY>&	rSafeBits, const bitset<MAX_BITS_PER_HIERARCHY>&	rLOSbits );

	void			ClearAndRegenerateBitsets();

private:
	//list of the nodes in this hierarchy and the connectivity lines to them.
	RenderableHierarchy		mConnectedNodeLines;
	RenderableHierarchy		mVisibleNodeLines;

	//This will only be used if this renderable hierarchy represents a walkable path, say for AI or a camera.
	RenderableHierarchy		mCurveLines;

	//booleans for toggling between rendering the different lists.
	bool					mbRenderConnectionsList;
	bool					mbRenderVisibilityList;

	//Here's all the nodes in the hierarchy.
	NODEPTR_LIST			mNodes;

};


#endif