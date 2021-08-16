#include "UnWeightedGraph.h"
#include "Utils.h"
#include "Utils2.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
using namespace std; 

#if 0
void DumpIntVec( const vector<int> &vec)
{
	if(vec.size() == 0)
	{
		cout << "No items in the vector." << endl;
		return;
	}	
	for(int i=0; i<vec.size()-1; ++i)
	{
		cout << vec[i] << ", ";
	}
	cout << vec[ vec.size()-1] << endl;
}

void DumpIntArray(const int array[], int sz) 
{
	for(int i=0; i<sz-1; ++i)
	{
		cout << array[i] << ", ";
	}
	cout << array[ sz-1] << endl;
}
#endif

void OutputQuotedString(ofstream &outFile, const char *buf)
{
	outFile << '"';
	outFile << buf;
	outFile << '"';
}

// ***************************************************************************
// Computing shortest path inside graph
// ***************************************************************************

int UnWeightedGraph :: GetAdjVert( int src, int lastAdj ) 
{
	// we want to find out adjacent nodes for node src
	// so we simply check whether edge (src, i) is an edge of graph
	// if so, return i
	int res = NIL_VERTEX;
	int start = lastAdj+1; 
	if(start < 0)
	{
		start = 0; 
	}
	for(int i= start; i < (int)listVertices.size(); ++i)
	{
		BGVertex *pv1 = &listVertices[src];
		BGVertex *pv2 = &listVertices[i];
		int j;
		for(j = 0; j<(int)listEdges.size() ; ++j)
		{
			BGEdge &pe = listEdges[j];
			if( ( pe.pv1 == pv1 && pe.pv2 == pv2) ||
					(pe.pv1 == pv2 && pe.pv2 == pv1)  )
			{
				res = i;
				break;
			}
		}
		if(j < (int)listEdges.size() )
		{
			break;
		}
	}

	return res;
}

bool UnWeightedGraph :: IsNeighour(int v1, int v2)
{
	BGVertex *pv1 = &listVertices[v1];
	BGVertex *pv2 = &listVertices[v2];
	int j;
	for(j = 0; j<(int)listEdges.size() ; ++j)
	{
		BGEdge &pe = listEdges[j];
		if( ( pe.pv1 == pv1 && pe.pv2 == pv2) ||
				(pe.pv1 == pv2 && pe.pv2 == pv1)  )
		{
			return true;
		}
	}


	return false;
}


bool UnWeightedGraph :: IsConnected()
{
	// This function checks if the graph is connected or not
	bool res = true; 

	// Initially, no vertex has been visited
	int numVerts = listVertices.size(); 
	bool *visited = new bool [  numVerts ]; 
	for(int i=0; i<numVerts; ++i)
	{
		visited[i] = false;
	}
	visited[0] = true;		// start from vertex 0, arbitarily
	while(true)
	{
		bool foundNew = false; 
		// we start from visited nodes to see if we can find new neighouring new nodes
		for(int i=0; i<numVerts; ++i)
		{
			if( visited[i] == true)
			{
				// Now get all i's neighour
				int ngbi = -1; 
				while( true )
				{
					ngbi = GetAdjVert ( i, ngbi ); 
					if(ngbi == NIL_VERTEX)
					{
						break;
					}
					if( visited[ ngbi ] == false)
					{
						visited [ ngbi ] = true; 
						foundNew =  true; 
						break; 
					}
				}

				if( foundNew == true)
				{
					break; 
				}

			}
		}

		if( foundNew == false) 
		{
			break;
		}
	}

	// Finally, we check if all vertices visited
	for(int i=0; i<numVerts; ++i)
	{
		if( visited [ i ] == false)
		{
			res = false;
			break;
		}
	}


	return res; 
}

