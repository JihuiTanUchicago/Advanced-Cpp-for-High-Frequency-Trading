#pragma once

#include <unordered_map>
#include <type_traits>
#include <memory>
#include <functional>
#include "OrderBook.hpp"

using namespace std;


template <
    typename PriceType, typename OrderIdType, typename SymbolType,
    typename = enable_if_t<is_invocable_v<hash<PriceType>, const PriceType&> &&
                           is_invocable_v<hash<SymbolType>, const SymbolType&>>
>
class OrderManager {
public:
    using OrderBookType = OrderBook<PriceType, OrderIdType, SymbolType>;
    using OrderType = typename OrderBookType::OrderType;
    enum class OrderStatus { New, Filled, PartiallyFilled, Cancelled };

    vector<shared_ptr<OrderType>> match(const SymbolType& symbol) {
        auto it = order_books.find(symbol);
        if (it == order_books.end()) {
            return {};
        }

        auto& orderBook = *(it->second);
        auto& bids = orderBook.bids;
        auto& asks = orderBook.asks;

        vector<shared_ptr<OrderType>> trades;

        while (!bids.empty() && !asks.empty()) {
            auto bestBidIt = bids.begin();
            auto bestAskIt = asks.begin();

            auto& bidOrders = bestBidIt->second;
            auto& askOrders = bestAskIt->second;

            if (bestBidIt->first < bestAskIt->first) {
                // No match possible
                break;
            }

            auto& buyOrder = bidOrders.front();
            auto& sellOrder = askOrders.front();

            // Find executed quantity (minimum of available quantities)
            int executedQty = min(buyOrder->quantity, sellOrder->quantity);
            PriceType executedPrice = sellOrder->price;

            trades.push_back(make_shared<OrderType>(
                OrderType{
                    -1, // dummy
                    symbol,
                    executedPrice,
                    executedQty,
                    true, // dummy
                    high_resolution_clock::now()
                }
            ));

            // Update quantities
            buyOrder->quantity -= executedQty;
            sellOrder->quantity -= executedQty;

            // Remove fully filled orders
            if (buyOrder->quantity == 0) {
                bidOrders.erase(bidOrders.begin());
                if (bidOrders.empty()) {
                    bids.erase(bestBidIt);
                }
            }

            if (sellOrder->quantity == 0) {
                askOrders.erase(askOrders.begin());
                if (askOrders.empty()) {
                    asks.erase(bestAskIt);
                }
            }
        }

        return trades;
    }

    void add_order(OrderType& order) {
        if (order_books.find(order.symbol) == order_books.end()) {
            order_books[order.symbol] = make_unique<OrderBookType>();
        }
        order_books[order.symbol]->add_order(order);
        order_statuses[order.id] = OrderStatus::New;
    }

    void remove_order(OrderType& order) {
        order_books[order.symbol]->remove_order(order);
        order_statuses.erase(order.id);
    }

    void modify_order(OrderType& order) {
        // Naive implmentation: modify by any means lose priority
        remove_order(order);
        add_order(order);
    }

private:
    static_assert(is_integral_v<OrderIdType>, 
                  "OrderIdType must be an integral type (int, long, etc).");
    unordered_map<SymbolType, unique_ptr<OrderBookType>> order_books;
    unordered_map<OrderIdType, OrderStatus> order_statuses;
    
};