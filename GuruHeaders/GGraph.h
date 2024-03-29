
//////////////////////////////////////////////////////////////////////////
// Daniel Strycharske
// 08.14.12
// GGraph.h - Declaration of a generic templatized graph class.
//////////////////////////////////////////////////////////////////////////


#ifndef _GURU_GRAPH_H_
#define _GURU_GRAPH_H_


//A generic graph of nodes and arcs.  Bi-directional.
template <typename T>
class GGraph
{
	template <typename T>
	class GNode
	{
	public:
		GNode<T>();
		~GNode();

	private:

		//////////////////////////////////////////////////////////////////////////
		// All the node connections for this node. 
		// A node can be connected to potentially all other nodes.
		//////////////////////////////////////////////////////////////////////////
		std::vector< GArc<T> > mNodeArcs;
	};

	template <typename T>
	class GArc
	{
		//An arc has references to only two nodes.
		GGraph::GNode<T>* A, B;
	};

	//////////////////////////////////////////////////////////////////////////
	// Flag the graph as being one way only.
	//////////////////////////////////////////////////////////////////////////
	bool mOneWayGraph;

	//////////////////////////////////////////////////////////////////////////
	// All of the nodes in the graph.  The nodes themselves hold the arcs.
	//////////////////////////////////////////////////////////////////////////
	std::vector< GNode<T> > mNodes;
	
	//////////////////////////////////////////////////////////////////////////
	// All of the nodes' arcs in the graph.  Each arc will have a quick ref
	// index into this array.
	//////////////////////////////////////////////////////////////////////////
	std::vector< GArc<T> > mArcs;
};

#endif