void UnWeightedGraph :: OutputGML ( const char *inFileName )
{
	// Now output a file in GML format
	// First create a new name
	string name = inFileName;
//cout << "num edges = " << listEdges.size() << endl;

	DEBUG("FileName=");
	DEBUG(name);
	DEBUG("\n");
	// Now open file to write out
	ofstream outFile( name.c_str() );

	// First output some header info
	outFile << "graph [\n"; 
	outFile << "comment ";
	OutputQuotedString(outFile, "Automatically generated by Graphing tool");
	outFile << "\ndirected  0\n";
	outFile << "id  1\n";
	outFile << "label ";
	OutputQuotedString ( outFile, "To be more meaningful later....\n");

	// Now output all the vertices
	int i;
	int numVerts = listVertices.size(); 
	for( i= 0;  i< numVerts;   ++i)
	{
		outFile << "node [\n";
		char name[100];
		name[0] = 'v'; 
		sprintf(&name[1], "%d", i+1);
		outFile << "id " <<  i+1  << endl;
		outFile << "label ";
		OutputQuotedString (outFile,  name  ); 
		outFile << endl;
		outFile << "defaultAtrribute   1\n";
		outFile << "]\n";
	}

	// Now output all the edges
	for(i=0; i< numVerts; ++i )
	{
		for(int j=i+1; j<numVerts; ++j)
		{
			if( IsNeighour ( i, j) )
			{

//cout << "Output an edge \n"; 
				outFile << "edge [\n";
				outFile << "source " << i+1 << endl; 
				outFile << "target  " << j+1 << endl; 
				outFile << "label " ;
				OutputQuotedString( outFile,  ""  );
				outFile << "\n";
				outFile << "]\n";
			}
		}
	}


	// Finally quite after closing file
	outFile << "\n]\n";
	outFile.close();
}

// ***************************************************************************
// A more general graph class, still ad-hoc, hope we can get some solid library 
// ***************************************************************************

// ***************************************************************************
// Generic graph 
// ***************************************************************************

GenericGraph:: GenericGraph()
{
     nextId = 0;     // id starting from 1
}

int GenericGraph :: AddVertex(int val)
{
    int res = nextId;
    GraphVertex v(nextId, val);

    vertices.insert( map<int, GraphVertex>::value_type(nextId, v) );

    nextId++;       // never reuse id

    return res;
}

bool GenericGraph :: RemoveVertex(int id)
{
    map<int, GraphVertex> ::iterator it =  vertices.find(id);
    if( it == vertices.end() )
    {
        return false;
    }
    if( it != vertices.end() )
    {
        vertices.erase(it);
    }
	// now also remove its entry in the edge list
	if( adjacencyList.find( id ) != adjacencyList.end() )
	{
		adjacencyList.erase( id );
	}
	// also should remove every adj list when this id appears
    for( map<int, EDGE_LIST> :: const_iterator it2 = adjacencyList.begin(); it2 != adjacencyList.end(); ++it2)
    {
		vector<GraphEdge> edgeListNew;
        for(int i=0; i<(int)it2->second.size(); ++i)
        {
            GraphEdge e = it2->second[i];
            int v1, v2;
            e.GetVertexIDs( v1, v2 );
			if( v2 != id )
			{
				// keep it
				edgeListNew.push_back(e);
			}
		}
		// set the new list
		adjacencyList[it2->first] = edgeListNew;
    }
    return true;
}

int GenericGraph :: GetNumEdges() const 
{
    int numEdges = 0;

    for( map<int, EDGE_LIST> :: const_iterator it = adjacencyList.begin(); it != adjacencyList.end(); ++it)
    {
        numEdges += it->second.size();
    }

    return numEdges;      
}

int GenericGraph :: GetEdgeNum (int vid)
{
    if(vertices.find(vid) == vertices.end() || adjacencyList.find(vid) == adjacencyList.end())
    {
//cout << "No such vertex or not in adjacency list." << endl;
        return 0;
    }
    return adjacencyList[vid].size();
//    curpos = 0;
//    if( adjacencyList[vid].size() == 0 )
//    {
//cout << "Nothing in the adjacency list.\n";
//        return false;
//    }
//cout << "Ok, we found one edge." << endl;
//    e =  adjacencyList[vid][0];
//    return true;
}

