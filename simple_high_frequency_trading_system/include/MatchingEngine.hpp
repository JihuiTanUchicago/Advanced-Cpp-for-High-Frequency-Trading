#pragma once

#include <unordered_map>
#include <type_traits>
#include <memory>
#include <functional>
#include "OrderManager.hpp"

using namespace std;


template <
    typename PriceType, typename OrderIdType, typename SymbolType,
    typename = enable_if_t<is_invocable_v<hash<PriceType>, const PriceType&> &&
                           is_invocable_v<hash<SymbolType>, const SymbolType&>>
>
class MatchingEngine {

public:
    using OrderType = Order<PriceType, OrderIdType>;
    using OrderBookType = OrderBook<PriceType, OrderIdType, SymbolType>;
    using OrderManagerType = OrderManager<PriceType, OrderIdType, SymbolType>;

    vector<shared_ptr<OrderType>> add_order(OrderType& order) {
        order_manager.add_order(order);
        return order_manager.match(order.symbol);
    }

    shared_ptr<OrderManagerType> modify_order(OrderType& order) {
        order_manager.modify_order(order);
        return order_manager.match(order.symbol);
    }

    void remove_order(OrderType& order) {
        order_manager.remove_order(order);
    }

    
private:
    OrderManagerType order_manager;
};