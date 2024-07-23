#include "AsyncMySQLClient.h"
#include <iostream>

AsyncMySQLClient::AsyncMySQLClient() : conn(mysql_init(nullptr)) {
    Log.init("../../config/LogConf/log.properties");
}

AsyncMySQLClient::~AsyncMySQLClient() {
    if (conn) {
        mysql_close(conn);
    }
}

void AsyncMySQLClient::connect(const std::string& host, int port, const std::string& user, const std::string& password) {
    std::string dbname= "";
    if (mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0) == nullptr) {
        std::cerr << "MySQL connection failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        conn = nullptr;
    }
}

void AsyncMySQLClient::connect(const std::string& host, int port, const std::string& user, const std::string& password, const std::string& dbname) {
    if (mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0) == nullptr) {
        std::cerr << "MySQL connection failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        conn = nullptr;
    }
}

void AsyncMySQLClient::write(const std::string& key, const std::string& value) {
    threads.emplace_back([this, key, value]() {
        std::lock_guard<std::mutex> lock(mtx);
        std::string query = "INSERT INTO test_db (id, name) VALUES ('" + key + "', '" + value + "')";
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "Write Error: " << mysql_error(conn) << std::endl;
            Log.Error("Write mysql Error %s ", mysql_error(conn));
        } else {
            std::cout << "Inserted: (" << key << ", " << value << ")" << std::endl;
            Log.Info("Inserted key : %s   value : %s  ", key.c_str(), value.c_str());
        }
    });
}

void AsyncMySQLClient::read(const std::string& key) {
    threads.emplace_back([this, key]() {
        std::lock_guard<std::mutex> lock(mtx);
        std::string query = "SELECT * FROM test_db WHERE id = '" + key + "'";
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "Read Error: " << mysql_error(conn) << std::endl;
            Log.Error("Read mysql Error %s   ", mysql_error(conn));
            return;
        }
        MYSQL_RES* result = mysql_store_result(conn);
        if (result == nullptr) {
            std::cerr << "Result Error: " << mysql_error(conn) << std::endl;
            Log.Error("mysql Result Error %s    ", mysql_error(conn));
            return;
        }
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            for (int i = 0; i < mysql_num_fields(result); i++) {
                std::cout << (row[i] ? row[i] : "NULL") << " ";
            }
            std::cout << std::endl;

        }
        mysql_free_result(result);
    });
}

void AsyncMySQLClient::run() {
    for (auto& th : threads) {
        th.join();
    }
    threads.clear();
}