GraphEdge * GenericGraph :: GetEdgeByIndex( int vid, int index )
{
    if(vertices.find(vid) == vertices.end() || adjacencyList.find(vid) == adjacencyList.end())
    {
        return NULL;
    }
//    curpos ++;
    if( (int)adjacencyList[vid].size() <= index  )
    {
        return NULL;
    }
    return &adjacencyList[vid][index];
//    return true;
}

GraphEdge *GenericGraph :: GetEdge( int vid, int uid) 
{
    if(vertices.find(vid) == vertices.end() || adjacencyList.find(vid) == adjacencyList.end())
    {
//cout << "Bad vertex: vid = " << vid << endl;
        return NULL;
    }
    for( int i=0; i<(int)adjacencyList[vid].size(); ++i  )
    {
        int v1, v2;
        adjacencyList[vid][i].GetVertexIDs( v1, v2 );
        if(  v2 == uid )
        {
            return &adjacencyList[vid][i];
        }
    }
    return NULL;
}

bool GenericGraph :: IsEdge(int vid, int uid) 
{
    // Check to see if (vid, uid) is an edge or not
    GraphEdge *pe = GetEdge(vid, uid);
    return (pe != NULL);
}

bool GenericGraph :: FindVertexByID(int id, GraphVertex &v)
{
    // Find by id
    if( vertices.find( id) == vertices.end() )
    {
        return false;
    }
    else
    {
         v = vertices[id];
         return true;
    }
}

GraphVertex* GenericGraph :: FindVertex( int id )
{
//for(map<int,GraphVertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
//{
//    cout << "FindVertex : Vertex id = " << it->first << endl;
//}
//cout << "qunery id = " << id << endl;
    if( vertices.find( id) == vertices.end() )
    {
//cout << "no, can not find it.\n";
        return NULL;
    }
    else
    {
//cout << "find it: vertex = " << vertices[id].GetID() << endl;
         return & vertices[id];
    }    
}


void GenericGraph :: SetVertexVisited(int vid,  bool flag )
{
    YW_ASSERT( vertices.find( vid) != vertices.end() );
    vertices[vid].SetVisited(flag);
}

bool GenericGraph :: IsVertexVisited( int vid )
{
    GraphVertex v;
    YW_ASSERT(  FindVertexByID ( vid , v) == true );

    return v.IsVisited() ;
}

void GenericGraph :: SetVertexLabel(int vid, string lbl)
{
    GraphVertex *pv = FindVertex( vid );
    YW_ASSERT_INFO(pv != NULL, "SetVertexLabel : Bad query");
    pv->SetLabel(lbl);
}

GraphVertex * GenericGraph :: GetVertexByLabel( string lbl )
{
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        if( it->second.GetLabel() == lbl )
        {
            return & vertices[it->first];
        }
    }
    return NULL;
}

void GenericGraph :: SetEdgeLabel(int vid1, int vid2, string lbl)
{
//cout << "vid1 = " << vid1 << ", vid2 = " << vid2  << "lbl = " << lbl << endl;
    GraphEdge *pe = GetEdge( vid1, vid2 );
    YW_ASSERT_INFO(pe != NULL, "SetEdgeLabel :: Bad query");
    pe->SetLabel(lbl);
}



// ***************************************************************************
// undirected graph 
// ***************************************************************************

UndirectedGraph ::    UndirectedGraph()
{
    prevArray = NULL;
}

UndirectedGraph :: ~UndirectedGraph()
{
    if( prevArray != NULL)
    {
        delete prevArray;
        prevArray = NULL;
    }
}



bool UndirectedGraph :: AddEdge(int vid1, int vid2, int val)
{
    // We need to add an edge to the adjacent list to both vertices
    // first make sure the graph is valid
    if(vertices.find(vid1) == vertices.end() || vertices.find(vid2) == vertices.end() )
    {
        return false;
    }

    // Then we add it to the adjaceny list 
    GraphEdge e(vid1, vid2, val);
    if(adjacencyList.find( vid1) == adjacencyList.end() )
    {
        EDGE_LIST el;
        adjacencyList.insert( map<int, EDGE_LIST> :: value_type(vid1, el)  );
    }
    adjacencyList[vid1].push_back(  e );
    if(adjacencyList.find( vid2) == adjacencyList.end() )
    {
        EDGE_LIST el;
        adjacencyList.insert( map<int, EDGE_LIST> :: value_type(vid2, el)  );
    }
    adjacencyList[vid2].push_back(  e );
    return true;
}

