
#ifndef RACECONDITION_H
#define RACECONDITION_H

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <barrier>
#include <syncstream>

// Race condition demonstration
inline void demonstrateRaceCondition() {

    std::cout << "\n--- WITHOUT MUTEX ---\n";

    std::atomic<bool> isAssigned{false};
    std::atomic<int> acceptedCount{0};

    std::barrier syncPoint(2);

    auto acceptOrder = [&](int riderId) {

        bool available = !isAssigned.load();

        syncPoint.arrive_and_wait();

        if (available) {
            isAssigned.store(true);
            acceptedCount.fetch_add(1);

            std::osyncstream(std::cout)
                << "Rider " << riderId
                << " accepted Order 106\n";
        }
    };

    std::thread rider1(acceptOrder, 1);
    std::thread rider2(acceptOrder, 2);

    rider1.join();
    rider2.join();

    std::cout << "Total accepted: "
              << acceptedCount.load() << "\n";

    if (acceptedCount.load() > 1) {
        std::cout << "ERROR: Duplicate order assignment!\n";
    }
}

// Mutex solution
inline void demonstrateMutexFix() {

    std::cout << "\n--- WITH MUTEX ---\n";

    bool isAssigned = false;
    int acceptedCount = 0;

    std::mutex orderMutex;

    auto acceptOrder = [&](int riderId) {

        std::lock_guard<std::mutex> lock(orderMutex);

        if (!isAssigned) {

            isAssigned = true;
            acceptedCount++;

            std::cout << "Rider " << riderId
                      << " accepted Order 106\n";

        } else {

            std::cout << "Rider " << riderId
                      << " rejected: Order already assigned\n";
        }
    };

    std::thread rider1(acceptOrder, 1);
    std::thread rider2(acceptOrder, 2);

    rider1.join();
    rider2.join();

    std::cout << "Total accepted: "
              << acceptedCount << "\n";

    if (acceptedCount == 1) {
        std::cout << "SUCCESS: No duplicate assignment!\n";
    }
}

#endif
