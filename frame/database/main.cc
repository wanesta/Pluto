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
std::mutex mtx;

void connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        std::cerr << "Error: " << c->errstr << std::endl;
        return;
    }
    std::cout << "Connected to Redis" << std::endl;
}

void disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        std::cerr << "Error: " << c->errstr << std::endl;
        return;
    }
    std::cout << "Disconnected from Redis" << std::endl;
}

void getCallback(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = static_cast<redisReply *>(r);
    if (reply == nullptr) return;

    std::cout << "GET " << static_cast<const char*>(privdata) << ": " << reply->str << std::endl;
    redisAsyncDisconnect(c);
}

void writeToRedis(redisAsyncContext *c, const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(mtx);
    redisAsyncCommand(c, nullptr, nullptr, "SET %s %s", key.c_str(), value.c_str());
}

void readFromRedis(redisAsyncContext *c, const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);
    redisAsyncCommand(c, getCallback, (void *)key.c_str(), "GET %s", key.c_str());
}

void runEventLoop(struct event_base *base) {
    event_base_dispatch(base);
}

int test1(){
    struct event_base *base = event_base_new();
    if (!base) {
        std::cerr << "Could not initialize libevent!" << std::endl;
        return 1;
    }

    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
    if (c->err) {
        std::cerr << "Error: " << c->errstr << std::endl;
        return 1;
    }

    redisLibeventAttach(c, base);
    redisAsyncSetConnectCallback(c, connectCallback);
    redisAsyncSetDisconnectCallback(c, disconnectCallback);

    std::vector<std::thread> threads;

    // Write to Redis asynchronously
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(writeToRedis, c, "key" + std::to_string(i), "value" + std::to_string(i));
    }

    // Read from Redis asynchronously
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(readFromRedis, c, "key" + std::to_string(i));
    }

    std::thread eventThread(runEventLoop, base);

    for (auto &th : threads) {
        th.join();
    }

    eventThread.join();
    event_base_free(base);
}

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

    // Redis Client
//        AsyncRedisClient redisClient;
//        redisClient.connect("127.0.0.1", 6379, "default", "redis_password");
//        redisClient.write("key111111", "redis_value1");
//        redisClient.write("key222222", "redis_value2");
//        redisClient.read("key111111");
//        redisClient.run();
    return 0;
}
int main() {
    test2();
    //test1();
    return 0;
}