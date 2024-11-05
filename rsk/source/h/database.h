// database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <memory>
#include <string_view>

enum class DBConnectionStatus {
    NONE = 0,
    OK = 1,
    BAD = 2
};

enum class DBCommandStatus {
    OK = 0,
    EMPTY_QUERY = 1,
    TUPLES_OK = 2,
    ERROR = 5,
    BAD_RESPONSE = 6,
    UNEXPECTED = 7
};

class DBResult;

class Database {
public:
    Database();
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) noexcept = default;
    Database& operator=(Database&&) noexcept = default;

    [[nodiscard]] DBCommandStatus init();
    [[nodiscard]] DBConnectionStatus status() const;
    [[nodiscard]] std::string_view errorMessage() const;

    [[nodiscard]] DBConnectionStatus connect(std::string_view host, std::string_view user, std::string_view passwd, std::string_view db);
    void disconnect();
    [[nodiscard]] DBCommandStatus reset();

    [[nodiscard]] std::unique_ptr<DBResult> exec(std::string_view sql);
    void exec(DBResult& res, std::string_view sql);

private:
    std::unique_ptr<MYSQL, decltype(&mysql_close)> mysql;
    bool connected{false};
    std::string error;
};

class DBResult {
public:
    DBResult();
    explicit DBResult(MYSQL* mysql, std::string_view query);
    ~DBResult();

    DBResult(const DBResult&) = delete;
    DBResult& operator=(const DBResult&) = delete;
    DBResult(DBResult&&) noexcept = default;
    DBResult& operator=(DBResult&&) noexcept = default;

    void init(MYSQL* mysql, std::string_view query);

    [[nodiscard]] DBCommandStatus status() const;
    [[nodiscard]] int nrTuples() const;
    [[nodiscard]] int nrFields() const;

    [[nodiscard]] std::string_view fieldName(int n) const;
    [[nodiscard]] int fieldSize(int n) const;
    [[nodiscard]] int fieldSize(std::string_view name) const;

    void seekTuple(int tuple);
    [[nodiscard]] std::vector<std::string> getTuple();
    [[nodiscard]] std::vector<std::string> getTuple(int tuple);

    [[nodiscard]] long getAffectedRows() const;

private:
    bool haveError{false};
    std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> result;
    long m_nAffectedRows{0};
};

#endif // DATABASE_H

//BY GST ARMV8 GCC 13.2