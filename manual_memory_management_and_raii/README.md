### trade.cpp - discussions
- Following warning is produced if using `delete` on array
```
trade.cpp:31:5: warning: 'delete' applied to a pointer that was allocated with 'new[]'; did you mean 'delete[]'? [-Wmismatched-new-delete]
    delete trades;
    ^
          []
trade.cpp:23:21: note: allocated with 'new[]' here
    Trade* trades = new Trade[5];
                    ^
1 warning generated.
```

- `delete` on array leads to undefined behavior even though program can run

- Forgetting to call `delete` will lead to memory leak since the memory is never freed.

- `delete` twice on the same pointer lead to the following output and exit of program:
```
trade(69439,0x1dbbbdc40) malloc: Double free of object 0x157704380
trade(69439,0x1dbbbdc40) malloc: *** set a breakpoint in malloc_error_break to debug
zsh: abort      ./trade
```

### TradeHandle.cpp - discussions
- 3 biggest risks of manual memory management: 1) forgetting to `delete`; 2) dangling pointers; 3) double freeing

- Forgetting to `delete` can exhaust memory space, which will significantly slow down HFT system. Fragmentations lead to more cache misses and make the data retrieval process longer.

- RAII reduces manual memory management risks by automatically freeing allocated memories and ensuring pointers are managed in an expected manner(e.g. unique_ptr, shared_ptr, etc.).

- Manual vs Automatic Memory Management: automatic memory management like `shared_ptr` introduce additional overheads like tracking reference count, which may slow down HFT system in ultra-low latency setting, while `new/delete` offers righer control and avoids this overhead.

