#ifndef PLUTO_ASYNCMYSQLCLIENT_H
#define PLUTO_ASYNCMYSQLCLIENT_H
#include "AsyncDatabaseClient.h"
#include "PLog.h"
#include <mysql/mysql.h>
#include <thread>
#include <vector>
#include <mutex>
#include <string>

class AsyncMySQLClient :public AsyncDatabaseClient{
public:
    AsyncMySQLClient();
    ~AsyncMySQLClient();
    void connect(const std::string& host, int port, const std::string& user, const std::string& password) override;
    void connect(const std::string& host, int port, const std::string& user, const std::string& password, const std::string& dbname) override;
    void write(const std::string& key, const std::string& value) override;
    void read(const std::string& key) override;
    void run() override;

private:
    MYSQL* conn;
    std::vector<std::thread> threads;
    std::mutex mtx;
};

#endif //PLUTO_ASYNCMYSQLCLIENT_H
