#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <queue>
#include <stack>
#include "Node.h"
#include <iomanip>
#include <vector>
#include <cstdlib>
#include "Quadruples.h"


using namespace std;

class InterCodeGenerator{

	public:
		InterCodeGenerator();
		~InterCodeGenerator();
		void startGenerate(multimap<int, Node> parsingTree);

	private:
		int name_index; 
		vector<Quadruples> quadruples;	
		
		queue<Node> postfix(queue<Node> expr);
		void createQuadruples(queue<Node> post_expr);
		int priority(Node node);
		vector<queue<string> > expr_v_queue;
		void outputQuadruples();
};
