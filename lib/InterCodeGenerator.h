#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <queue>
#include <stack>
#include "Node.h"
#include <vector>

using namespace std;

class InterCodeGenerator{

	public:
		InterCodeGenerator();
		~InterCodeGenerator();
		void startGenerate(multimap<int, Node> parsingTree);
		void quadruples();

	private:
		void postfix(queue<Node> expr);
		void threeAddressCode();
		int priority(Node node);
		vector<queue<string> > expr_v_queue;
		void outputQuadruples();
};
