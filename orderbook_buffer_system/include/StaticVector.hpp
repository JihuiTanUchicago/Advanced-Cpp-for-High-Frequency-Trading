#pragma once
#include <array>
#include <cstddef>
#include <iostream>

using namespace std;

template<typename T, size_t N>
class StaticVector {
    private:
        T data[N];
        size_t index = 0;
    public:
        size_t size() const {
            return index;
        }

        T& operator[](size_t index) {
            return data[index];
        }

        void push_back(const T& value) {
            data[index] = value;
            index++;
        }

        T* begin() {
            return data;
        }

        T* end() {
            return data + index;
        }
};