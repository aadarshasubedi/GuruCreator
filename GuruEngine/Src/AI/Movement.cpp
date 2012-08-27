
#include "GameObject.h"
#include "Movement.h"
#include "Sphere.h"
#include "Body.h"
#include "PathingNode.h"
//#include "CPriorityQueue.h"
#include "Graphics.h"
#include "Utilities.h"
#include <algorithm>
#include <math.h>


///A* FUNCTIONS///

bool IsQueueEmpty( const CPriorityQueue& queue )
{
	return queue.m_heap.empty();
}
//
//void FrontOfQueue( const CPriorityQueue& queue, PathingNode* node )
//{
//	node = queue.m_heap.front();
//}

PathingNode* FrontOfQueue( const CPriorityQueue& queue )
{
	return queue.m_heap.front();
}

PathingNode* PopQueue( CPriorityQueue& queue )
{
	//Set the node to the front node.
	PathingNode* temp = FrontOfQueue(queue);

	//This function takes the first element and puts it at the end, then resorts
	// the rest of it up to the end.
	std::pop_heap( queue.m_heap.begin(), queue.m_heap.end(), NodeWeightGreater() );

	//Now actually pop the back element off since this is now the one
	// node was set to.
	queue.m_heap.pop_back();

	return temp;
}

void PushQueue( CPriorityQueue& queue, PathingNode* node )
{
	//Push the element onto the back of the queue.
	queue.m_heap.push_back( node );

	//Now resort the whole thing as it's likely out of order now.
	std::push_heap( queue.m_heap.begin(), queue.m_heap.end(), NodeWeightGreater() );

	//std::sort_heap( queue.m_heap.begin(), queue.m_heap.end(), NodeWeightGreater() );

}

void RemoveFromQueue( CPriorityQueue& queue, PathingNode* node )
{
	//If the list has nothing, don't bother removing it.
	if( 0 == queue.m_heap.size() )
	{
		return;
	}

	CPriorityQueue::HEAP::iterator iter, holder, end = queue.m_heap.end();

	//This priority queue will be used as a temp.
	CPriorityQueue temp;

	for( iter = queue.m_heap.begin(); iter != end; ++iter )
	{
		if( AreNodesSame( (*iter), node ) )
		{
			//Set the weight to negative, since we're not using negative weights.
			(*iter)->SetFinalWeight(-1.0f);
			std::push_heap( queue.m_heap.begin(), iter + 1, NodeWeightGreater() );
			PopQueue( queue );
			break;
		}
	}
}

bool IsNodeOnQueueLower( CPriorityQueue& queue, PathingNode* node )
{
	//A quick and easy out, if the queue's heap doesn't have any elements on it, just
	// return false.
	if( 0 == queue.m_heap.size() )
	{
		return false;
	}

	//Check to see if the node is on the queue.  If it isn't, return false.
	CPriorityQueue::HEAP::iterator iter, end = queue.m_heap.end();

	for( iter = queue.m_heap.begin(); iter != end; ++iter )
	{
		if( !AreNodesSame( (*iter), node ) )
		{	
			continue;
		}
		//If I ever get here, both cases return, so it won't search more of the queue
		// after this point.
		else
		{
			//The nodes are the same, so check to see if the incoming weight is less.
			if( (*iter)->GetTempFinal() > node->GetTempFinal() )
			{
				//This function will update the values of the node in (*begin).
				//UpdateNode( node, (*iter) );
				node->SetFinalWeight( node->GetTempFinal() );
				node->SetGivenWeight( node->GetTempGiven() );

				//Now, I need to update sort this sucker.
				//std::sort_heap( queue.m_heap.begin(), iter + 1, NodeWeightGreater() );

				//And I can return here because, well, nodes already found and updated.
				//False here means that it was not on the list lower, so I needed
				return false;
			}
			else
			{
				//The weight was less, so return true.
				return true;
			}
		}
	}

	//This just means it wasn't on the list.
	return false;
}

bool IsNodeOnQueue( CPriorityQueue& queue, PathingNode* node )
{
	CPriorityQueue::HEAP::iterator iter, end = queue.m_heap.end();

	for( iter = queue.m_heap.begin(); iter != end; ++iter )
	{
		if( AreNodesSame( (*iter), node ) )
		{	
			return true;
		}
	}

	return false;
}


bool AreNodesSame( PathingNode* first, PathingNode* second )
{
	return ( first == second );
}

//
//void UpdateNode( const PathingNode& updater, PathingNode& updateMe )
//{
//}

