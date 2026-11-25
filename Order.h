
#ifndef ORDER_H
#define ORDER_H

#include <string>

enum class OrderStatus {
    PLACED,
    COOKING,
    READY,
    PICKED_UP,
    DELIVERED
};

struct Order {
    int orderId;
    int customerId;
    std::string foodName;
    int cookingTime;
    int deliveryTime;

    int riderId = -1;
    bool isAssigned = false;

    OrderStatus status = OrderStatus::PLACED;
};

#endif
