
#include <iostream>
#include <thread>
#include <chrono>

#include "Order.h"
#include "OrderQueue.h"
#include "Customer.h"
#include "Restaurant.h"
#include "Rider.h"
#include "RaceCondition.h"

int main() {
    std::cout << "FOOD DELIVERY SYSTEM - PHASE 1\n\n";

    auto start = std::chrono::steady_clock::now();

    OrderQueue orderQueue;
    OrderQueue readyQueue;

    Customer customer1(1, orderQueue);
    Customer customer2(2, orderQueue);
    Customer customer3(3, orderQueue);

    Restaurant restaurant1(1, orderQueue, readyQueue);
    Restaurant restaurant2(2, orderQueue, readyQueue);

    Rider rider1(1, readyQueue);
    Rider rider2(2, readyQueue);

    // Restaurant threads
    std::thread r1(&Restaurant::prepareOrders, &restaurant1);
    std::thread r2(&Restaurant::prepareOrders, &restaurant2);

    // Delivery rider threads
    std::thread d1(&Rider::deliverOrders, &rider1);
    std::thread d2(&Rider::deliverOrders, &rider2);

    // Customer threads
    std::thread c1([&]() {
        customer1.placeOrder(101, "Pizza", 3000, 5000);
        customer1.placeOrder(104, "Sandwich", 2000, 3000);
    });

    std::thread c2([&]() {
        customer2.placeOrder(102, "Burger", 2000, 4000);
        customer2.placeOrder(105, "Noodles", 3000, 5000);
    });

    std::thread c3([&]() {
        customer3.placeOrder(103, "Pasta", 4000, 6000);
    });

    // Wait for customers
    c1.join();
    c2.join();
    c3.join();

    orderQueue.close();

    // Wait for restaurants
    r1.join();
    r2.join();

    readyQueue.close();

    // Wait for riders
    d1.join();
    d2.join();

    auto end = std::chrono::steady_clock::now();

    double executionTime =
        std::chrono::duration<double>(end - start).count();

    std::cout << "\nALL WORKER THREADS COMPLETED\n";
    std::cout << "Total orders submitted: 5\n";
    std::cout << "Total customers: 3\n";
    std::cout << "Total restaurants: 2\n";
    std::cout << "Total riders: 2\n";
    std::cout << "Execution time: "
              << executionTime << " seconds\n";
              demonstrateRaceCondition();

demonstrateMutexFix();

return 0;

    
}
