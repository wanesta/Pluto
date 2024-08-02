#include "AsyncRedisClient.h"
#include <iostream>


//void AsyncRedisClient::connectCallback(const redisAsyncContext *c, int status) {
//    if (status != REDIS_OK) {
//        std::cerr << "Error: " << c->errstr << std::endl;
//        return;
//    }
//    std::cout << "Connected to Redis" << std::endl;
//}
//
//void AsyncRedisClient::disconnectCallback(const redisAsyncContext *c, int status) {
//    if (status != REDIS_OK) {
//        std::cerr << "Error: " << c->errstr << std::endl;
//        return;
//    }
//    std::cout << "Disconnected from Redis" << std::endl;
//}
//
//void AsyncRedisClient::getCallback(redisAsyncContext *c, void *r, void *privdata) {
//    redisReply *reply = static_cast<redisReply *>(r);
//    if (reply == nullptr) return;
//
//    std::cout << "GET " << static_cast<const char*>(privdata) << ": " << reply->str << std::endl;
//    redisAsyncDisconnect(c);
//}
//
//void AsyncRedisClient::write(redisAsyncContext *c, const std::string &key, const std::string &value) {
//    std::lock_guard<std::mutex> lock(mtx);
//    redisAsyncCommand(c, nullptr, nullptr, "SET %s %s", key.c_str(), value.c_str());
//}
//
//void AsyncRedisClient::read(redisAsyncContext *c, const std::string &key) {
//    std::lock_guard<std::mutex> lock(mtx);
//    redisAsyncCommand(c, getCallback, (void *)key.c_str(), "GET %s", key.c_str());
//}
//
//void AsyncRedisClient::run(struct event_base *base) {
//    event_base_dispatch(base);
//}


AsyncRedisClient::AsyncRedisClient() : base(event_base_new()), conn(nullptr) {
    Log.init("../../config/LogConf/log.properties");
}

AsyncRedisClient::~AsyncRedisClient() {
    if (this->base) {
        event_base_free(this->base);
    }
    if (this->conn) {
        redisAsyncDisconnect(this->conn);
    }
}

void AsyncRedisClient::connect(const std::string& host, int port, const std::string& user, const std::string& password) {
    this->conn = redisAsyncConnect(host.c_str(), port);
    if (conn->err) {
        //std::cerr << "Redis connection failed: " << conn->errstr << std::endl;
        Log.Error("Redis connection failed %s",conn->errstr);
        redisAsyncDisconnect(conn);
        conn = nullptr;
        return;
    }

    redisLibeventAttach(conn, base);
    redisAsyncSetConnectCallback(conn, connectCallback);
    redisAsyncSetDisconnectCallback(conn, disconnectCallback);
    redisAsyncCommand(conn, authCallback, nullptr, "AUTH %s %s", user.c_str(), password.c_str());
}

void AsyncRedisClient::connect(const std::string& host, int port, const std::string& user, const std::string& password, const std::string& dbname) {
    this->conn = redisAsyncConnect(host.c_str(), port);
    if (conn->err) {
        //std::cerr << "Redis connection failed: " << conn->errstr << std::endl;
        Log.Error("Redis connection failed %s",conn->errstr);
        redisAsyncDisconnect(conn);
        conn = nullptr;
        return;
    }

    redisLibeventAttach(conn, base);
    redisAsyncSetConnectCallback(conn, connectCallback);
    redisAsyncSetDisconnectCallback(conn, disconnectCallback);
    redisAsyncCommand(conn, authCallback, nullptr, "AUTH %s %s", user.c_str(), password.c_str());
}

void AsyncRedisClient::write(const std::string& key, const std::string& value) {
    threads.emplace_back([this, key, value]() {
        std::lock_guard<std::mutex> lock(mtx);
        redisAsyncCommand(conn, nullptr, nullptr, "SET %s %s", key.c_str(), value.c_str());
    });
}

void AsyncRedisClient::read(const std::string& key) {
    threads.emplace_back([this, key]() {
        std::lock_guard<std::mutex> lock(mtx);
        redisAsyncCommand(conn, getCallback, (void*)key.c_str(), "GET %s", key.c_str());
    });
}

void AsyncRedisClient::run() {
    std::thread eventThread(runEventLoop, base);

    for (auto& th : threads) {
        th.join();
    }

    eventThread.join();
    threads.clear();
}

void AsyncRedisClient::connectCallback(const redisAsyncContext* c, int status) {
    if (status != REDIS_OK) {
        std::cerr << "Error: " << c->errstr << std::endl;
        Log.Error("Redis connectCallback Error : %s ", c->errstr);
        return;
    }

    std::cout << "Connected to Redis" << std::endl;
}

void AsyncRedisClient::disconnectCallback(const redisAsyncContext* c, int status) {
    if (status != REDIS_OK) {
        //std::cerr << "Error: " << c->errstr << std::endl;
        Log.Error("Redis disconnect Callback Error : %s", c->errstr);
        return;
    }
    Log.Info(" Disconnected from Redis !");
    //std::cout << "Disconnected from Redis" << std::endl;
}

void AsyncRedisClient::authCallback(redisAsyncContext* c, void* r, void* privdata) {
    redisReply* reply = static_cast<redisReply*>(r);
    if (reply == nullptr) return;

    if (reply->type == REDIS_REPLY_ERROR) {
        //std::cerr << "Auth Error: ----" << reply->str << std::endl;
        Log.Error("Redis Auth Callback Error : %s",reply->str);
    } else {
        Log.Info(" ... Auth Success  ");
        //std::cout << "Auth Success" << std::endl;
    }
}

void AsyncRedisClient::getCallback(redisAsyncContext* c, void* r, void* privdata) {
    redisReply* reply = static_cast<redisReply*>(r);
    if (reply == nullptr) return;
    Log.Info(" GET %s, : %s ",static_cast<const char*>(privdata),reply->str);
    //std::cout << "GET " << static_cast<const char*>(privdata) << ": " << reply->str << std::endl;
}

void AsyncRedisClient::runEventLoop(struct event_base* base) {
    event_base_dispatch(base);
}