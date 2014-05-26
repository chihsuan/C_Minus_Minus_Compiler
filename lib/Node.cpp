#include <iostream>
#include <string>
#include "Node.h"
using namespace std;


Node:: Node(int index, string token, string symbol, string catergory)
	: index(index),
	  token(token),
	  symbol(symbol),
	  catergory(catergory)
{
}
