#pragma once

#include <memory>
#include <type_traits>
#include <functional>
#include <map>
#include "Order.hpp"

using namespace std;

template <
    typename PriceType, typename OrderIdType, typename SymbolType, 
        typename = enable_if_t<is_invocable_v<hash<PriceType>, const PriceType&>>
>
class OrderBook {

public:
    using OrderType = Order<PriceType, OrderIdType>;
    map<PriceType, vector<unique_ptr<OrderType>>, greater<PriceType>> bids;
    map<PriceType, vector<unique_ptr<OrderType>>> asks;

    void add_order(OrderType& order) {
        if (order.is_buy) {
            bids[order.price].push_back(make_unique<OrderType>(order));
        } else {
            asks[order.price].push_back(make_unique<OrderType>(order));
        }
    }

    void remove_order(OrderType& order) {
        if (order.is_buy) {
            auto& orders = bids[order.price];
            auto it = find_if(orders.begin(), orders.end(), [&](const auto& o) {
                return o->id == order.id;
            });
            if (it != orders.end()) {
                orders.erase(it);
            }
        } else {
            auto& orders = asks[order.price];
            auto it = find_if(orders.begin(), orders.end(), [&](const auto& o) {
                return o->id == order.id;
            });
            if (it != orders.end()) {
                orders.erase(it);
            }
        }
    }
};