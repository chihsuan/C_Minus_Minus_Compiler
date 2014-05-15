#include <iostream>
#include <string>

using namespace std;


#ifndef NODE_H
#define NODE_H

class Node{

	public:
		Node(int index, string token, string symbol, string catergory);
		int index;
		string token;
		string symbol;
		string catergory;
	private:


};

#endif
