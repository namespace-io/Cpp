#include <iostream>
#include <atomic>
#include <memory>
using namespace std;

template<typename T>
struct Node
{
    friend std::ostream& operator<<(std::ostream& os, const Node<T>& node){
        os << node.data << " ";
        return os;
    }
    T data;
    Node* prev;
    Node* next;
    Node():prev(nullptr), next(nullptr){};
    Node(const T &data):data(data), prev(nullptr), next(nullptr){};
};


template<typename T>
class List
{
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list){
        Node<T>* p = (list.head)->next;
        while(p != list.tail){
            os << p->data << " ";
            p = p->next;
        }
        os << std::endl;
        return os;
    }
private:
    /* data */
    Node<T> *head, *tail;
    size_t size;

public:
    List(/* args */);
    List(const std::initializer_list<T>& list):List(){
        
        for(auto &val : list){
            push_back(val);
        }
    };
    bool empty(){ return head->next == tail; }
    void push_back(const T& val){
        Node<T>* p = new Node<T>(val);
        p->next = tail;
        p->prev = tail->prev;

        tail->prev->next = p;
        tail->prev = p;
    }

    void pop_back(){
        if(!empty()){
            Node<T>* p = tail->prev;
            p->prev->next = tail;
            tail->prev = p->prev;
            delete p;
        }
    }

    void push_front(const T &val){
        Node<T>* p = new Node<T>(val);
        p->next = head->next;
        p->prev = head;

        head->next->prev = p;
        head->next = p;
    }

    void pop_front(){
        if(!empty()){
            Node<T>* p = head->next;
            p->next->prev = head;
            head->next = p->next;
            delete p;
        }
    }

    ~List();
};
template<typename T>
List<T>::List()
{
    head = new Node<T>();
    tail = new Node<T>();

    head->next = tail;
    head->prev = tail;

    tail->next = head;
    tail->prev = head;
}
template<typename T>
List<T>::~List<T>()
{
    Node<T>* p = head->next;
    while(p != tail){
        p = p->next;
        delete p->prev;
    }
    delete head;
    delete tail;
}

int main(){
    List<int> l{1, 2, 4};
    l.push_front(13);
    l.push_back(25);
    cout << l;
    l.pop_back();
    cout << l;
    return 0;   
}