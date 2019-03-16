#include <iostream>
#include <memory>
template<typename T>
class SmartPointer{
    private:
    T* ptr;
    size_t* count;

    
    public:
    SmartPointer(const SmartPointer& sp){
        if(this != &sp){
            ptr = sp.ptr;
            count = sp.count;
            (*count)++;
        }
    };
    SmartPointer(T* p=nullptr):ptr(p){
        if(p){
            count = new size_t(1);
        } else {
            count = new size_t(0);
        }
    };

    SmartPointer& operator=(SmartPointer& sp){
        if(ptr == sp.ptr) return *this;

        if(ptr){
            if(--(*count) == 0){
                delete ptr;
                delete count;
            }
        }

        ptr = sp.ptr;
        count = sp.count;
        (*count)++;
        return *this;  
    };

    T& operator->(){
        assert(ptr == nullptr);
        return ptr;
    };

    T* operator*(){
        assert(ptr == nullptr);
        return *ptr;
    };

    ~SmartPointer(){
        if(--(*count) ==  0){
            delete ptr;
            delete count;
        } 
    };

    size_t use_count(){
        return *count;
    };
};

int main(){
    SmartPointer<int> sp(new int(10));
    SmartPointer<int> sp2(sp);
    SmartPointer<int> sp3(new int(20));
    sp2 = sp3;
    std::cout << sp.use_count() << std::endl;
    std::cout << sp3.use_count() << std::endl;
    return 0;
}