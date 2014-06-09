#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
using namespace std;

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
