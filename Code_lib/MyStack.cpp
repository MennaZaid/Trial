#include "MyStack.h"
#include <string>
#include "ExpressionTree.h"
template<typename T>
MyStack<T>::Node::Node(const T& value) : data(value), next(nullptr) {}

// Stack constructor
template<typename T>
MyStack<T>::MyStack() : topNode(nullptr), stackSize(0) {}

// Destructor
template<typename T>
MyStack<T>::~MyStack() {
    clear();
}

// Copy constructor
template<typename T>
MyStack<T>::MyStack(const MyStack& other) : topNode(nullptr), stackSize(0) {
    if (other.empty()) return;

    // Create array to store values in correct order
    T* values = new T[other.stackSize];
    Node* current = other.topNode;
    size_t index = other.stackSize;

    while (current != nullptr) {
        values[--index] = current->data;
        current = current->next;
    }

    // Push values in correct order
    for (size_t i = 0; i < other.stackSize; ++i) {
        push(values[i]);
    }

    delete[] values;
}

// Assignment operator
template<typename T>
MyStack<T>& MyStack<T>::operator=(const MyStack& other) {
    if (this != &other) {
        clear();

        if (!other.empty()) {
            // Create array to store values in correct order
            T* values = new T[other.stackSize];
            Node* current = other.topNode;
            size_t index = other.stackSize;

            while (current != nullptr) {
                values[--index] = current->data;
                current = current->next;
            }

            // Push values in correct order
            for (size_t i = 0; i < other.stackSize; ++i) {
                push(values[i]);
            }

            delete[] values;
        }
    }
    return *this;
}

// Push element to top of stack
template<typename T>
void MyStack<T>::push(const T& value) {
    Node* newNode = new Node(value);
    newNode->next = topNode;
    topNode = newNode;
    ++stackSize;
}

// Remove top element
template<typename T>
void MyStack<T>::pop() {
    if (empty()) {
        throw runtime_error("Cannot pop from empty stack");
    }
    Node* temp = topNode;
    topNode = topNode->next;
    delete temp;
    --stackSize;
}

// Return top element
template<typename T>
T& MyStack<T>::top() {
    if (empty()) {
        throw runtime_error("Cannot access top of empty stack");
    }
    return topNode->data;
}

// Return top element (const version)
template<typename T>
const T& MyStack<T>::top() const {
    if (empty()) {
        throw runtime_error("Cannot access top of empty stack");
    }
    return topNode->data;
}

// Check if stack is empty
template<typename T>
bool MyStack<T>::empty() const {
    return topNode == nullptr;
}

// Return size of stack
template<typename T>
size_t MyStack<T>::size() const {
    return stackSize;
}

// Clear the stack
template<typename T>
void MyStack<T>::clear() {
    while (!empty()) {
        pop();
    }
}
template class MyStack<int>;
template class MyStack<char>;
template class MyStack<double>;
template class MyStack<string>;
template class MyStack<ExpressionTree::TreeNode*>;