int UndirectedGraph :: GetNumEdges() const 
{
    return GenericGraph::GetNumEdges()/2;  // in undirected graph, we counted twice here
//    int numEdges = 0;

//    for( map<int, EDGE_LIST> :: const_iterator it = adjacencyList.begin(); it != adjacencyList.end(); ++it)
//    {
//        numEdges += it->second.size();
//    }

//    return numEdges/2;      // in undirected graph, we counted twice here
}

void UndirectedGraph :: InitTraversal()
{
    for(map<int, GraphVertex> :: iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        it->second.SetVisited(false);
    }
}


GraphEdge *UndirectedGraph :: GetEdge( int vid, int uid) 
{
    if(vertices.find(vid) == vertices.end() || adjacencyList.find(vid) == adjacencyList.end())
    {
//cout << "Bad vertex: vid = " << vid << endl;
        return NULL;
    }
    for( int i=0; i<(int)adjacencyList[vid].size(); ++i  )
    {
        int v1, v2;
        adjacencyList[vid][i].GetVertexIDs( v1, v2 );
        if( v1 == uid || v2 == uid )
        {
            return &adjacencyList[vid][i];
        }
    }
//cout << "Edge not in adjuacency list\n";
    return NULL;
}


int UndirectedGraph :: GetFirstNode( GraphVertex &v )
{
    // return -1 when done
    itCurrent = vertices.begin();
    if(itCurrent == vertices.end() )
    {
        // No vertices
        return -1;
    }
    else
    {
        v = (*itCurrent).second;
        return (*itCurrent).first;
    }
}

int UndirectedGraph :: GetNextNode( GraphVertex &v)
{
    // return id of the node
    ++itCurrent;
    if(itCurrent == vertices.end() )
    {
        // No vertices
        return -1;
    }
    else
    {
        v = (*itCurrent).second;
        return (*itCurrent).first;
    }
}


int UndirectedGraph :: TraversalFrom( int id, set<int> &listOfCCVertices )
{
    // Return value = number of nodes visited, and store the found cc vertices into the set
    // First mark the curernt node as visisted
    GraphVertex v;
    if( FindVertexByID ( id , v) == false)
    {
        return 0;
    }
    if(v.IsVisited() == true)
    {
        // No need to continue if already visited
        return 0;
    }
    v.SetVisited( true );
    listOfCCVertices.insert( id );

    // Now recurse into others
//    GRAPH_TRAV_POSITION pos;

    // This function traverse graph from node'id = id
    
//    if( GetFirstEdge( id, edge ) == false)
    if( GetEdgeNum(id) == 0)
    {
        return 1;    // only visited this node
    }

    int nRes = 0;
    for(int evIndex = 0; evIndex <GetEdgeNum(id); ++evIndex)
    {
        GraphEdge *pedge = GetEdgeByIndex( id, evIndex );
        YW_ASSERT( pedge != NULL);

        // Now move on to that edge's other node
        int id1, id2, idToUse;
        pedge->GetVertexIDs( id1, id2);
        YW_ASSERT( id1 != id2 );
        if( id1 == id)
        {
            idToUse = id2;
        }
        else 
        {
            idToUse = id1;
        }
        nRes += TraversalFrom(idToUse, listOfCCVertices);

        // Now move to the next
//        if( GetNextEdge( id, edge ) == false)
//        {
//           break;
//        }
    }

    return nRes;
}


int  UndirectedGraph :: FindUnvisitedNode ( )
{      
    // return the id of an unvisited node, if none return -1
    for(map<int, GraphVertex> :: iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        if ( it->second.IsVisited() == false )
        {
            return it->second.GetID();
        }
    }    
    return -1;
}

