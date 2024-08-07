#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <event2/event.h>
#include <mutex>
#include "AsyncMySQLClient.h"
#include "AsyncRedisClient.h"
#include "test_1.hpp"
#include <chrono>
#include <iomanip>

int test2(){
    AsyncMySQLClient mysqlClient;
    mysqlClient.connect("127.0.0.1", 3306, "test", "Gaosm@@0127", "test_db");

    for(int i = 0; i < 5000; i++){
        std::string str = "mysql_val" + std::to_string(i);
        mysqlClient.write(std::to_string(i), str);
    }
    for(int i = 0; i < 5000; i++){
        mysqlClient.read(std::to_string(i));
    }
    mysqlClient.run();

    //Redis Client
    AsyncRedisClient redisClient;
    redisClient.connect("127.0.0.1", 6379, "default", "redis_password");
    redisClient.write("key111111", "redis_value1");
    redisClient.write("key222222", "redis_value2");
    redisClient.read("key111111");
    redisClient.run();
    return 0;
}

void test_mpl(){
    //std::cout << "Total size of types in type_list: " << total_size::value << std::endl;

    //boost::mpl::for_each<sequence>(print_value());
    //std::cout << "Sum of integers from 1 to 500: " << sum::value << std::endl;
}

long int example_func(long int n) {

    if (n == 1) {
        return 1;
    } else {
        return n + example_func(n - 1);
    }
}


int main() {
    //test_mpl();
    std::cout << std::fixed;
    std::cout << std::setprecision(14);
    auto start = std::chrono::high_resolution_clock::now();
    const static long int f = 600;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << " result : " << Sum<f>::value << std::endl;
    auto start2 = std::chrono::high_resolution_clock::now();

    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;
    std::cout << "Execution time: " << duration2.count() << " seconds" << " result : " << example_func(f) << std::endl;

    return 0;
}