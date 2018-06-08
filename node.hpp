#include <chrono>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <thread>
#include <cstdlib>
#include <cmath>

#ifndef __NODE__
#define __NODE__

#define TIME std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())

//std::mutex buss_tone_mutex;

typedef struct entry
{
	int destination;
	int nextHop;
	int numHops;
	int numSeq;
	time_t time;

	void print(){
		std::cout << "  \t" << destination << "  \t  " << nextHop << "  \t\t  " << numHops << "  \t\t  " << numSeq  << "\n";
	}
} Entry;

//*******************************************************************************************

int currentNumSeq(){
	static int next_ID = 1;
	return next_ID++;
}

typedef struct message
{
	int src_ID;
	int dst_ID;
	std::string msg;
	int numSeq;
	time_t time = TIME;
	message(){}
	message(int _src_ID, int _dst_ID, std::string _msg, int _numSeq = currentNumSeq()):
		src_ID(_src_ID),
		dst_ID(_dst_ID),
		msg(_msg),
		numSeq(_numSeq)
		{}

	std::string print(){
		char buffer [80];
		auto timeinfo = localtime (&time);
  		strftime (buffer,80,"%x-%X - ",timeinfo);
		std::cout 	<< buffer << "Source: " << src_ID << "  Destination: " << dst_ID 
					<< "  Mesasge: " << msg << "  Seq Number: " << numSeq << "\n";
	}
} Msg;

typedef struct package
{
	int dst_ID;
	Msg msg;
} Package;

//*******************************************************************************************
typedef struct position
{
	int x,y;

	void print(){
		std::cout << "x: " << x << "\ty: " << y << "\n";
	}
} Pos;
//*******************************************************************************************
int currentID(){
	static int next_ID = 1;
	return next_ID++;
}

class Node
{
	
public:
	int ID;
	int ratio;
	std::vector<Entry> routingTable;
	Pos pos;
	Node(int x, int y,int _ratio = 3): 
		ID(currentID()),
		pos{x,y}, 
		ratio(_ratio)
	{};	

	void sendMessage(int Dst, std::string msg){
		Msg m;
		if (auto a = std::find_if(routingTable.begin(), routingTable.end(), [&](const Entry& e){ return (e.destination == Dst); }) != routingTable.end()) {
			m = Msg{ID, Dst, msg};
		}else{
			m = Msg{ID, Dst, "Route Request"};
		}
		m.print();
		/*std::vector<int> prox;
		for(n: nodes){
			//std::cout << "Loops nodes" << "\n";
			if(n.ID != ID){
				if(isInRange(n)){
					if( !m.msg.substr(0,13).compare( {"Route Request",13}) ){
						std::cout << "Is a Route Request ID: " << n.ID << '\n';
						auto aux = m.msg;
						aux += " " + n.ID;
						prox.push_back(n.ID);
					}
				}
			}
		}
		for(p : prox){
			std::cout << p << "\n";
			auto aux = m.msg;
			aux += " " + nodes[p].ID;
			nodes[p].sendMessage(Dst, aux, nodes);
		}*/

	}
	bool isInRange(Node& n){
		return (n.pos.x - pos.x)* (n.pos.x - pos.x) + (n.pos.y - pos.y)* (n.pos.y - pos.y) <= ratio*ratio;
	}

	void printNeighbours(){
		std::cout << "Table of " << ID << "\n";
		std::cout << "Destination  \t  NextHop  \t  NumHops  \t  NumSeq  \n";
		for(auto rt : routingTable){
			rt.print();
		}
	}

	void discoverNeighbours(std::vector<Node> &nodes){
		routingTable.push_back({ID,ID,0,0,TIME});
		for(auto n: nodes){
			//std::cout << "Loops nodes" << "\n";
			if(n.ID != ID){
				if(isInRange(n)){
					routingTable.push_back({n.ID,n.ID,1,0,TIME});
				}
			}
		}
	}
	
	void sendTable(Node *n1, Node *n2){
		int isin = 0;
		int dest = 0;

		for (int i = 0; i < n1->routingTable.size(); ++i)
		{
			for (int j = 0; j <= n2->routingTable.size(); ++j)
			{
				
				if (n1->routingTable[i].destination == n2->ID)
				{
					isin = 1;
				}
				if (n1->routingTable[i].destination != n2->ID)
				{
					if(n1->routingTable[i].destination == n2->routingTable[j].destination)
					{
						isin = 1; 
						
						if((n1->routingTable[i].numHops > 0) && (n1->routingTable[i].numHops + 1 < n2->routingTable[j].numHops) && (isin ==1))
						{
							n2->routingTable[j].numHops = n1->routingTable[i].numHops + 1;
							n2->routingTable[j].nextHop = n1->ID;
						}
						  
					}
				}
				
				
			}
			if(isin == 0 && (n1->routingTable[i].numHops > 0))
				{
				//	std::cout<< "Hi Size " <<n2->routingTable.size()<<"\n";
				//	std::cout<< "Hi ID " <<n2->ID<<"\n";
					n2->routingTable.push_back({n1->routingTable[i].destination,n1->ID,(n1->routingTable[i].numHops + 1),0,TIME});
				}
			isin =0;
		}

	}
	void sendRegularUpdate(std::vector<Node> &nodes){
		for (int i = 0; i < nodes.size(); ++i)
		{
			for (int j = 0; j < nodes[i].routingTable.size();++j)
			{
				if(nodes[i].routingTable[j].numHops == 1)
				{
					for (int k = 0; k < nodes[i].routingTable.size() ; ++k)
					{
						if(nodes[i].routingTable[j].destination == nodes[k].ID)
						{
							//std::cout<< "mandando table i: " << i << "pra k: " << k << "\n";
							sendTable(&nodes[i],&nodes[k]);
						}
					}
				}
			}
		}
	}
/*	void BellmanFord(Entry routingTable, Entry routingTable2 vertices, list edges, vertex source)
   ::distance[],predecessor[]
   
   // This implementation takes in a graph, represented as
   // lists of vertices and edges, and fills two arrays
   // (distance and predecessor) with shortest-path
   // (less cost/distance/metric) information
   
   // Step 1: initialize graph
   for(int i= 0; i < &nodes->size(); i++)
   {
	
   }
   for each vertex v in vertices:
       distance[v] := inf             // At the beginning , all vertices have a weight of infinity
       predecessor[v] := null         // And a null predecessor
   
   distance[source] := 0              // The weight is zero at the source
   
   // Step 2: relax edges repeatedly
   for i from 1 to size(vertices)-1:
       for each edge (u, v) with weight w in edges:
           if distance[u] + w < distance[v]:
               distance[v] := distance[u] + w
               predecessor[v] := u
   
   // Step 3: check for negative-weight cycles
   for each edge (u, v) with weight w in edges:
       if distance[u] + w < distance[v]:
           error "Graph contains a negative-weight cycle"
   
  
   */
};

#endif //__NODE__
