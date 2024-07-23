#ifndef PLUTO_ASYNCREDISCLIENT_H
#define PLUTO_ASYNCREDISCLIENT_H
#include "AsyncDatabaseClient.h"
#include "PLog.h"
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <event2/event.h>
#include <thread>
#include <vector>
#include <mutex>

class AsyncRedisClient : public AsyncDatabaseClient {
public:
    AsyncRedisClient();
    ~AsyncRedisClient();
    void connect(const std::string& host, int port, const std::string& user, const std::string& password) override;
    void write(const std::string& key, const std::string& value) override;
    void read(const std::string& key) override;
    void run() override;

private:
    redisAsyncContext* conn;
    struct event_base* base;
    std::vector<std::thread> threads;
    std::mutex mtx;
    static void connectCallback(const redisAsyncContext* c, int status);
    static void disconnectCallback(const redisAsyncContext* c, int status);
    static void authCallback(redisAsyncContext* c, void* r, void* privdata);
    static void getCallback(redisAsyncContext* c, void* r, void* privdata);
    static void runEventLoop(struct event_base* base);
};
#endif //PLUTO_ASYNCREDISCLIENT_H
