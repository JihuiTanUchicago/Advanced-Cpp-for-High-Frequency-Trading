#pragma once
#include <iostream>
#include <cmath>
#include <type_traits>

using namespace std;

// constexpr factorial
constexpr int factorial(int n) {
    return n == 0 ? 1 : n * factorial(n - 1);
}

// constexpr fibonacci
constexpr int fibonacci(int n) {
    return n < 2 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

constexpr double price_bucket(double price) {
    double rounded_price = (price * 100 - (int)(price * 100) % 5) / 100;
    return rounded_price;
}

constexpr int square(int x) {
    return x * x;
}