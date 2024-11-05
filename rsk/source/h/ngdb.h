// ngdb.h
/*
 * ngdb.h 
 * netguard database interface head file for oracle.
 * billy shi
 * 2003/05/04
 */

#ifndef __NG_DB_H__
#define __NG_DB_H__

#include <string>
#include <memory>
#include <string_view>
#include "doorsbase.h"

class IOracleReader : public CDsObject {
public:
    ~IOracleReader() override = default;

    [[nodiscard]] virtual int ConnectToOracle(std::string_view userName, std::string_view password) = 0;
    virtual void Disconnect() = 0;
    [[nodiscard]] virtual int ExecSQL(std::string_view sql) = 0;
};

class COracleReader : public IOracleReader {
public:
    COracleReader() = default;
    ~COracleReader() override = default;

    COracleReader(const COracleReader&) = delete;
    COracleReader& operator=(const COracleReader&) = delete;
    COracleReader(COracleReader&&) noexcept = default;
    COracleReader& operator=(COracleReader&&) noexcept = default;

    [[nodiscard]] int ConnectToOracle(std::string_view userName, std::string_view password) override;
    void Disconnect() override;
    [[nodiscard]] int ExecSQL(std::string_view sql) override;

protected:
    void SqlError();
};

class IForbidClient : public CDsObject {
public:
    ~IForbidClient() override = default;
    [[nodiscard]] virtual std::string_view GetClientIp() const = 0;
};

class IForbidClientReader : public COracleReader {
public:
    ~IForbidClientReader() override = default;
    virtual void OpenRecordset() = 0;
    virtual void Close() = 0;
    [[nodiscard]] virtual std::shared_ptr<IForbidClient> ReadNextClient() = 0;
    [[nodiscard]] virtual bool IsEmpty() const = 0;
};

class IForbidUrl : public CDsObject {
public:
    ~IForbidUrl() override = default;
    [[nodiscard]] virtual std::string_view GetUrl() const = 0;
};

class IForbidUrlReader : public COracleReader {
public:
    ~IForbidUrlReader() override = default;
    virtual void OpenRecordset() = 0;
    virtual void Close() = 0;
    [[nodiscard]] virtual std::shared_ptr<IForbidUrl> ReadNextUrl() = 0;
};

class IForbidContent : public CDsObject {
public:
    ~IForbidContent() override = default;
    [[nodiscard]] virtual std::string_view GetContent() const = 0;
};

class IForbidContentReader : public COracleReader {
public:
    ~IForbidContentReader() override = default;
    virtual void OpenRecordset() = 0;
    virtual void Close() = 0;
    [[nodiscard]] virtual std::shared_ptr<IForbidContent> ReadNextContent() = 0;
};

class IRegClient : public CDsObject {
public:
    ~IRegClient() override = default;
    [[nodiscard]] virtual std::string_view GetClient() const = 0;
};

class IRegClientReader : public COracleReader {
public:
    ~IRegClientReader() override = default;
    virtual void OpenRecordset() = 0;
    virtual void Close() = 0;
    [[nodiscard]] virtual std::shared_ptr<IRegClient> ReadNextClient() = 0;
};

class IDatabaseLoader : public CDsObject {
public:
    ~IDatabaseLoader() override = default;
    [[nodiscard]] virtual bool Init() = 0;
    [[nodiscard]] virtual bool ExecSQL(std::string_view sql) = 0;
    [[nodiscard]] virtual std::shared_ptr<CDsList> LoadUrl() = 0;
    [[nodiscard]] virtual std::shared_ptr<CDsList> LoadContent() = 0;
    [[nodiscard]] virtual std::shared_ptr<CDsList> LoadClientIp() = 0;
    [[nodiscard]] virtual std::shared_ptr<CDsList> LoadRegClient() = 0;
};

extern "C" {
    [[nodiscard]] int DsCreateForbidClientReader(std::shared_ptr<IForbidClientReader>& ppReader);
    [[nodiscard]] int DsCreateForbidUrlReader(std::shared_ptr<IForbidUrlReader>& ppReader);
    [[nodiscard]] int DsCreateForbidContentReader(std::shared_ptr<IForbidContentReader>& ppReader);
    [[nodiscard]] int DsCreateRegClientReader(std::shared_ptr<IRegClientReader>& ppReader);
    [[nodiscard]] int DsCreateDatabaseLoader(std::shared_ptr<IDatabaseLoader>& ppILoader);
}

#endif // __NG_DB_H__

// ngdb.cpp
#include "ngdb.h"

int COracleReader::ConnectToOracle(std::string_view userName, std::string_view password) {
    // Implement the connection logic here
    return 1; // For demonstration purposes, always return success
}

void COracleReader::Disconnect() {
    // Implement the disconnection logic here
}

int COracleReader::ExecSQL(std::string_view sql) {
    // Implement the SQL execution logic here
    return 1; // For demonstration purposes, always return success
}

void COracleReader::SqlError() {
    // Implement the SQL error handling logic here
}

extern "C" {
    int DsCreateForbidClientReader(std::shared_ptr<IForbidClientReader>& ppReader) {
        ppReader = std::make_shared<IForbidClientReader>();
        return ppReader ? 1 : 0;
    }

    int DsCreateForbidUrlReader(std::shared_ptr<IForbidUrlReader>& ppReader) {
        ppReader = std::make_shared<IForbidUrlReader>();
        return ppReader ? 1 : 0;
    }

    int DsCreateForbidContentReader(std::shared_ptr<IForbidContentReader>& ppReader) {
        ppReader = std::make_shared<IForbidContentReader>();
        return ppReader ? 1 : 0;
    }

    int DsCreateRegClientReader(std::shared_ptr<IRegClientReader>& ppReader) {
        ppReader = std::make_shared<IRegClientReader>();
        return ppReader ? 1 : 0;
    }

    int DsCreateDatabaseLoader(std::shared_ptr<IDatabaseLoader>& ppILoader) {
        ppILoader = std::make_shared<IDatabaseLoader>();
        return ppILoader ? 1 : 0;
    }
}

//BY GST ARMV8 GCC 13.2