void UndirectedGraph :: InitPrevConfig()
{
    //if( prevArray != NULL)
    //{
      //  delete [] prevArray;
    //}
    prevMap.clear();
    nextMap.clear();
}

void UndirectedGraph :: DFSSetPrevNode( int u, int uprev )
{
//cout << "Set u=" << u << " prev is " << uprev << endl;
    map<int,int>::iterator it = prevMap.find(  u );
    if( it != prevMap.end() )
    {
        prevMap.erase( it );
    }
    // We need to make sure we can remove the record first before insertion
    prevMap.insert( map<int,int>::value_type(u, uprev) );
}

int UndirectedGraph :: DFSGetPrevNode( int u)
{
    if(prevMap.find(u) == prevMap.end() )
    {
        return -1;          // did not find anything
    }

    // Then we get the prev
    return prevMap[u];
}

void UndirectedGraph :: DFSSetNextNode(int u, int unext)
{
//cout << "Set u=" << u << " next to " << unext << endl;
    map<int,int>::iterator it = nextMap.find(  u );
    if( it != nextMap.end() )
    {
        nextMap.erase( it );
    }
    nextMap.insert( map<int,int>::value_type(u, unext) );
}

int UndirectedGraph :: DFSGetNextNode( int u)
{
    if(nextMap.find(u) == nextMap.end() )
    {
        return -1;          // did not find anything
    }

    // Then we get the prev
    return nextMap[u];
}

bool UndirectedGraph :: IsBipartitie()
{
    // CAUTION: only work for id = 0, 1, 2, 3, ... Do not support node deletion yet. TBD
    const int WHITE = -1;
    const int GRAY = 1;
    //const int BLACK = 2;

    if( GetNumVertices() == 0)
    {
        // No nodes
        return true;
    }

    // Test if this graph is bipartitie
    int *partition = new int[GetNumVertices()];
    int *color = new int[GetNumVertices()];
    for(int i=0; i<GetNumVertices(); ++i)
    {
        partition[i] = 0;
        color[i] = WHITE; 
    }

    int n = GetNumVertices();

    // Now start form node #1, whose id = 0. CAUTION: it is OK here since there is no deletion
    // if later on we may delete some nodes, we can not assume it anymore
//    partition[0] = 1;
//    color[0] = GRAY;
    
  for (int v=0; v<n; v++) // start at first vertex 
   { 
     if (color[v] != WHITE) continue; 

     color[v] = GRAY; 
     queue<int> toGrow;    // use BFS queue search 
     toGrow.push( v ); 

     while ( !toGrow.empty() ) 
     { 
        int grow = toGrow.front(); 
        toGrow.pop();
//cout << "pop " << grow << endl;        
        // Find neighour of this node
//        bool flag = GetFirstEdge(grow, ev);

        for( int evIndex = 0; evIndex < GetEdgeNum(grow); ++evIndex ) 
        { 
          GraphEdge *pev = GetEdgeByIndex( grow, evIndex);
          YW_ASSERT( pev != NULL );
          int id1, id2;
          pev->GetVertexIDs( id1, id2 );
          int u = id1;
          if( u == grow)
          {
              u = id2;
          }

          if ( color[u] == WHITE )          // not colored yet 
          { 
            color[u] = 3 - color[grow]; // set to other color 
            toGrow.push( u  ); 
//cout << "push " << u << endl;
          } 
          else // check for different color 
          { 
            if ( color[u] == color[grow] ) 
            {
//cout << "u=" << u << ", grow =" << grow << " are same color\n";
                delete [] partition;
                delete [] color;
                return false; 
            }
          } 
          // Now try to move to the next
//          flag = GetNextEdge( grow, ev );

        }
        
     }                                  // more nodes in this component 
   }                                    // while all components have been checked 
//cout << "here\n";

    delete [] partition;
    delete [] color;
    return true;
}

void  UndirectedGraph :: FindComponents( set< set<int> > &comps )
{
	// merging list of elements
	comps.clear();
	// init with each node as itself
//cout << "vertices size = " << vertices.size() << endl;
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
//cout << "add one singleton" << it->first << endl;
		set<int> single;
		single.insert( it->first );
		comps.insert(single);
	}
