#include <map>
#include <iostream>

using namespace std;

struct PriceLevel {
    double price;
    int quantity;

    PriceLevel(double p, int q) : price(p), quantity(q) {}
};

class MarketSnapshot {
    private:
        map<double, std::unique_ptr<PriceLevel>, greater<double>> bids; // sorted descending
        map<double, std::unique_ptr<PriceLevel>> asks; // sorted ascending
    public:
        void update_bid(double price, int qty) {
            cout << "update_bid(" << price << ", " << qty << ")" << endl;
            if (bids.find(price) == bids.end()) {
                bids[price] = make_unique<PriceLevel>(price, qty);
            } else {
                bids[price]->quantity = qty;
            }
        }
        void update_ask(double price, int qty) {
            cout << "update_ask(" << price << ", " << qty << ")" << endl;
            if (asks.find(price) == asks.end()) {
                asks[price] = make_unique<PriceLevel>(price, qty);
            } else {
                asks[price]->quantity = qty;
            }
        }
        const PriceLevel* get_best_bid() const {
            if (bids.empty()) {
                cout << "get_best_bid(), but bids is empty" << endl;
                return nullptr;
            }
            const PriceLevel* best_bid = bids.begin()->second.get();
            cout << "get_best_bid(), best_bid->price: " << best_bid->price << endl;
            return bids.begin()->second.get();
        }
        const PriceLevel* get_best_ask() const {
            if (asks.empty()) {
                cout << "get_best_ask(), but asks is empty" << endl;
                return nullptr;
            }
            cout << "get_best_ask(), best_ask->price: " << asks.begin()->second->price << endl;
            return asks.begin()->second.get();
        }
};