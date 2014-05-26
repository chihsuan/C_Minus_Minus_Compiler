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
		vector<Quadruples> getQuadruples();

	private:
		int name_index; 
		vector<Quadruples> quadruples;	
		vector<queue<string> > expr_v_queue;
		queue<Node> postfix(queue<Node> expr);
		stack<string> if_tmp_stack;

		string toString(int integer);
		void createQuadruples(queue<Node> post_expr);
		int priority(Node node);
		void outputQuadruples();
		void arrayCreate(queue<Node> post_expr);
};