float DistanceBetweenNodesEuclidean( PathingNode* first, PathingNode* second )
{
	D3DXVECTOR3 vec = D3DXVECTOR3( first->GetPosition() - second->GetPosition() );
	return D3DXVec3Length( &vec );
}

float DistanceBetweenNodesCardinal( PathingNode* first, PathingNode* second )
{
	float dx = abs(first->GetPosition().x - second->GetPosition().x);
	float dz = abs(first->GetPosition().z - second->GetPosition().z); 

	float minimum = min( dx, dz );
	float maximum = max( dx, dz );

	return (sqrt(2.0f) * (float)minimum) + (maximum - minimum);
}
	

Movement::Movement( GameObject* owner, int pMovementType ) : 
mSmooth(false),
mRubberband(false),
mSingleStep(false),
mHeuristicWeight(1.0f),
mHeuristicCalc(0),
mMovementType(pMovementType),
mSearchMode(-1),
mOpenlist(),
mClosedlist(),
mGoalNode(NULL),
mStartNode(NULL),
mWaypointList(0),
mpAStarPathRenderObj(NULL),
mbRenderAStarPath(false),
mSpeedWalk(2.f),
mSpeedJog(10.f),
mCurIndex(0.f),
mSpeedCurrent(mSpeedJog),
mCounter(0),
mOwner(owner)
{
	Init();
}

D3DXVECTOR3		Movement::GetCurrentPosition()			
{ 
	return mOwner->GetPosition();
}

//Accessor for the movement type.
int		Movement::GetMovementType()
{ 
	return mMovementType;
}

//Setter for the movement type.
void	Movement::SetMovementType( int pType )
{ 
	mMovementType = pType;
}

//Initializes all state variables and gets the goal node from the picker.  If
//  picker has none, then no A* will be calculated as no goal or start has been
//	selected.
void	Movement::Init()
{
	mSearchMode = SEARCH_INIT;
	mGoalNode = NULL;
}

//Helper function to compare node pointers and see if it matches the goal node stored.
bool	Movement::IsNodeGoal( PathingNode* testingNode )
{
	return AreNodesSame( testingNode, mGoalNode );
}
	

bool	Movement::IsNodeStart( PathingNode* testingNode )
{
	return AreNodesSame( testingNode, mStartNode);
}


//Checks to see if this node has a parent.
bool	Movement::HasParentNode( PathingNode* node )
{
	if( node->HasParentNode() )
	{
		return true;
	}
	return false;
}

void	Movement::PushOpenListQueue( PathingNode* node )		
{ 
	node->SetOnOpenList(true); PushQueue( mOpenlist, node );
}

void	Movement::PushClosedListQueue( PathingNode* node )
{ 
	node->SetOnClosedList(true); PushQueue( mClosedlist, node );
}	

void	Movement::PopOpenListQueue( PathingNode* node )
{ 
	node->SetOnOpenList(false); node = PopQueue( mOpenlist );
}

void	Movement::PopClosedListQueue( PathingNode* node )
{ 
	node->SetOnClosedList(false); node = PopQueue( mClosedlist );
}