//cout << "Comp initial size = " << comps.size() << endl;
	// see if we can merge two sets if any edges are connected (well not very efficient...)
	bool fCont = true;
	while(fCont == true)
	{
//cout << "Inside while loop: Components = \n";
//for( set< set<int> > :: iterator it = comps.begin(); it != comps.end(); ++it)
//{
//DumpIntSet( *it );
//}

		fCont = false;
		for( set< set<int> > :: iterator it = comps.begin(); it != comps.end(); ++it )
		{
			// 
			 set< set<int> > :: iterator it2 = it;
			 it2 ++;
			 for( ; it2 != comps.end(); ++it2 )
			 {
			
				 // see if these two should be merged
				 for( set<int> :: iterator itt1 = (*it).begin(); itt1 !=(*it).end(); ++itt1)
				 {
					 for( set<int> :: iterator itt2 = (*it2).begin(); itt2 !=(*it2).end(); ++itt2)
					 {
//	cout << "itt1 = " << *itt1 << ", itt2 = " << *itt2 << endl;
						 // is these two connected?
						 if( IsEdge( *itt1, *itt2 ) == true )
						 {
//	cout << "yes, an edge\n";
							 // merge it
							 fCont = true;
							 set<int> snew = *it;
							 UnionSets(snew, *it2);
							 //UnionSets(*it, *it2);
							 comps.erase(*it);
							 comps.erase(it2);
							 comps.insert( snew );
							 break;
						 }
					 }
					 if( fCont == true)
					 {
						 break;
					 }
				 }
				 if( fCont == true)
				 {
					 break;
				 }
			 }
			 if( fCont == true)
			 {
				 break;
			 }
		}
	}
//cout << "Components = \n";
//for( set< set<int> > :: iterator it = comps.begin(); it != comps.end(); ++it)
//{
//DumpIntSet( *it );
//}
}


// ***************************************************************************
// directed graph 
// ***************************************************************************

bool DirectedGraph :: AddEdge(int vid1, int vid2, int val)
{
    // Here vid1 is source, vid2 is dest

    // first make sure the graph is valid
    if(vertices.find(vid1) == vertices.end() || vertices.find(vid2) == vertices.end() )
    {
        return false;
    }

    // Then we add it to the adjaceny list 
    GraphEdge e(vid1, vid2, val);
    if(adjacencyList.find( vid1) == adjacencyList.end() )
    {
        EDGE_LIST el;
        adjacencyList.insert( map<int, EDGE_LIST> :: value_type(vid1, el)  );
    }
    adjacencyList[vid1].push_back(  e );

    return true;
}
bool DirectedGraph :: IsNodeSink( int vid ) 
{
    // is this node a sink (i.e. no outgoing arcs?)
    //YW_ASSERT_INFO(  );
    if( adjacencyList.find(vid) == adjacencyList.end()  )
    {
        return true;
    }
	// otherwise, if the list is empty, also a sink
	if( adjacencyList[vid].size() == 0 )
	{
		return true;
	}
    return false;
}

bool DirectedGraph :: IsNodeSource( int vid ) 
{
	// check all adj list to see if anyone point to it
    for( map<int, EDGE_LIST> :: const_iterator it = adjacencyList.begin(); it != adjacencyList.end(); ++it)
	{
        for(int i=0; i<(int)it->second.size(); ++i)
        {
            GraphEdge e = it->second[i];
            int v1, v2;
            e.GetVertexIDs( v1, v2 );
			if( v2 == vid )
			{
				return false;
			}
		}

	}
	return true;
}

