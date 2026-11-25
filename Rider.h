
#ifndef RIDER_H
#define RIDER_H

#include <iostream>
#include <thread>
#include <chrono>
#include <syncstream>
#include "OrderQueue.h"

class Rider {
private:
    int riderId;
    OrderQueue& readyQueue;

public:
    Rider(int id, OrderQueue& queue)
        : riderId(id), readyQueue(queue) {}

    void deliverOrders() {
        Order order;

        while (readyQueue.getOrder(order)) {

            order.isAssigned = true;
            order.riderId = riderId;
            order.status = OrderStatus::PICKED_UP;

            std::osyncstream(std::cout)
                << "Rider " << riderId
                << " picked up Order "
                << order.orderId
                << " (" << order.foodName << ")\n";

            std::this_thread::sleep_for(
                std::chrono::milliseconds(
                    order.deliveryTime));

            order.status = OrderStatus::DELIVERED;

            std::osyncstream(std::cout)
                << "Rider " << riderId
                << " delivered Order "
                << order.orderId
                << " to Customer "
                << order.customerId << "\n";
        }

        std::osyncstream(std::cout)
            << "Rider " << riderId
            << " finished working.\n";
    }
};

#endif
