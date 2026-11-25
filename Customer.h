
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <thread>
#include <chrono>
#include <syncstream>
#include "OrderQueue.h"

class Customer {
private:
    int customerId;
    OrderQueue& orderQueue;

public:
    Customer(int id, OrderQueue& queue)
        : customerId(id), orderQueue(queue) {}

    void placeOrder(int orderId, std::string foodName,
                    int cookingTime, int deliveryTime) {

        Order order;

        order.orderId = orderId;
        order.customerId = customerId;
        order.foodName = foodName;
        order.cookingTime = cookingTime;
        order.deliveryTime = deliveryTime;

        std::osyncstream(std::cout)
            << "Customer " << customerId
            << " placed Order " << orderId
            << " (" << foodName << ")\n";

        orderQueue.addOrder(order);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(500));
    }
};

#endif
