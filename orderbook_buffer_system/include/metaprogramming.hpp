#pragma once
#include <iostream>
#include <type_traits>

using namespace std;

// TMP-based factorial
template<int N> struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};
template<> struct Factorial<0> {
    static constexpr int value = 1;
};

// TMP-based fibonacci
template<int N> struct Fibonacci {
    static constexpr int value = Fibonacci<N - 2>::value + Fibonacci<N - 1>::value;
};
template<> struct Fibonacci<1> {
    static constexpr int value = 1;
};
template<> struct Fibonacci<0> {
    static constexpr int value = 0;
};

// TMP-based IsOdd<T>
template<auto N, typename = enable_if_t<is_integral_v<decltype(N)> > >
struct IsOdd : bool_constant<N % 2 == 1> {};

template<auto N, typename = enable_if_t<IsOdd<N>::value> >
void print_if_odd() {
    cout << N << " is odd\n";
}

// prints all arguments, regardless of type or count
template<typename... Args>
void print_all(Args... args) {
    ((cout << args << " "), ...);
    cout << endl;
}
