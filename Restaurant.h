
#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <iostream>
#include <thread>
#include <chrono>
#include <syncstream>
#include "OrderQueue.h"

class Restaurant {
private:
    int restaurantId;
    OrderQueue& orderQueue;
    OrderQueue& readyQueue;

public:
    Restaurant(int id, OrderQueue& orders,
               OrderQueue& ready)
        : restaurantId(id),
          orderQueue(orders),
          readyQueue(ready) {}

    void prepareOrders() {
        Order order;

        while (orderQueue.getOrder(order)) {

            order.status = OrderStatus::COOKING;

            std::osyncstream(std::cout)
                << "Restaurant " << restaurantId
                << " is preparing Order "
                << order.orderId
                << " (" << order.foodName << ")\n";

            std::this_thread::sleep_for(
                std::chrono::milliseconds(
                    order.cookingTime));

            order.status = OrderStatus::READY;

            std::osyncstream(std::cout)
                << "Restaurant " << restaurantId
                << " completed Order "
                << order.orderId
                << " - Ready for pickup\n";

            readyQueue.addOrder(order);
        }

        std::osyncstream(std::cout)
            << "Restaurant " << restaurantId
            << " finished working.\n";
    }
};

#endif
