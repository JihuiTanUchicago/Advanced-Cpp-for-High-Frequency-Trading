### Compilation
```
clang++ -std=c++17 -O2 -march=native main.cpp -o main
./main
```

### Overview
- `OrderManager`: Handles the lifecycle of client orders (MyOrder). It uses a map<int, unique_ptr<MyOrder>> to store and uniquely own each order.

- `MarketSnapshot`: Maintains the current state of the order book, storing top-of-book bid/ask levels with price-to-quantity mappings. Bids are stored in descending order using std::greater, and asks in ascending order using the default std::less.

- `main.cpp`: Loads a feed of market data from a file and processes bid/ask updates or executions by routing them to MarketSnapshot and OrderManager.

- Once a `unique_ptr` goes out of scope or is erased from the map, the corresponding `MyOrder` or `PriceLevel` object is automatically destroyed.