#include "RBTree.h"

int main(){
    RBTree<int, int> m;

	for(int i = 1; i <= 26; i++){
		m.insert(i,i);
	}

	m.print();
	
	for(int i = 1; i <= 14; i++)
		m.erase(i);
	
	m.print();
	for(int i = 15; i <= 26; i++)
		m.erase(i);
	m.print();



    return 0;
}