### Compilation
At root, run:
```
make
```

Then
```
./hft_main
```

### Output
You should see something like
```
<List of Trading Activities>
Traded [Symbol: AMZN, Price: 100.45, Quantity: 45]
Traded [Symbol: AAPL, Price: 100.55, Quantity: 12]
Traded [Symbol: AAPL, Price: 100.55, Quantity: 43]
Traded [Symbol: AAPL, Price: 102.97, Quantity: 18]
Traded [Symbol: AMZN, Price: 103.43, Quantity: 24]
Traded [Symbol: AAPL, Price: 100.25, Quantity: 8]
Traded [Symbol: AAPL, Price: 100.25, Quantity: 17]
Traded [Symbol: AMZN, Price: 103.43, Quantity: 19]
Traded [Symbol: AMZN, Price: 103.83, Quantity: 55]
Tick-to-Trade Latency (nanoseconds):
Min: 583 | Max: 173750 | Mean: 1146.08
```

### Architecture
```
+------------------+       +-----------------+       +------------------+
| MarketDataFeed   |  -->  | OrderBook       | <---> | OrderManagement  |
+------------------+       +-----------------+       +------------------+
                                 |     |
                                 v     v
                         +-------------------+      +------------------+
                         | MatchingEngine    | ---> | TradeLogger      |
                         +-------------------+      +------------------+
```

### File Structure
```
hft_project/
├── include/
│   ├── MarketData.hpp
│   ├── Order.hpp
│   ├── OrderBook.hpp
│   ├── MatchingEngine.hpp
│   ├── OrderManager.hpp
│   ├── TradeLogger.hpp
│   └── Timer.hpp
├── src/
│   └── main.cpp
├── MakeFile
└── README.md
```