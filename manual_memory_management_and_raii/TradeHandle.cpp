#include <iostream>
#include <string>
#include <utility>

using namespace std;

struct Trade {
    std::string symbol;
    double price;

    Trade(const std::string& sym, double p)
        : symbol(sym), price(p) {}
};

class TradeHandle {
    Trade* ptr;

public:
    TradeHandle(Trade* p) : ptr(p) {}
    ~TradeHandle() { 
        if (ptr == nullptr) return;
        cout << "Trade destroyed: " << ptr->symbol << endl;
        delete ptr; 
    }
    TradeHandle(const TradeHandle&) = delete;
    TradeHandle& operator=(const TradeHandle&) = delete;
    TradeHandle(TradeHandle&& other) noexcept : ptr(other.ptr) { 
        other.ptr = nullptr; 
    }
    TradeHandle& operator=(TradeHandle&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }    

    Trade* operator->() { return ptr; }
    Trade& operator*()  { return *ptr; }
};

int main() {
    TradeHandle h1(new Trade("AAPL", 150.0));
    cout << h1->symbol << " $" << h1->price << endl;

    TradeHandle h2 = std::move(h1);
    if (h1.operator->() == nullptr)
        cout << "h1 moved out." << endl;

    cout << (*h2).symbol << " still valid." << endl;
    return 0;
}