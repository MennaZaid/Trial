// implemented by Mohamed Enan
#ifndef MYSTACK_H
#define MYSTACK_H

#include <stdexcept>
#include <string>
using namespace std;
template<typename T>
class MyStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value);
    };
    Node* topNode;
    size_t stackSize;

public:
    MyStack();
    ~MyStack();
    MyStack(const MyStack& other);
    MyStack& operator=(const MyStack& other);
    void push(const T& value);
    void pop();
    T& top();
    const T& top() const;
    bool empty() const;
    size_t size() const;
    void clear();
};


#endif // MYSTACK_H