
#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "Order.h"

class OrderQueue {
private:
    std::queue<Order> orders;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool closed = false;

public:
    void addOrder(const Order& order) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            orders.push(order);
        }
        cv.notify_one();
    }

    bool getOrder(Order& order) {
        std::unique_lock<std::mutex> lock(queueMutex);

        cv.wait(lock, [this] {
            return !orders.empty() || closed;
        });

        if (orders.empty()) {
            return false;
        }

        order = orders.front();
        orders.pop();

        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            closed = true;
        }
        cv.notify_all();
    }
};

#endif
