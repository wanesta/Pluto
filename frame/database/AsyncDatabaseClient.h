#ifndef PLUTO_ASYNCDATABASECLIENT_H
#define PLUTO_ASYNCDATABASECLIENT_H
#include <string>

class AsyncDatabaseClient {
public:
    virtual ~AsyncDatabaseClient() = default;

    virtual void connect(const std::string& host, int port, const std::string& user, const std::string& password) = 0;
    virtual void connect(const std::string& host, int port, const std::string& user, const std::string& password, const std::string& dbname) = 0;
    virtual void write(const std::string& key, const std::string& value) = 0;
    virtual void read(const std::string& key) = 0;
    virtual void run() = 0;
};

#endif //PLUTO_ASYNCDATABASECLIENT_H
