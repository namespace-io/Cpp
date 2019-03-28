#include <iostream>

template<typename T, size_t BLOCK_SIZE = 4096>
class MemoryPool
{
private:
    
    union Slot
    {
        T e;
        Slot *next;
    };
    size_t padding(char* p, size_t align){
        uintptr_t addr = reinterpret_cast<intptr_t>(p);
        return (size_t)((align - addr) % align);
    }
    
    void AllocBlock(){
        char* nb = reinterpret_cast<char*>(::operator new(BLOCK_SIZE));
        reinterpret_cast<Slot*>(nb)->next = cur_block;
        cur_block = reinterpret_cast<Slot*>(nb);
        cur_slot  = reinterpret_cast<Slot*>(nb + sizeof(Slot*) + padding(nb, alignof(Slot*)));
        last_slot = reinterpret_cast<Slot*>(nb + BLOCK_SIZE - sizeof(Slot) + 1); // cur_slot>=last_slot不够分配
    }
    /* data */
    Slot* cur_block;

    Slot* free_slot;
    Slot* cur_slot;
    Slot* last_slot;
public:


    // allocate deallocate construct destroy 为分配器接口
    T* allocate(){
        if(free_slot != nullptr){
            T* ret = reinterpret_cast<T*> (free_slot);
            free_slot = free_slot->next;
            return ret;
        } else {
            if(cur_slot >= last_slot){
                AllocBlock();
            }
            return reinterpret_cast<T*>(cur_slot++);
        }
    }

    void deallocate(T* p){
        if(p != nullptr){
            reinterpret_cast<Slot*>(p)->next = free_slot;
            free_slot = reinterpret_cast<Slot*>(p);
        }    
    }

    template<typename U, typename... Args>
    void construct(U* p, Args&& ...args){
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destory(U *p) { p->~U(); }

    template<typename... Args>
    T* newEle(Args&&... args){
        T* ret = allocate();
        construct(ret, std::forward<Args>(args)...);
        return ret; 
    }

    void delEle(T *p){
        if(p != nullptr){
            p->~T();
            deallocate(p);
        }
    }

    MemoryPool(/* args */):cur_block(nullptr), cur_slot(nullptr), free_slot(nullptr), last_slot(nullptr){};
    MemoryPool(const MemoryPool<T>& mp):MemoryPool(){} 

    ~MemoryPool(){
        auto cur = cur_block;
        while(cur != nullptr){
            auto next = cur->next;
            ::operator delete (reinterpret_cast<void*>(cur));
            cur = next;
        }
        
    };

    static_assert(BLOCK_SIZE >= 2 * sizeof(Slot), "BLOCKSIZE TOO SMALL!");
};


int main(){

    MemoryPool<int> mp;
    auto x = mp.allocate();
    *x = 4;
    std::cout << *x << std::endl;;
    mp.deallocate(x);
    return 0;
}