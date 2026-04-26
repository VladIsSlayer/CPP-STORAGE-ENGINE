#include "storage.h"
#include <iostream>
#include <chrono>

void benchmark_put(Storage& storage, int n) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; ++i)
    {
        storage.put("key" + std::to_string(i), "value");
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "PUT ops: " << n << "\n";
    std::cout << "Time: " << duration.count() << " ms\n";
    std::cout << "Ops/sec: " << (n * 1000.0 / duration.count()) << "\n";
}

void benchmark_get(Storage& storage, int n) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; ++i)
    {
        storage.get("key" + std::to_string(i));
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "GET ops: " << n << "\n";
    std::cout << "Time: " << duration.count() << " ms\n";
    std::cout << "0ps/sec: " << (n * 1000.0 / duration.count()) << "\n";
}