#pragma once

#include <string>
#include <memory>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

template <typename PriceType, typename OrderIdType>
struct Order {
    OrderIdType id;
    string symbol;
    PriceType price;
    int quantity;
    bool is_buy;
    high_resolution_clock::time_point timestamp;

    Order(OrderIdType id, std::string sym, PriceType pr, int qty, bool buy, high_resolution_clock::time_point time)
        : id(id), symbol(std::move(sym)), price(pr), quantity(qty), is_buy(buy), timestamp(time) {}
};

template <typename PriceType, typename OrderIdType>
std::ostream& operator<<(std::ostream& os, const Order<PriceType, OrderIdType>& order) {
    os << "Traded [Symbol: " << order.symbol
       << ", Price: " << order.price
       << ", Quantity: " << order.quantity
       << "]";
    return os;
}