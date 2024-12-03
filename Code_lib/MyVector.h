#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <string>
#include <stdexcept>
using namespace std;
class MyVector {
private:
    string* data;
    int current_size;
    int current_capacity;

    void resize(int new_capacity);

public:
    MyVector();
    ~MyVector();
    MyVector(const MyVector& other);
    MyVector& operator=(const MyVector& other);

    void push_back(const std::string& value);
    void pop_back();
    std::string& at(int index);
    const string& at(int index) const;
    int getSize() const;
    bool empty() const;
    void clear();

    string& operator[](int index);
    const string& operator[](int index) const;

    class iterator {
    private:
        string* ptr;
    public:
        iterator(string* p) : ptr(p) {}
        string& operator*() { return *ptr; }
        iterator& operator++() { ++ptr; return *this; }
        iterator& operator--() { --ptr; return *this; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + current_size); }
    iterator rbegin() { return iterator(data + current_size - 1); }
    iterator rend() { return iterator(data - 1); }
};

#endif // MYVECTOR_H