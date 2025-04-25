#pragma once

#include <type_traits>
#include <functional>
#include <vector>
#include <iostream>

using namespace std;

template <typename TradeType>
class TradeLogger {
    public:
        TradeLogger(size_t capacity = 10000) {
            trades.reserve(capacity);
        }

        void add_trade(const TradeType& trade) {
            trades.push_back(trade);
        }

        void print_trades() const {
            for (const auto& trade : trades) {
                cout << trade << endl;
            }
        }
    private:
        vector<TradeType> trades;
};