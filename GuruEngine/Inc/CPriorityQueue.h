/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	PathingNode.h
brief:	A priority queue class, internally handled by a heap, which will
		be used for the open and closed lists by having internal sorting
		done.  
*/
#ifndef CPRIORITYQUEUE_H
#define CPRIORITYQUEUE_H

#include <vector>
#include <algorithm>


template < class Type >
class WeightGreater
{
public:
	bool operator() ( Type first, Type second ) const
	{
		return ( first->Weight() > second->Weight() );
	}
};


template < class Type >
class CTemplatePriorityQueue
{
	typedef std::vector<Type>	HEAP;
	//The priority queue will simply be a heap.

public:
	CTemplatePriorityQueue();
	bool IsQueueEmpty(  );
	void FrontOfQueue( Type& node );
	void PopQueue( Type& node );
	void PushQueue( Type node );
	void RemoveFromQueue( Type node );
	bool IsNodeOnQueueLower( const Type& node );
	bool IsNodeOnQueue( const Type& node );
	void Clear()	{ mHeap.clear(); }
	bool AreNodesSame( const Type& first, const Type& second );

private:
	//The actual heap implementing the queue.
	HEAP			mHeap;
};

template< class Type >
CTemplatePriorityQueue<Type>::CTemplatePriorityQueue()
{
}

template< class Type >
bool CTemplatePriorityQueue<Type>::IsQueueEmpty(  )
{
	return mHeap.empty();
}

template< class Type >
void CTemplatePriorityQueue<Type>::FrontOfQueue( Type& node )
{
	node = mHeap.front();
}

template< class Type >
void CTemplatePriorityQueue<Type>::PopQueue( Type& node )
{
	//Set the node to the front node.
	FrontOfQueue(node);

	//This function takes the first element and puts it at the end, then resorts
	// the rest of it up to the end.
	std::pop_heap( mHeap.begin(), mHeap.end(), WeightGreater<Type>() );

	//Now actually pop the back element off since this is now the one
	// node was set to.
	mHeap.pop_back();

	//return temp;
}

template< class Type >
void CTemplatePriorityQueue<Type>::PushQueue( Type node )
{	
	mHeap.push_back( node );

	//Now resort the whole thing as it's likely out of order now.
	std::push_heap( mHeap.begin(), mHeap.end(), WeightGreater<Type>() );
}

template< class Type >
void CTemplatePriorityQueue<Type>::RemoveFromQueue( Type node )
{
	//If the list has nothing, don't bother removing it.
	if( 0 == mHeap.size() )
	{
		return;
	}

	CTemplatePriorityQueue::HEAP::iterator iter, holder, end = mHeap.end();

	//This priority queue will be used as a temp.
	CTemplatePriorityQueue temp;

	for( iter = mHeap.begin(); iter != end; ++iter )
	{
		if( AreNodesSame( (*iter), node ) )
		{
			//Set the weight to negative, since we're not using negative weights.
			//(*iter).m_finalWeight = -1.0f;
			std::push_heap( mHeap.begin(), iter + 1, WeightGreater() );
			PopQueue( Type() );
			break;
		}
	}
}

template< class Type >
bool CTemplatePriorityQueue<Type>::IsNodeOnQueueLower( const Type& node )
{
	//A quick and easy out, if the queue's heap doesn't have any elements on it, just
	// return false.
	if( 0 == mHeap.size() )
	{
		return false;
	}

	//Check to see if the node is on the queue.  If it isn't, return false.
	CPriorityQueue::HEAP::iterator iter, end = queue.m_heap.end();

	for( iter = mHeap.begin(); iter != end; ++iter )
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
			if( (*iter)->Weight() > node->Weight() )
			{
				//This function will update the values of the node in (*begin).
				//UpdateNode( node, (*iter) );
				//node->SetFinalWeight( node->GetTempFinal() );
				//node->SetGivenWeight( node->GetTempGiven() );

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

template< class Type >
bool CTemplatePriorityQueue<Type>::IsNodeOnQueue( const Type& node )
{
	CTemplatePriorityQueue::HEAP::iterator iter, end = mHeap.end();

	for( iter = mHeap.begin(); iter != end; ++iter )
	{
		if( AreNodesSame( (*iter), node ) )
		{	
			return true;
		}
	}

	return false;
}


template< class Type >
bool CTemplatePriorityQueue<Type>::AreNodesSame( const Type& first, const Type& second )
{
	return ( &first == &second );
}


#endif
