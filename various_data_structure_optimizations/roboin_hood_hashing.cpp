#include <unordered_map>
#include <chrono>
#include <vector>
#include <tuple>
#include <functional>
#include <chrono>
#include <type_traits>

using namespace std;
using namespace std::chrono;

template <typename keyType>
class HashMap {

private:
    vector<tuple<keyType, int>> hashTable;
    int tableSize;

public:
    size_t hashFunction(keyType key) {
        return hash<keyType>{}(key) % tableSize;
    }

    void insert(keyType key) {

    }


};

int main() {
    
}