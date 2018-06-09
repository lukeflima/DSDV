#include <iostream>
#include <vector>

#include "node.hpp"



int main()
{
	std::vector<Node> nodes{  {1,2}, {0,0}, {4,5}, {6,5}, {0,-1} };
	for(auto &n: nodes){ n.pos.print(); }	
	for(auto &n: nodes){ n.discoverNeighbours(nodes); }	
	for(auto &n: nodes){ n.sendRegularUpdate(nodes); }
	for(auto &n: nodes){ n.printNeighbours(); }	
	//for(auto &n: nodes){ n.}	
	int inicial_index = 0;
	int index = inicial_index;
	std::vector<int> next_index;
	nodes[index].sendMessage(5, "Hello :D", nodes);
	nodes.erase(nodes.begin() + 2);
	nodes[0].sendMessage(4, "Vai dar certo2", nodes);
	nodes[1].sendMessage(5, "Vai dar certo", nodes);
	
	return 0;
}