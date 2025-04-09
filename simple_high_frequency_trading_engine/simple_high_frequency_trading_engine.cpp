#include <chrono>
#include <vector>
#include <random>
#include <iostream>
#include <unordered_map>
#include <deque>
#include <numeric>
#include <thread>
#include <mutex>

using namespace std;
using namespace chrono;

/*
* Fake market data ticks that simulate real-time price feeds
*/
struct alignas(32) MarketData {
    int instrument_id;
    double price;
    high_resolution_clock::time_point timestamp;
};

class MarketDataFeed {
public:
    MarketDataFeed(vector<MarketData>& ref) : data(ref) {}

    void generateData(int num_ticks) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> price_dist(100.0, 200.0);

        data.reserve(num_ticks);
        for (int i = 0; i < num_ticks; ++i) {
            MarketData md;
            md.instrument_id = i % 10;
            md.price = price_dist(gen);
            md.timestamp = high_resolution_clock::now();
            data.push_back(md);
            this_thread::yield();
        }

        MarketData market_close_tick;
        market_close_tick.instrument_id = -1;
        data.push_back(market_close_tick);
    }

private:
    vector<MarketData>& data;
};

/*
* Create a TradeEngine class to process the market data and make decisions using three signals
*/
struct alignas(32) Order {
    int instrument_id;
    double price;
    bool is_buy;
    high_resolution_clock::time_point timestamp;
};

class TradeEngine {
public:
    TradeEngine(const vector<MarketData>& feed)
        : market_data(feed) {}

    void process() {
        long long index = 0;
        while (true) {
            if (index >= market_data.size()) {
                this_thread::yield();
                continue;
            }
            const auto& tick = market_data[index];
            index++;
            if (tick.instrument_id == -1) {
                break;
            }
            // Update history
            updateHistory(tick);

            // Apply signals
            bool buy = false, sell = false;

            if (signal1(tick)) {
                signal1_count += 1;
                buy = true;
            } 
            
            if (signal2(tick)) { 
                signal2_count += 1;
                if (tick.price < getAvg(tick.instrument_id)) {
                    buy = true;
                } else {
                    sell = true; 
                }
            } 

            if (signal3(tick)) {
                signal3_count += 1;
                buy = true;
            }

            if (buy || sell) {
                auto now = high_resolution_clock::now();
                Order o { tick.instrument_id, tick.price + (buy ? 0.01 : -0.01), buy, now };
                orders.push_back(o);
                auto latency = duration_cast<nanoseconds>(now - tick.timestamp).count();
                latencies.push_back(latency);
            }
        }
    }

    void reportStats() {
        long long sum = 0, max_latency = 0;
        for (auto l : latencies) {
            sum += l;
            if (l > max_latency) max_latency = l;
        }

        cout << "\n--- Performance Report ---" << endl;
        cout << "Total Market Ticks Processed: " << market_data.size() - 1 << "\n"; // deduct market close tick
        cout << "Total Orders Placed: " << orders.size() << "\n";
        cout << "Average Tick-to-Trade Latency (ns): " << (latencies.empty() ? 0 : sum / latencies.size()) << "\n";
        cout << "Maximum Tick-to-Trade Latency (ns): " << max_latency << "\n";
        cout << "Signal 1 Count: " << signal1_count << "\n";
        cout << "Signal 2 Count: " << signal2_count << "\n";
        cout << "Signal 3 Count: " << signal3_count << "\n";
    }

private:
    const vector<MarketData>& market_data;
    vector<Order> orders;
    vector<long long> latencies;
    unordered_map<int, deque<double>> price_history;
    unordered_map<int, double> sum_last_10_map;
    long signal1_count = 0, signal2_count = 0, signal3_count = 0;

    void updateHistory(const MarketData& tick) {
        auto& hist = price_history[tick.instrument_id];
        hist.push_back(tick.price);
        auto& sum_last_10 = sum_last_10_map[tick.instrument_id];
        sum_last_10 += tick.price;
        if (hist.size() > 10) {
            sum_last_10 -= hist[0];
            hist.pop_front();
        }
    }

    double getAvg(int id) {
        auto& hist = price_history[id];
        return hist.empty() ? 0 : sum_last_10_map[id] / hist.size();
    }

    // Signal 1: Price thresholds
    bool signal1(const MarketData& tick) {
        return tick.price < 105.0 || tick.price > 195.0;
    }

    // Signal 2: Deviation from average
    bool signal2(const MarketData& tick) {
        if (price_history[tick.instrument_id].size() < 5) return false;
        double avg = getAvg(tick.instrument_id);
        return tick.price < avg * 0.98 || tick.price > avg * 1.02;
    }

    // Signal 3: Simple momentum
    bool signal3(const MarketData& tick) {
        auto& hist = price_history[tick.instrument_id];
        if (hist.size() < 3) return false;
        double diff1 = hist[hist.size() - 2] - hist[hist.size() - 3];
        double diff2 = hist[hist.size() - 1] - hist[hist.size() - 2];
        return diff1 > 0 && diff2 > 0;
    }
};

int main() {
    vector<MarketData> feed;
    feed.reserve(100001);
    MarketDataFeed generator(feed);
    TradeEngine engine(feed);

    auto start = high_resolution_clock::now();
    
    thread generator_thread([&] { generator.generateData(100000); });
    thread engine_thread([&] { engine.process(); });
    generator_thread.join();
    engine_thread.join();

    auto end = high_resolution_clock::now();
    auto runtime = duration_cast<milliseconds>(end - start).count();

    engine.reportStats();
    cout << "Total Runtime (ms): " << runtime << endl;

    return 0;
}