//This sets the goal node, calculates the first heuristic values for the start node,
//	clears the waypoints, and begins the search processing to start.  Pushes start
//	node onto the open list.  The start node has a parent pointer set NULL.
void	Movement::InitializeAStarState()
{
	//Clear the way point list since the A* is beginning fresh now.
	mPathpointList.clear();
	mWaypointList.clear();

	mCounter = 0;
	mCurIndex = 0.f;
	//mStartNode = NULL;
	//mGoalNode = NULL;


	////Get from the picker the start and goal nodes.
	//mStartNode = (PathingNode*) CGraphics::GetSingleton().picker->GetPickedObject();

	//Need to check and see if any nodes are in the hierarchy. if not, then just do nothing.
	unsigned nodesInHierarchy = PathingArea::GetSingleton().GetCurrentHierarchy()->GetNumNodes();
	if( nodesInHierarchy < 2 )
	{
		return;
	}
	
	unsigned index = PathingArea::GetSingleton().GetCurrentHierarchy()->NearestNodeToObject( this->Owner());
	mStartNode = PathingArea::GetSingleton().GetCurrentHierarchy()->GetNode( index );

	//std::bitset<64> LOSbits, safeBits;
	//PathingArea::GetSingleton().GetCurrentHierarchy()->DetermineLOSNodesFromObject( CGraphics::GetSingleton().objsList[0], LOSbits );

	//PathingArea::GetSingleton().GetCurrentHierarchy()->DetermineSafeNodesFromLOSBits( safeBits, LOSbits );



	//If no start node can be found, then we'll simply use the first node placed as the start.
	if( mStartNode == NULL )
	{
		mStartNode =  (PathingNode*) PathingArea::GetSingleton().GetCurrentHierarchy()->GetNode(0);
	}
	//mGoalNode = (PathingNode*) PathingArea::GetSingleton().GetCurrentHierarchy()->GetNode( PathingArea::GetSingleton().GetCurrentHierarchy()->GetNumNodes() - 1 );
	if( mGoalNode == NULL )
	{
		printf("Failed to find a Goal Node.\n");
		mSearchMode = SEARCH_FAILED;
		return;
	}

	//Make sure that the parents have been set to NULL as the path will likely change and thus the parents will too.
	mStartNode->SetPreviousNode( NULL );
	mGoalNode->SetPreviousNode( NULL );

	//This is an early out, since we may be standing on the goal node immediately.
	if( mStartNode == mGoalNode )
	{
		mSearchMode = SEARCH_SUCCESS;
	}
	//Also as a quick be done, we can check to see if there are no connections for either the start or goal node.
	//	If one of them has no connections, then that means there's no possible path to it.
	//The reason the check here is against 1 is because all nodes are connected to themselves, so we need to check
	//	if it's more than a single connection.
	else if( mStartNode->NumConnectivityBits() <= 1 || mGoalNode->NumConnectivityBits() <= 1)
	{
		//This means search failed because no path exists.
		mSearchMode = SEARCH_FAILED;
	}
	else
	{
		//Start of the algorithm, we need to have the start node pushed onto the open list.
		PushOpenListQueue( mStartNode );

		//Explicitly set the final weight since we know the goal.

		//m_waypointList.push_back( g_terrain.GetCoordinates( m_goalNode.m_row, m_goalNode.m_column ));
		if( mHeuristicCalc == 0 )
		{
			mStartNode->SetFinalWeight( DistanceBetweenNodesEuclidean( mStartNode, mGoalNode ) ); 
		}
		else if( mHeuristicCalc == 1 )
		{
			mStartNode->SetFinalWeight( DistanceBetweenNodesCardinal( mStartNode, mGoalNode ) );
		}

		mSearchMode = SEARCH_PROCESSING;
	}
}

//This is the actual "A* algorithm" function.  Short and sweet, it goes
//	through the nodes, finds neighbors, does heuristic stuff, and checks
//	next best node.
void	Movement::SearchForPath()
{
	if( mSearchMode == SEARCH_SUCCESS || mSearchMode == SEARCH_FAILED )
	{
		mMovementType = MOVEMENT_WAYPOINT_LIST;
		return;
	}

	if( !IsQueueEmpty(mOpenlist) )
	{		
		PathingNode* nodeA = PopQueue( mOpenlist );	
		
		//This is to see if it's on either of the lists already with a lower value.
		//If it's found on either list, this function will update it's value and
		//	immediately resort the heap from that node.
		//			
		if( IsNodeGoal(nodeA) )
		{
			mSearchMode = SEARCH_SUCCESS;
			PushClosedListQueue(nodeA);
		}
		else
		{
			FindNeighborsOfNode(nodeA);
			PushClosedListQueue(nodeA);
		}
	}
	else
	{
		//Fail check
		mSearchMode = SEARCH_FAILED;
	}

	//Set up the searching cases for the algorithm
	//ColorAStarPath();
}

void	Movement::Update()
{
	//If path successfully found, do that stuff here.
	if( SEARCH_INIT == mSearchMode )
	{
		//Do nothing.
	}
	else if( SEARCH_SUCCESS == mSearchMode )
	{
		//Then generate the actual path from the closed list.
		GenerateWaypointsFromClosedList( );

		if( mRubberband )
		{
			RubberBandWaypoints( );
		}

		mOpenlist.m_heap.clear();
		mClosedlist.m_heap.clear();

		//mSearchMode = SEARCH_INIT;
		mMovementType = MOVEMENT_SEEK_TARGET;
	}


	//If path not yet found but still looking, do that here.
	else if( SEARCH_PROCESSING == mSearchMode )
	{
		mMovementType = MOVEMENT_NULL;
		//Check if it's single step of not.
		if( mSingleStep )
		{
			SearchForPath();
		}
		else
		{
			while( SEARCH_PROCESSING == mSearchMode )
			{
				SearchForPath();
			}
		}
	}

	//If path failed to be found, do that here.
	else if( SEARCH_FAILED == mSearchMode )
	{
		//YOU SUCK!!!!
		//
		//Clear waypoint list.
		//And push your own position to keep character from walking in place.

		//m_waypointList.clear();
		//m_waypointList.push_back( m_owner->GetBody().GetPos() );

		//g_terrain.ResetColors();

		mOpenlist.m_heap.clear();
		mClosedlist.m_heap.clear();

		mWaypointList.clear();
		mPathpointList.clear();

		mSearchMode = SEARCH_INIT;
	}

	else
	{
		mMovementType = MOVEMENT_NULL;
	}

	switch( mMovementType )
	{
		case MOVEMENT_NULL:
		{
			//Do nothing no path has been found or it's being generated.
			break;
		}

		case MOVEMENT_WAYPOINT_LIST:
		{
			//This means it's generating, so maybe we could play a waiting animation
			// or something to make it seem like they're doing something.
			break;
		}

		case MOVEMENT_SEEK_TARGET:
		{
			//Okay, path is found, waypoints are set.  Walk through them.
			//mOwner->GetBody()->SetPosition( mPathpointList[mCounter] );
			break;
		}

		default:
		{
			break;
		}
	}
}

