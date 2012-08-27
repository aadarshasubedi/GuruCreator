
/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	PathingNode.h
brief:	

*/

#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <vector>
#include <d3dx9math.h>
#include "PathingNode.h"


class GameObject;
class PathingNode;
class RenderableHierarchy;

/*
A functor for the sorting of the priority queue.
*/
//template < class Type >
class NodeWeightGreater
{
public:
	bool operator() ( PathingNode* first, PathingNode* second ) const
	{
		return ( first->Weight() > second->Weight() );
	}
};


/*
A priority queue class, internally handled by a heap, which will
	be used for the open and closed lists by having internal sorting
	done.  
*/
class CPriorityQueue
{
	//The priority queue will simply be a heap.
public:
	typedef std::vector<PathingNode*>	HEAP;

	HEAP			m_heap;
};

/*
A bunch of functions for the priority queue:

IsEmpty
FrontOfQueue
PopQueue
Pushueue
IsOnQueueLower

*/

/*
<Start Algorithm>
Push start node with null parent on open list

while openlist isn't empty
	Pop the best node, call it nodeA
	Find best neighbor of nodeA and call it nodeB

	If nodeB is goal
		Push onto closed list
		break
	
	If nodeB is on open or closed list with lower value
		do nothing, continue

	Push nodeB onto closedlist
	If too much time, stop for now and come back later....
endwhile

Go through closed list and link up the path
<End Algorithm>
*/
//
/*
The actual functions involved in the A* algorithm:

A function to initialze all of the state variables and to
 initialize start node.
InitStateVariables

*/

bool IsQueueEmpty( const CPriorityQueue& queue );
PathingNode* FrontOfQueue( const CPriorityQueue& queue);
//void FrontOfQueue( const CPriorityQueue& queue, PathingNode* node );
PathingNode* PopQueue( CPriorityQueue& queue );
void PushQueue( CPriorityQueue& queue, PathingNode* node );
void RemoveFromQueue( CPriorityQueue& queue, PathingNode* node );
bool IsNodeOnQueueLower( CPriorityQueue& queue, PathingNode* node );
bool IsNodeOnQueue( CPriorityQueue& queue, PathingNode* node );


bool AreNodesSame( PathingNode* first, PathingNode* second );
//void UpdateNode( const PathingNode& updater, PathingNode& updateMe );

float DistanceBetweenNodesEuclidean( PathingNode* first, PathingNode* second );
float DistanceBetweenNodesCardinal( PathingNode* first, PathingNode* second );


enum SearchMode
{
	SEARCH_INIT,
	SEARCH_SUCCESS,
	SEARCH_FAILED,
	SEARCH_PROCESSING
};


enum MovementMode
{
	MOVEMENT_NULL,
	MOVEMENT_SEEK_TARGET,
	MOVEMENT_WAYPOINT_LIST
};


class Movement
{
	typedef std::vector<PathingNode*>	WaypointList;
	typedef std::vector<D3DXVECTOR3>	PathPointList;

public:
	//The owner has a body to interact with, otherwise it couldn't move anything.
	Movement( GameObject* owner, int pMovementType );

	//Getters
	D3DXVECTOR3		GetCurrentPosition();

	//Accessor for the movement type.
	int				GetMovementType();

	//Setter for the movement type.
	void			SetMovementType( int pType );

	///A* FUNCTIONS///

	//Initializes all state variables and gets the goal node from the picker.  If
	//  picker has none, then no A* will be calculated as no goal or start has been
	//	selected.
	void	Init();

	//Helper function to compare node pointers and see if it matches the goal node stored.
	bool	IsNodeGoal( PathingNode* testingNode );	
	bool	IsNodeStart( PathingNode* testingNode );

	//Checks to see if this node has a parent.
	bool	HasParentNode( PathingNode* node );

	//Goes through the closed list and finds this node pointer as it's the parent of
	// the current node we're looking at.
	void	 FindParentNode( PathingNode* parentNode, PathingNode* node );

