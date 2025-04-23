#include <iostream>
#include "metaprogramming.hpp"
#include "constexpr_math.hpp"
#include "StaticVector.hpp"
#include "generic_algorithms.hpp"
#include "OrderBookBuffer.hpp"

using namespace std;

int main() {
    cout << "🚀 HFT Template Homework Starter\n";
    cout << "-------------------------" << endl;
    
    // Test metaprogramming
    cout << "Test Metaprogramming" << endl;
    static_assert(Factorial<5>::value == 120, "Incorrect factorial");
    static_assert(Fibonacci<7>::value == 13, "Incorrect fibonacci");
    static_assert(IsOdd<5>::value == true, "Incorrect IsOdd");
    static_assert(IsOdd<6>::value == false, "Incorrect IsOdd");
    print_if_odd<3>();
    print_all("Hello", "World", 111, 3.14);
    cout << "-------------------------" << endl;

    // Test constexpr_math
    static_assert(factorial(5) == 120, "Incorrect factorial");
    static_assert(fibonacci(7) == 13, "Incorrect fibonacci");
    static_assert(square(5) == 25, "Incorrect square");
    static_assert(price_bucket(101.73) == 101.70, "Incorrect price_bucket");
    constexpr int Size = 5;
    constexpr int Array[Size] = {1, 2, 3, 4, 5};
    cout << "Static Assert cases passed" << endl;
    cout << "-------------------------" << endl;
    

    // Test generic_algorithms and StaticVector
    struct Order { int id; double price; int qty; };
    StaticVector<Order, 3> orders;
    orders.push_back(Order{1, 101, 1});
    orders.push_back(Order{2, 102, 20});
    orders.push_back(Order{3, 103, 3});

    Order* start = orders.begin();
    Order* end = orders.end();
    cout << "Find all orders with price > 100:" << endl;
    while(start != end) {
        Order* it = generic_algorithms::find_if(start, end, [](Order& order) { return order.price > 100; });
        if (it != end) {
            cout << "Order " << it->id << " with price " << it->price << " and quantity " << it->qty << endl;
            start = it + 1;
        } else {
            break;
        }
    }

    cout << "Find first order with quantity divisible by 10:" << endl;
    start = orders.begin();
    Order* it = generic_algorithms::find_if(start, end, [](Order& order) { return order.qty % 10 == 0; });
    if (it != end) {
        cout << "Order " << it->id << " with price " << it->price << " and quantity " << it->qty << endl;
    } else {
        cout << "No order with quantity divisible by 10" << endl;
    }
    cout << "-------------------------" << endl;

    // Test OrderBookBuffer
    cout << "Test OrderBookBuffer" << endl;
    OrderBookBuffer<Order, StackAllocator, NoLock> book1(10);
    book1.add_order(Order{1, 101, 1});
    book1.add_order(Order{2, 102, 20});
    book1.print_orders();
    OrderBookBuffer<Order, HeapAllocator, MutexLock> book2(10);
    book2.add_order(Order{1, 101, 1});
    book2.add_order(Order{2, 102, 20});
    book2.print_orders();
    cout << "OrderBookBuffer cases passed" << endl;
    cout << "-------------------------" << endl;
    return 0;

    
}