//Goes through the closed list and finds this node pointer as it's the parent of
// the current node we're looking at.
void	 Movement:: FindParentNode( PathingNode* parentNode, PathingNode*	node )
{
	//Go through the closed list and find this one.
	CPriorityQueue::HEAP::iterator iter, end = mClosedlist.m_heap.end();

	for( iter = mClosedlist.m_heap.begin(); iter != end; ++iter )
	{
		if( AreNodesSame( (*iter), parentNode ) )
		{
			node = (*iter);
			return;
		}
	}
}

//For during A*, this goes through all of the nodes connected bits and finds all of those
//  it is connected with.  These will be put onto the open list to be checked later if 
//	necessary as the algorithm dictates.
void	Movement::FindNeighborsOfNode( PathingNode* curNode )
{
	//Go through the nodes bits.
	unsigned size = (unsigned)PathingArea::GetSingleton().GetCurrentHierarchy()->GetNumNodes();
	for( unsigned bitIter = 0; bitIter < size; ++bitIter )
	{
		if( bitIter == (unsigned)curNode->GetGUID() )
		{
			//Do nothing since this is the same node.
		}
		else
		{
			//If the bit is set, then add that node to the open list.
			if( curNode->IsConnectedToNode( bitIter ) )
			{
				AddNodeToOpenList( curNode, PathingArea::GetSingleton().GetCurrentHierarchy()->GetNode( bitIter ) );
			}
		}
	}
}

//Big work horse of the A* algorithm.  This is what will calculate heuristic values and check if it's on the
//	open or closed list with lower values.
void	Movement::AddNodeToOpenList( PathingNode* parentNode, PathingNode* checkingThisNode )
{
	float finalWeight = 0.f;
	float givenWeight = 0.f;

	//Also, make it's given cost the nodes given cost plus the distance between
	//	these two.
	if( mHeuristicCalc == 0 )
	{
		givenWeight = parentNode->GetGivenWeight() + DistanceBetweenNodesEuclidean( parentNode, checkingThisNode );

		float temp = DistanceBetweenNodesEuclidean(checkingThisNode, mGoalNode);
		temp += checkingThisNode->GetUserDefinedWeight();
		temp -= checkingThisNode->GetTacticalWeight();

		if( temp < 0.f )
		{
			temp = 0.f;
		}

		finalWeight = givenWeight + temp * mHeuristicWeight;
	}
	else if( mHeuristicCalc == 1 )
	{	
		givenWeight = parentNode->GetGivenWeight() + DistanceBetweenNodesCardinal( parentNode, checkingThisNode );

		float temp = DistanceBetweenNodesCardinal(checkingThisNode, mGoalNode);
		temp += checkingThisNode->GetUserDefinedWeight();
		temp -= checkingThisNode->GetTacticalWeight();

		finalWeight = givenWeight + temp * mHeuristicWeight;
	}

	checkingThisNode->SetTempFinal( finalWeight );
	checkingThisNode->SetTempGiven( givenWeight );

	//It's open and closed variables are set to false each upon the construction, which is good.
	if( IsNodeOnQueueLower( mOpenlist, checkingThisNode) || IsNodeOnQueueLower( mClosedlist, checkingThisNode) )				
	{
		//It is on the list with a lower value.
		return;
	}

	if( IsNodeOnQueue( mClosedlist, checkingThisNode ) || IsNodeOnQueue( mOpenlist, checkingThisNode ))
	{
		RemoveFromQueue( mClosedlist, checkingThisNode );
		RemoveFromQueue( mOpenlist, checkingThisNode );

		//If we're pushing onto the queue, then we now need to update the values.
		checkingThisNode->SetFinalWeight( finalWeight );
		checkingThisNode->SetGivenWeight( givenWeight );
		checkingThisNode->SetPreviousNode( parentNode );

		PushQueue( mOpenlist, checkingThisNode );
	}
	else
	{
		//If we're pushing onto the queue, then we now need to update the values.
		checkingThisNode->SetFinalWeight( finalWeight );
		checkingThisNode->SetGivenWeight( givenWeight );
		checkingThisNode->SetPreviousNode( parentNode );

		PushQueue( mOpenlist, checkingThisNode );
	}
}



