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
	int src_ID;
	int dst_ID;
	Msg msg;

	void print(){
		std::cout << "Source: " << src_ID <<  "  Destination: " << dst_ID << "\nMessage: ";
		msg.print();
	}
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
	Node(int x, int y,int _ratio = 5): 
		ID(currentID()),
		pos{x,y}, 
		ratio(_ratio)
	{};	
	bool sendMessage(int Dst, Msg msg, std::vector<Node> &nodes){
		
		auto dst = std::find_if(routingTable.begin(), routingTable.end(), [&](auto rt){ return rt.destination == Dst; });
		Package p{ID,(*dst).nextHop, msg};
		p.print();
		if(std::find_if(nodes.begin(), nodes.end(), [&](auto n){ return n.ID == (*dst).nextHop; }) != nodes.end()){
			for(auto &n: nodes){
				if(n.ID == p.dst_ID){
					if(p.msg.dst_ID == n.ID){
						std::cout  << n.ID << " Recived msg: " << p.msg.msg << "\n";
					} else{
						n.sendMessage(Dst, p.msg, nodes);
						return true;
					}
				}
			}
		}else{
			std::cout << "Node "  << (*dst).nextHop << " is inactive.\n"; 
			recalculate(nodes);
			return false;
		}
	}
	void recalculate(std::vector<Node> &nodes){
		for(auto &n: nodes){ n.discoverNeighbours(nodes); }	
		for(auto &n: nodes){ n.sendRegularUpdate(nodes); }
		for(auto &n: nodes){ n.printNeighbours(); }	
	}

	bool sendMessage(int Dst, std::string msg, std::vector<Node> &nodes){
		std::cout << ID << " Sending msg \"" << msg << "\" to " << Dst << "\n";
		Msg m;
		if (auto a = std::find_if(routingTable.begin(), routingTable.end(), [&](const Entry& e){ return (e.destination == Dst); }) != routingTable.end()) {
			m = Msg{ID, Dst, msg};
		} else{
			m = Msg{ID, Dst, "Route Request"};
		}
		m.print();
		auto dst = std::find_if(routingTable.begin(), routingTable.end(), [&](auto rt){ return rt.destination == Dst; });
		if(std::find_if(nodes.begin(), nodes.end(), [&](auto n){ return n.ID == (*dst).nextHop; }) != nodes.end()){
			Package p{ID,(*dst).nextHop, m};
			p.print();
			for(auto &n: nodes){
				if(n.ID == p.dst_ID){
					if(p.msg.dst_ID == n.ID){
						std::cout  << n.ID << " Recived msg: " << p.msg.msg << "\n \n";
					} else{
						n.sendMessage(Dst, p.msg, nodes);
						return true;
					}
				}
			}
		}else{
			std::cout << "Node "  << (*dst).nextHop << " is inactive.\n"; 
			recalculate(nodes);
			return true;
		}
		
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
		routingTable.clear();
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

		for (int i = 0; i < n1->routingTable.size(); ++i){
			for (int j = 0; j <= n2->routingTable.size(); ++j){
				if (n1->routingTable[i].destination == n2->ID){
					isin = 1;
				} 
				if (n1->routingTable[i].destination != n2->ID){
					if(n1->routingTable[i].destination == n2->routingTable[j].destination){
						isin = 1; 
						if((n1->routingTable[i].numHops > 0) && (n1->routingTable[i].numHops + 1 < n2->routingTable[j].numHops) && (isin ==1)){
							n2->routingTable[j].numHops = n1->routingTable[i].numHops + 1;
							n2->routingTable[j].nextHop = n1->ID;
						}  
					}
				}
			}
			if(isin == 0 && (n1->routingTable[i].numHops > 0)){
				n2->routingTable.push_back({n1->routingTable[i].destination,n1->ID,(n1->routingTable[i].numHops + 1),0,TIME});
			}
			isin =0;
		}

	}

	void sendRegularUpdate(std::vector<Node> &nodes){
		for (int i = 0; i < nodes.size(); ++i){
			for (int j = 0; j < nodes[i].routingTable.size();++j){
				if(nodes[i].routingTable[j].numHops == 1 ){
					for (int k = 0; k < nodes[i].routingTable.size() ; ++k){
						if(nodes[i].routingTable[j].destination == nodes[k].ID ){
							sendTable(&nodes[i],&nodes[k]);
						}
					}
				}
			}
		}
	}
};

#endif //__NODE__

