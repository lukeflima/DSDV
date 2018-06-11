#include <iostream>
#include <vector>

#include "node.hpp"



int main()
{
	std::vector<Node> nodes;
	int aux_x, aux_y, aux_r, n;
	std::cin >> n;
	for(int i = 0; i < n; i++){
		std::cin >> aux_x >> aux_y >> aux_r;
		nodes.emplace_back(aux_x, aux_y, aux_r);
	}

	for(auto &n: nodes){ n.pos.print(); }	
	for(auto &n: nodes){ n.discoverNeighbours(nodes); }	
	for(auto &n: nodes){ n.sendRegularUpdate(nodes); }
	for(auto &n: nodes){ n.printNeighbours(); }	
	
	int aux_index, aux_dst;	
	std::string aux_msg;
	std::cin >> n;
	for(int i = 0; i < n; i++){
		std::cin >> aux_index >> aux_dst;
		std::getline(std::cin, aux_msg);
		nodes[aux_index - 1].sendMessage(aux_dst, aux_msg, nodes);
		std::cout << "\n";
	}
	
	return 0;
}