//Waypoint list generation function.
void	Movement::GenerateWaypointsFromClosedList( )
{
	//Go through the closed list, starting with the goal.  It should be last always but we'll find
	// it by address here to be sure.
	bool startNodeFound = false;

	PathingNode* temp = mGoalNode;

	mPathpointList.clear();

	while( !startNodeFound )
	{
		mPathpointList.push_back( temp->GetPosition() );
		temp = temp->HasParentNode();

		if( temp )
		{
			//The start hasn't been found yet, keep going.
		}
		else
		{
			//We found the start, so set the boolean and be done.
			startNodeFound = true;
		}
	}
	//mPathpointList.push_back( mOwner->GetPosition() );

	std::reverse( mPathpointList.begin(), mPathpointList.end() );

	//Okay, here I need to generate a much bigger waypoint list so he can move along it
	// smoothly.  For now he'll just increment along it by set amount modified by some velocity.
	
	//For now, loop through points and generate 40 more points between these two.
	PathPointList		newPtList;

	unsigned numPoints = (unsigned)mPathpointList.size() - 1;
	for( unsigned wayIter = 0; wayIter < numPoints ; ++wayIter )
	{
		//Grab the current and next point in the list and generate 40 more points.   Vector algebra
		// for the win.
		D3DXVECTOR3	curDir = mPathpointList[wayIter + 1] - mPathpointList[wayIter];
		float length = D3DXVec3Length( &curDir );

		length /= 20;
		D3DXVec3Normalize( &curDir, &curDir );

		newPtList.push_back( mPathpointList[wayIter] );

		for( unsigned newPtsIter = 1; newPtsIter < 20; ++newPtsIter )
		{
			//make a new point off the increment of the line.
			newPtList.push_back( mPathpointList[wayIter] + curDir*(newPtsIter*length) );
		}
	}
	newPtList.push_back( mPathpointList[numPoints] );

	mPathpointList = newPtList;

	if( mPathpointList.size() <= 1 )
	{
	}
	else
	{
		//Now set up the renderable hierarchy and tell this thing to render.
		if( NULL == mpAStarPathRenderObj )
		{
			mpAStarPathRenderObj = new RenderableHierarchy( );
		}
		mpAStarPathRenderObj->UpdateBuffersWithPointList( mPathpointList );
		mpAStarPathRenderObj->SetDrawAsStrip(true);
		mbRenderAStarPath = true;
	}
}

void	Movement::RubberBandWaypoints( )
{
}

void	Movement::SmoothCurrentWaypoints( std::vector<D3DXVECTOR3>&	endPath )
{
}

D3DXVECTOR3		Movement::CatmullRom( D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, float u )
{
	return D3DXVECTOR3(0,0,0);
}

void	Movement::Render()
{
	if( mpAStarPathRenderObj )
	{
		mpAStarPathRenderObj->Render();
	}
}
	
void	Movement::ClearAStarValues()
{
	//Clear the waypointlist, the pathpointlist, and clear the renderable hierarchy so nothing draws.
	mWaypointList.clear();
	mPathpointList.clear();

	//mpAStarPathRenderObj->Clear();
	mbRenderAStarPath = false;

	//mStartNode = NULL;
	//mGoalNode  = NULL;

	mSearchMode = SEARCH_INIT;
}



void	Movement::LinearMovement()
{
	//Take into account the velocity, and base what point in the waypoint list the guy will go 
	//	to next off of that.  
	unsigned size = (unsigned)mPathpointList.size();
	if( 0 == size )
	{
		return;
	}

	float indexInc = mSpeedCurrent / 40;
	mCurIndex += indexInc;

	mCounter = (unsigned)mCurIndex;
	if( mCounter > size - 1 )
	{
		mCounter = size - 1;
	}

	mOwner->GetBody()->SetPosition( mPathpointList[mCounter] );
}
