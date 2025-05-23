### How to Compile
Server:
```
g++ -std=c++20 -O2 main_server.cpp \
    -I"$(brew --prefix)/include" -L"$(brew --prefix)/lib" \
    -lboost_system -lpthread -o HFTserver
```

Client:
```
g++ -std=c++20 -O2 hft_client.cpp \
    -I"$(brew --prefix)/include" -L"$(brew --prefix)/lib" \
    -lboost_system -lpthread -o HFTclient
```