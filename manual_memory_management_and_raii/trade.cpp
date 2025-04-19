#include <string>
#include <array>
#include <iostream>

using namespace std;

struct Trade {
    string symbol;
    double price;

    Trade() : symbol("None"), price(0.0){}
    Trade(const string& sym, double p)
        : symbol(sym), price(p) {}
};

int main() {
    // Dynamically allocate a single `Trade` object
    Trade* trade = new Trade("AAPL", 100.0);
    cout << trade->symbol << " " << trade->price << " initialized." << endl;
    delete trade;

    // Dynamically allocate an array of 5 `Trade` objects
    Trade* trades = new Trade[5];
    for (int i = 0; i < 5; ++i) {
        new (&trades[i]) Trade("AAPL", 101.0 + i);
    }
    for (int i = 0; i < 5; ++i) {
        cout << trades[i].symbol << " " << trades[i].price << " initialized." << endl;
    }

    delete[] trades;
    return 0;
}
