### Compilation
```
clang++ -std=c++20 -O2 -march=native simple_high_frequency_trading_engine.cpp -o simple_high_frequency_trading_engine
```

### Output
```
--- Performance Report ---
Total Market Ticks Processed: 100000
Total Orders Placed: 94134
Average Tick-to-Trade Latency (ns): 6763
Maximum Tick-to-Trade Latency (ns): 389750
Signal 1 Count: 9932
Signal 2 Count: 93424
Signal 3 Count: 16633
Total Runtime (ms): 33
```

### Reflections
- Signal 2 triggered the most
- Theoretically, generate data is the responsibility of the exchange, so to test our trade engine's latency, we should move generate data logic to a separate server(like on AWS, etc.).
- TradeEngine could parallelly process data belonging to different symbols, which could speed up the process significantly.