void DirectedGraph :: OutputGML( const char *fileName)
{
	// Now output a file in GML format
	// First create a new name
	string name = fileName;
//cout << "num edges = " << listEdges.size() << endl;

	DEBUG("FileName=");
	DEBUG(name);
	DEBUG("\n");
	// Now open file to write out
	ofstream outFile( name.c_str() );

	// First output some header info
	outFile << "graph [\n"; 
	outFile << "comment ";
	OutputQuotedString(outFile, "Automatically generated by Graphing tool");
	outFile << "\ndirected  1\n";
	outFile << "id  1\n";
	outFile << "label ";
	OutputQuotedString ( outFile, "To be more meaningful later....\n");

	// Now output all the vertices
	//int i;
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		outFile << "node [\n";
		const char *name = it->second.GetLabel().c_str();
        // the name is equal to it
//		name[0] = 'v'; 
//		sprintf(&name[1], "%d", i+1);
		outFile << "id " <<  it->first  << endl;
		outFile << "label ";
		OutputQuotedString (outFile,  name  ); 
		outFile << endl;
		outFile << "defaultAtrribute   1\n";
		outFile << "]\n";
	}

	// Now output all the edges
    for( map<int, EDGE_LIST> :: const_iterator it = adjacencyList.begin(); it != adjacencyList.end(); ++it)
    {
        // Output for each id
        for(int i=0; i<(int)it->second.size(); ++i)
        {
            GraphEdge e = it->second[i];
    		const char *name = e.GetLabel().c_str();
            int v1, v2;
            e.GetVertexIDs( v1, v2 );

			outFile << "edge [\n";
			outFile << "source " << v1 << endl; 
			outFile << "target  " << v2 << endl; 
			outFile << "label " ;
//cout << "edge label = " << name << endl;
			OutputQuotedString( outFile,  name  );
			outFile << "\n";
			outFile << "]\n";
        }
    }



#if 0
	for(int i=0; i< numVerts; ++i )
	{
		for(int j=i+1; j<numVerts; ++j)
		{
			if( IsNeighour ( i, j) )
			{

//cout << "Output an edge \n"; 
				outFile << "edge [\n";
				outFile << "source " << i+1 << endl; 
				outFile << "target  " << j+1 << endl; 
				outFile << "label " ;
				OutputQuotedString( outFile,  ""  );
				outFile << "\n";
				outFile << "]\n";
			}
		}
	}
#endif

	// Finally quite after closing file
	outFile << "\n]\n";
	outFile.close();
}

void DirectedGraph:: DFSVisitAcyclic(int nid, int &time, map<int,int> &nodesColor, map<int,int> &nodesdval, map<int,int> &nodesfval, vector<int> *plistFinishedNodes )
{
	// visit the 
	nodesColor[nid] = 1;
	time++;
	nodesdval[nid] = time;
//cout << "nid " << nid << ", D time = " << time << endl;
    for( int ii=0; ii<(int)adjacencyList[nid].size(); ++ii  )
    {
        // 
        int v1,v2;
        adjacencyList[nid][ii].GetVertexIDs( v1,v2 );
        YW_ASSERT_INFO( v1 == nid, "wrong here" );
        //stackVisitedDFS.push( v2 );
		if( nodesColor[v2] == 0 )
		{
			DFSVisitAcyclic(v2, time, nodesColor, nodesdval, nodesfval, plistFinishedNodes);
		}
    }
	// 
	nodesColor[nid] = 2;
	time++;
	nodesfval[nid] = time;
    
    if( plistFinishedNodes != NULL)
    {
        plistFinishedNodes->push_back(nid);
    }
//cout << "nid " << nid << ", F time = " << time << endl;
}

