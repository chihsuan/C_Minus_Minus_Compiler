#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <queue>
#include <stack>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <sstream>
#include "Node.h"
#include "Quadruples.h"


using namespace std;

class InterCodeGenerator{

	public:
		InterCodeGenerator();
		~InterCodeGenerator();
		void startGenerate(multimap<int, Node> parsingTree);
		vector<Quadruples> getQuadruples();

	private:
		int t_index; 
		vector<Quadruples> quadruples;	
		vector<queue<string> > expr_v_queue;
		queue<Node> postfix(queue<Node> expr);
		stack<int> while_ret_stack;
		stack<int> while_index_stack;
		stack<string> while_tmp_stack;
		stack<int> break_jmp_stack;
		stack<int> if_jmp_stack;
		stack<string> if_tmp_stack;
		stack<int> else_jmp_stack;
		stack<int> else_index;
		bool in_while;

		void createQuadruples(queue<Node> post_expr);
		bool whileStmt(int index, string token, string symbol);
		bool ifStmt(int index, string token, string symbol);
		int priority(Node node);
		void outputQuadruples();
		void arrayCreate(queue<Node> post_expr);
		string toString(int integer);
};
