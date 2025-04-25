#include <iostream>
#include <vector>
#include <numeric>
#include <memory>
#include <algorithm>
#include "../include/OrderBook.hpp"
#include "../include/OrderManager.hpp"
#include "../include/Order.hpp"
#include "../include/MatchingEngine.hpp"
#include "../include/MarketData.hpp"
#include "../include/TradeLogger.hpp"
#include "../include/Timer.hpp"

using namespace std;
using OrderType = Order<double, int>;

int main() {
    vector<long long> latencies;
    const int num_ticks = 10000;
    MarketDataFeed feed = MarketDataFeed();
    MatchingEngine<double, int, string> engine = MatchingEngine<double, int, string>();
    TradeLogger<OrderType> logger = TradeLogger<OrderType>(static_cast<size_t>(num_ticks));

    for (int i = 0; i < num_ticks; ++i) {
        Timer timer;
        timer.start();

        // Simulated tick + order match
        unique_ptr<MarketData> tick = feed.generateMarketData();
        OrderType order(i, tick->symbol, tick->price, tick->quantity, tick->is_buy, tick->timestamp);
        vector<shared_ptr<OrderType>> trades = engine.add_order(order);

        // Log trades
        if(!trades.empty()) {
            latencies.push_back(timer.stop());
            for (const auto& trade : trades) {
                logger.add_trade(*trade);
            }
        }
    }

    // Analyze latency
    auto min = *min_element(latencies.begin(), latencies.end());
    auto max = *max_element(latencies.begin(), latencies.end());
    double mean = accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();

    // Print stats
    logger.print_trades();
    cout << "Tick-to-Trade Latency (nanoseconds):\n";
    cout << "Min: " << min << " | Max: " << max << " | Mean: " << mean << '\n';
}