	//For during A*, this goes through all of the nodes connected bits and finds all of those
	//  it is connected with.  These will be put onto the open list to be checked later if 
	//	necessary as the algorithm dictates.
	void	 FindNeighborsOfNode( PathingNode* curNode );
	
	//Helpers to use and query the open and closed lists.
	void PushOpenListQueue( PathingNode* node );
	void PushClosedListQueue( PathingNode* node );
	void PopOpenListQueue( PathingNode* node );
	void PopClosedListQueue( PathingNode* node );

	//This is the actual "A* algorithm" function.  Short and sweet, it goes
	//	through the nodes, finds neighbors, does heuristic stuff, and checks
	//	next best node.
	void SearchForPath();

	//This sets the goal node, calculates the first heuristic values for the start node,
	//	clears the waypoints, and begins the search processing to start.  Pushes start
	//	node onto the open list.  The start node has a parent pointer set NULL.
	void InitializeAStarState();

	//Big work horse of the A* algorithm.  This is what will calculate heuristic values and check if it's on the
	//	open or closed list with lower values.
	void AddNodeToOpenList( PathingNode* parentNode, PathingNode* checkingThisNode );

	//Waypoint list generation function.
	void GenerateWaypointsFromClosedList( );

	bool	AStarComputed()			{ if( mSearchMode == SEARCH_SUCCESS ) { return true; } return false; }
	
	void RubberBandWaypoints( );
	void SmoothCurrentWaypoints( std::vector<D3DXVECTOR3>&	endPath );

	D3DXVECTOR3 CatmullRom( D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, float u );

	void	LinearMovement();


	//This function will be called during the update phase, 
	void	Update();

	void	Render();

	bool	ShouldRenderAStar()		{ return mbRenderAStarPath; }

	void	IncrementPositionCounter()	{ ++mCounter; mCounter = mCounter % mPathpointList.size(); }

	void	ClearAStarValues();

	//@TODO:  Give this guy a RenderableHierarchy, and at the end of it
	//			it can be updated and rendered if wanted.
	////This function will set colors of all lists.
	//void ColorAStarPath();

	////This function will color the open list.
	//void ColorOpenList();
	////This function will color the closed list.
	//void ColorClosedList();
	////Color the goal with this function.
	//void ColorGoal();
	void SetGoalNode(PathingNode* node) { if (node) { mGoalNode = node; } }
	PathingNode* GetGoalNode(void) { return mGoalNode; }
	PathingNode* GetStartNode(void) { return mStartNode; }


	GameObject*		Owner()		{ return mOwner; }


private:
	//Smooth out the path or not.
	bool				mSmooth;
	//Cut corners and such if possible.
	bool				mRubberband;
	//Watch A* work or generate path instantly.
	bool				mSingleStep;
	//The intermediate heuristic weight generated during the a* algorithm.
	float				mHeuristicWeight;
	//The type of heuristic calculation used to generate above value.
	int					mHeuristicCalc;

	//The way this entity will move around.  Could be user controlled, or
	//	AI controlled, with varying levels of intelligence.
	int					mMovementType;
	int					mSearchMode;

	//The two lists to be used during the path finding.
	CPriorityQueue		mOpenlist;
	CPriorityQueue		mClosedlist;

	//A pointer to the goal node, easy to see if we're there.
	PathingNode*		mGoalNode;
	PathingNode*		mStartNode;

	//The final list of
	WaypointList		mWaypointList;
	PathPointList		mPathpointList;

	//The Rendered path for the completed A*
	RenderableHierarchy*	mpAStarPathRenderObj;

	//A boolean whether to display the renderable path.
	bool					mbRenderAStarPath;

	//These are values that set certain pre-determined speeds.
	float				mSpeedWalk;
	float				mSpeedJog;

	//This is the speed that is used for how fast they move once a path
	// has been generated.
	float				mSpeedCurrent;
	float				mCurIndex;

	unsigned			mCounter;

	//The game object who owns this component.
	GameObject*			mOwner;

};



#endif

