#include "MyVector.h"

MyVector::MyVector() : data(nullptr), current_size(0), current_capacity(0) {}

MyVector::~MyVector() {
    delete[] data;
}

MyVector::MyVector(const MyVector& other) : data(nullptr), current_size(0), current_capacity(0) {
    *this = other;
}

MyVector& MyVector::operator=(const MyVector& other) {
    if (this != &other) {
        delete[] data;
        current_size = other.current_size;
        current_capacity = other.current_capacity;

        if (current_capacity > 0) {
            data = new std::string[current_capacity];
            for (int i = 0; i < current_size; ++i) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }
    return *this;
}

void MyVector::resize(int new_capacity) {
    std::string* new_data = new std::string[new_capacity];

    for (int i = 0; i < current_size; ++i) {
        new_data[i] = data[i];
    }

    delete[] data;
    data = new_data;
    current_capacity = new_capacity;
}

void MyVector::push_back(const std::string& value) {
    if (current_size == current_capacity) {
        int new_capacity = current_capacity == 0 ? 1 : current_capacity * 2;
        resize(new_capacity);
    }
    data[current_size++] = value;
}

void MyVector::pop_back() {
    if (current_size > 0) {
        --current_size;
    }
}

std::string& MyVector::at(int index) {
    if (index < 0 || index >= current_size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

const std::string& MyVector::at(int index) const {
    if (index < 0 || index >= current_size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

int MyVector::getSize() const {
    return current_size;
}

bool MyVector::empty() const {
    return current_size == 0;
}

void MyVector::clear() {
    delete[] data;
    data = nullptr;
    current_size = 0;
    current_capacity = 0;
}

std::string& MyVector::operator[](int index) {
    return data[index];
}

const std::string& MyVector::operator[](int index) const {
    return data[index];
}