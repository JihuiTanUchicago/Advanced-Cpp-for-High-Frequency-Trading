#include <cstddef>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <mutex>
#include <iostream>

struct HeapAllocator {
    std::size_t capacity = 0; 
    std::size_t used     = 0; 

    explicit HeapAllocator(std::size_t cap) : capacity(cap) {}

    template<typename T>
    T* allocate(std::size_t n) {
        std::size_t req = n * sizeof(T);
        if (used + req > capacity) throw std::bad_alloc();
        used += req;
        return static_cast<T*>(std::malloc(req));
    }

    template<typename T>
    void deallocate(T* ptr, std::size_t n = 1) {
        std::free(ptr);
        used -= n * sizeof(T);
    }
};

// Stack Allocator
struct StackAllocator {
    void* buffer = nullptr;
    std::size_t capacity = 0;
    std::size_t offset = 0;

    StackAllocator(std::size_t size) {
        buffer = std::malloc(size);
        capacity = size;
        offset = 0;
    }

    ~StackAllocator() {
        std::free(buffer);
    }

    template<typename T>
    T* allocate(std::size_t n) {
        std::size_t required = n * sizeof(T);
        if (offset + required > capacity) throw std::bad_alloc();
        T* ptr = reinterpret_cast<T*>((char*)buffer + offset);
        offset += required;
        return ptr;
    }

    template<typename T>
    void deallocate(T*) {
        // Stack allocator does not deallocate individual objects
    }
};

// NoLock policy
struct NoLock {
    void lock() {}
    void unlock() {}
};

// MutexLock policy
struct MutexLock {
    std::mutex mtx;
    void lock() { mtx.lock(); }
    void unlock() { mtx.unlock(); }
};

template<typename T, typename AllocatorPolicy, typename ThreadingPolicy>
class OrderBookBuffer {
private:
    T* data;
    std::size_t capacity;
    std::size_t size = 0;
    AllocatorPolicy allocator;
    ThreadingPolicy lockPolicy;

public:
    OrderBookBuffer(std::size_t cap) : capacity(cap), allocator(cap * sizeof(T)) {
        data = allocator.template allocate<T>(capacity);
    }

    ~OrderBookBuffer() {
        allocator.template deallocate<T>(data);
    }

    void add_order(const T& order) {
        lockPolicy.lock();
        if (size < capacity) {
            data[size++] = order;
        } else {
            std::cerr << "OrderBookBuffer full!" << std::endl;
        }
        lockPolicy.unlock();
    }

    void print_orders() const {
        for (std::size_t i = 0; i < size; ++i) {
            std::cout << "Order " << data[i].id
                      << " | Price: " << data[i].price
                      << " | Qty: " << data[i].qty << '\n';
        }
    }
};


