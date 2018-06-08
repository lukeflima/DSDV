#include <iostream>
#include <vector>

#include "node.hpp"



int main()
{
	std::vector<Node> nodes{  {1,2}, {0,0,7}, {4,5}, {5,5}, {0,-1,10} };
	for(auto &n: nodes){ n.pos.print(); }	
	for(auto &n: nodes){ n.discoverNeighbours(nodes); }	
	for(auto &n: nodes){ n.sendRegularUpdate(nodes); }
	for(auto &n: nodes){ n.printNeighbours(); }	
	//for(auto &n: nodes){ n.}	
	int inicial_index = 0;
	int index = inicial_index;
	std::vector<int> next_index;
	//nodes[index].sendMessage(2, "Hello :D", nodes);
	
	return 0;
}