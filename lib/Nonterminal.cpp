#include "Nonterminal.h"
using namespace std;

Nonterminal:: Nonterminal(string term)
	:term( term )
{

}

void Nonterminal:: insertFollow(set<string> const  follow_set){
	for(set<string>::iterator itr = follow_set.begin(); itr != follow_set.end(); ++itr){
		follow.insert(*itr);
	}

}

void Nonterminal:: insertFirst(set<string> const first_set){
	
	for(set<string>::iterator itr = first_set.begin(); itr != first_set.end(); ++itr){
		first.insert(*itr);
	}
}