bool DirectedGraph :: IsAcyclic( ) 
{
    // for each node with in-degree 0, do a DFS search
    map<int,int> nodesColor;
	map<int,int> nodesdval;
	map<int,int> nodesfval;
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        nodesColor.insert( map<int,int>:: value_type( it->first, 0 ) );		// un-visited
        nodesdval.insert( map<int,int>:: value_type( it->first, 0 ) );
        nodesfval.insert( map<int,int>:: value_type( it->first, 0 ) );
    }
	int time = 0;
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		if( nodesColor[it->first] == 0 )
		{
			DFSVisitAcyclic( it->first, time, nodesColor, nodesdval, nodesfval  );
		}
	}
	// check each arc
	// if see an arc with src's time interval is contained inside dest's interval, then cycle!
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		int nodeid = it->first;
        for( int ii=0; ii<(int)adjacencyList[nodeid].size(); ++ii  )
        {
            // 
            int v1,v2;
            adjacencyList[nodeid][ii].GetVertexIDs( v1,v2 );
            YW_ASSERT_INFO( v1 == nodeid, "wrong here" );
//cout << "nid = " << nodeid << ", v2 = " << v2 << ", d1 = " << nodesdval[nodeid] << "f1 = ";
//cout << nodesfval[nodeid] <<", d2 = " << nodesdval[v2]  << ", f2 = " << nodesfval[v2] << endl;  
			if( nodesdval[nodeid] > nodesdval[v2] && nodesfval[nodeid] < nodesfval[v2]  ) 
			{
//cout << "Cycle here!\n";
				return false;
			}
		}
	}
    // test whether DFS has inconsistency
    return true;

#if 0
    // start from every node
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        // if this node is already visited, done
        if( nodesCompID[it->first] > 0 )
        {
            continue;
        }
cout << "Component id = " << compId << ", Outer: Checking node " << it->first << endl;

        // now explore DFS from it
        stack<int> stackVisitedDFS;
        stackVisitedDFS.push(it->first);
		int depth = 1;
        while(  stackVisitedDFS.empty() == false )
        {
            int nodeid = stackVisitedDFS.top();
cout << "Inner: Checking node " << nodeid << endl;
            stackVisitedDFS.pop();
            if(nodesCompID[nodeid] == compId && nodesDepth[nodeid] > 0 && nodesDepth[nodeid] < depth)
            {
                // found a cycle
cout << "in checking acyclic: nodeid = " << nodeid << " is found to the part of cycle\n";
                return false;
            }
            // if this is a cross edge, quit it
            if( nodesCompID[nodeid] > 0 )
            {
cout << "node " << nodeid << " has already been visited\n";
                continue;
            }

            nodesCompID[nodeid] = compId;
			//nodesDepth[nodeid] = depth;
cout << "node " << nodeid << " is assigned to compnent " << compId << endl;
            // get all its descendents in queue
            for( int ii=0; ii<(int)adjacencyList[nodeid].size(); ++ii  )
            {
                // 
                int v1,v2;
                adjacencyList[nodeid][ii].GetVertexIDs( v1,v2 );
                YW_ASSERT_INFO( v1 == nodeid, "wrong here" );
                stackVisitedDFS.push( v2 );
cout << "push stack v2 = " << v2 << endl;
            }
        }

        // now it is the turn of next component
        compId ++;
    }
#endif

}


void DirectedGraph :: TrimTreeArcs()
{
	// recursivly remove all nodes as sinks
	// loop to find one sink and remove it and start-over
	while(true)
	{
		// stop when the number of vertices is not very large
		if( GetNumVertices() < 2 )
		{
			break;
		}

		bool fFound = false;
		for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			if( IsNodeSink( it->first ) == true  || IsNodeSource(it->first) == true )
			{
				RemoveVertex( it->first );
				fFound = true;
				break;
			}
			// also reduce pure source nodes (e.g. no incoming edges)
		}
		if( fFound == false )
		{
			break;
		}
	}
}

void DirectedGraph :: TopologicalSort(vector<int> &listNodesFinished)
{
    //
    // for each node with in-degree 0, do a DFS search
    map<int,int> nodesColor;
	map<int,int> nodesdval;
	map<int,int> nodesfval;
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        nodesColor.insert( map<int,int>:: value_type( it->first, 0 ) );		// un-visited
        nodesdval.insert( map<int,int>:: value_type( it->first, 0 ) );
        nodesfval.insert( map<int,int>:: value_type( it->first, 0 ) );
    }
    //vector<int> listNodesFinished;
	int time = 0;
    for( map<int, GraphVertex> :: const_iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		if( nodesColor[it->first] == 0 )
		{
			DFSVisitAcyclic( it->first, time, nodesColor, nodesdval, nodesfval, &listNodesFinished  );
		}
	}
    
    //
    ReverseIntVec( listNodesFinished );
}

