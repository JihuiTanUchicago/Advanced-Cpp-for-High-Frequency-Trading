#include <iostream>
#include <string>

struct Trade {
    std::string symbol;
    double price;

    Trade(const std::string& s, double p) : symbol(s), price(p) {
        std::cout << "Trade created: " << symbol << "\n";
    }

    ~Trade() {
        std::cout << "Trade destroyed: " << symbol << "\n";
    }
};

int main() {
    Trade* t1 = new Trade("AAPL", 150.0);
    Trade* t2 = new Trade("GOOG", 2800.0);
    delete t1;
    delete t2; // previous the program did not free t2
    // delete t1; ❌ double freeing the same memory thows an error

    Trade* t3 = new Trade("MSFT", 300.0);
    delete t3; //previous memory allocation via `new` should be freed
    t3 = new Trade("TSLA", 750.0);
    delete t3;

    Trade* trades = new Trade[3] {
        {"NVDA", 900.0},
        {"AMZN", 3200.0},
        {"META", 250.0}
    };
    //delete trades; ❌ delete the pointer of array is an undefined behavior
    delete[] trades;

    return 0;
}