#include <iostream>
template <typename T>
void swap(T a[],int first,int second){
	T temp;
	temp = a[first];
	a[first] = a[second];
	a[second] = temp;
}
template <typename T>
void reverse(T a[],int begin,int end){
	while(begin < end){
		swap(a,begin++,end--);
	}
}


// 交换内存[a...b,b+1...d] -> [b+1...d,a...b]
template <typename T>
void exchange(T a[],int begin,int mid,int end){
	reverse(a,begin,mid);
	reverse(a,mid+1,end);
	reverse(a,begin,end);
}



// in-place MergeSort
template<typename T>
void MergeSort(T a[], int l, int r){
    if(l < r){
        int m, i, j;

        m = l + (r - l) / 2;
        i = l;
        j = m+1;
        // [l, i)为已经排序好的序列
        MergeSort(a, l, m);
        MergeSort(a, m + 1, r);
        while(i < j && j < r+1){
            while(i < j && a[i] <= a[j]) ++i;
            int old_j = j;
            if(i == j) break;
            while(j < r + 1 && a[i] > a[j]) ++j;
            exchange(a, i, old_j - 1, j - 1);
            i = i + j - old_j + 1;
        }
    }
}


int main(){
    int a[] = {2,5,8,1,3,7};
    MergeSort(a, 0, 5);
    for(auto i : a) std::cout << i << " ";
    return 0;
}