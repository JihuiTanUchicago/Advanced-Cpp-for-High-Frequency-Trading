#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <random>
#include <memory>

using namespace std;
using namespace chrono;


struct alignas(64) MarketData {
    string symbol;
    double price;
    int quantity;
    bool is_buy;
    high_resolution_clock::time_point timestamp;
};

class MarketDataFeed {

private:
    vector<string> SYMBOLS = {"AAPL", "TSLA", "AMZN"};
    mt19937 gen_;
    
public:
    MarketDataFeed() : gen_(random_device{}()) {}

    unique_ptr<MarketData> generateMarketData() {
        auto md = std::make_unique<MarketData>();
        unsigned int random_num = gen_();
        md->symbol = SYMBOLS[random_num % SYMBOLS.size()];
        md->price = 100.0 + random_num % 5 + (random_num % 100) / 100.0;
        md->quantity = random_num % 100;
        md->is_buy = (random_num % 2 == 0);
        md->timestamp = high_resolution_clock::now();
        return md;
    }
};