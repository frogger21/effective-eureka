#include <iostream>
#include <string>
#include <list>
#include <fstream>
using namespace std;

/*
Depth First Search & Topological Sort
*/

/*
---------------------------------------------------------------
STRUCTS
---------------------------------------------------------------
*/

//Edge has A->B
struct Edge {
	int src;
	int dest;
	Edge()
	{
		src = 0;
		dest = 0;
	}
};

//vertex 
struct Vertex {
	int vertex;
	bool explored;
	Vertex()
	{
		vertex = 0;
		explored = false;
	}
};

//The Graph network
struct Graph
{
	bool undirected; //is the graph network undirected?
	int Nvertices; //# of vertices
	int Medges; //# of edges
	std::list<Edge> Edges; //list of the edges
	std::list<Vertex> Vertices; //list of vertices;
};

/*
---------------------------------------------------------------
FUNCTIONS
---------------------------------------------------------------
*/

bool aNumber(char *a)
{
	//[48,57] is ascii for #s 0 to 9
	int temp = static_cast<int>(*a);
	if (temp <= 57 && temp >= 48)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool check(Graph &X, Edge &Y)
{
	//checks if a -> b already exists as b -> a
	list<Edge>::iterator i;
	for (i = X.Edges.begin(); i != X.Edges.end(); ++i)
	{
		if (Y.src == i->dest && Y.dest == i->src)
		{
			return true; //we have a match
		}
	}
	return false;
}

//print edges of the graph
void printGraphEdges(Graph &X)
{
	list<Edge>::iterator i;
	cout << "Graph Network\n";
	for (i = X.Edges.begin(); i != X.Edges.end(); ++i)
	{
		std::cout << "[" << i->src << "," << i->dest << "]" << endl;
	}
	std::cout << endl;
}

//print vertices of the graph
void printGraphVertices(Graph &X)
{
	list<Vertex>::iterator i;
	cout << "Graph vertices\n";
	for (i = X.Vertices.begin(); i != X.Vertices.end(); ++i)
	{
		std::cout << "Vertex :" << i->vertex << endl;
	}
	std::cout << endl;
}

//parses string line and looks for numbers to put into a list
//1 2 3 4 5 6 7 would be read as [1,2] [1,3] [1,4] [1,5] etc...
//ignores all non number ascii
void split(string line, Graph &X, bool undirected)
{
	string::size_type sz;
	Edge TempEdge;
	Vertex TempVertex;
	int NumCount = 0;
	int startPos, numLen, tracker, StringSize;
	int theNumber;
	int firstVertex;
	StringSize = line.size();
	bool Num = false;
	bool temp = false;
	for (int i = 0; i < StringSize; i++)
	{
		//is it a number?
		temp = aNumber(&line[i]);
		//if it's not a number then 
		if (temp == false)
		{
			Num = false;
		}
		//previous char was not a number but current is: we got a new number
		if (Num == false && temp == true)
		{
			Num = true;
			startPos = i;
			numLen = 1; //we don't know yet fully
			tracker = i + 1;
			while (tracker < StringSize)
			{
				if (aNumber(&line[tracker]) == true)
				{
					numLen += 1;
					tracker++;
				}
				else
				{
					// don't need to check these anymore cuz we already know 
					//it's # -1 b/c we i++ in for loop
					i += numLen - 1;
					break;
				}
			}
			string test2 = line.substr(startPos, numLen);
			theNumber = stoi(test2, &sz); //convert string to integer
			NumCount += 1;

			if (NumCount == 1)
			{
				firstVertex = theNumber;
				TempVertex.vertex = firstVertex;
				TempVertex.explored = false;
				X.Vertices.push_back(TempVertex); //add to vertex list
				X.Nvertices += 1;
			}
			else
			{
				TempEdge.src = firstVertex;
				TempEdge.dest = theNumber;
				if (undirected == true)
				{
					//undirected graph
					if (check(X, TempEdge) == false)
					{
						//we've never seen this before so add it.
						X.Edges.push_back(TempEdge);
						X.Medges += 1;
					}
				}
				else
				{
					//directed graph
					X.Edges.push_back(TempEdge);
					X.Medges += 1;
				}
			}
		}
	}
}

void markVertex(Graph &G, int s)
{
	//mark vertex s as explored
	list<Vertex>::iterator i;
	for (i = G.Vertices.begin(); i != G.Vertices.end(); ++i)
	{
		if (i->vertex == s)
		{
			i->explored = true;
			break; //stop loopin
		}
	}
}

bool virgin(Graph &G, int v)
{
	list<Vertex>::iterator i;
	for (i = G.Vertices.begin(); i != G.Vertices.end(); ++i)
	{
		if (i->vertex == v)
		{
			if (i->explored == true)
			{
				//it was already explored
				return false;
			}
			else {
				//it was never explored
				return true;
			}
		}
	}
	return false; //the code never reach here though...
}

//DFS Depth First Search algorithm is recursive 
void DFS(Graph &G, int s)
{
	//we are given vertex s that we will mark as explored
	markVertex(G, s);
	cout << s << " explored!\n";
	//for every edge (s,v) or s->v or src -> dest
	list<Edge>::iterator i;
	for (i = G.Edges.begin(); i != G.Edges.end(); ++i)
	{

		if (i->src == s)
		{
			//was v explored?
			if (virgin(G, i->dest) == true)
			{
				cout << "Edge [" << i->src << "," << i->dest << "]\n";
				//v was never explored so call DFS on it with s = v
				DFS(G, i->dest);
			}
		}
	}
}

/*
---------------------------------------------------------------
MAIN
---------------------------------------------------------------
*/
int main()
{
	string fileNom;
	fileNom = "D:/coursera/stanford data struc and algos/4/dfs1.txt"; //file path and file name
	Graph X;
	X.undirected = false; //the graph will be directed
	X.Nvertices = 0;
	X.Medges = 0;
	
	//Open file
	string line;
	ifstream myfile(fileNom);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			split(line, X, false);
		}
		myfile.close();
	}
	printGraphEdges(X);
	printGraphVertices(X);
	DFS(X, 1);

	//pause cmd
	string userInput;
	cout << "Exiting...\n";
	cin >> userInput;
	return 0;
}