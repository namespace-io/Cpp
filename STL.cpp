#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
using namespace std;

void MultiSet(){
    multiset<int> s;

    for(int i = 0; i < 5; i++){
        s.insert(i);
        s.insert(i);
    }


    auto it = s.find(3) ;
    if(it != s.end()){
        s.erase(it);
    }
    for(auto it=s.begin(); it != s.end(); it++){
        cout << *it << " ";
    }

    cout << endl;
}

void next(){
    string s = "abc";
    while(std::next_permutation(s.begin(), s.end())){
        cout << s << endl;
    }
    cout << s << endl;
}
int main(){
    next();
    return 0;
}