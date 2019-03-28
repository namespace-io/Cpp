#include <iostream>
using namespace std;

int main(){
    int a[] = {-1, 2, 0, 4, 3, 6, 5, 8, -2, 1, 3, 0, 3, 6, 5, 2};
    
    int n = sizeof(a) / sizeof(int); 
    int* b = new int[11]{0};
    int* c = new int[n]{0};
    for(int i = 0; i < n; ++i){
        b[a[i] -(-2)]++;
    }
    for(auto& i : a){
        cout << i << " ";
    }
    
    for(int i = 1; i < 11; ++i){
        b[i] = b[i] + b[i-1];
    }
   
    cout << endl;
    for(int i = 0; i < 11; ++i)
        cout << b[i] << " ";

    cout << endl;
    for(int i = n - 1; i > -1; --i){
        c[--b[a[i]- (-2)]] = a[i];
    }
    
    cout << endl;
    for(int i = 0; i < n; ++i) 
        cout << c[i] << " ";

    delete[] c;
    delete[] b;

    return 0;
}