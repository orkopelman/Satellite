#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <string>

#include "database_bookmark.hpp"

namespace sat {

namespace details {
    constexpr char comm_table[] = "comm_table";
    constexpr char adcs_table[] = "adcs_table";
    constexpr char eps_table[] = "eps_table";
    constexpr char temp_table[] = "temp_table";
    constexpr char sun_table[] = "sun_table";
    constexpr size_t db_query_length = 200;
    constexpr size_t collected_data_length = 200;
} //details


class SqliteDatabase {
public:
    explicit SqliteDatabase(const std::string& a_databaseName);
    SqliteDatabase(const SqliteDatabase& a_other) = default;
    SqliteDatabase& operator=(const SqliteDatabase& a_other) = default;
    ~SqliteDatabase() = default;

    void Insert(const std::string& a_query) noexcept;
    std::string Read(const std::string& a_tableName) const noexcept;
    std::string Read(const std::string& a_tableName, DatabaseBookmark& a_bookmark) const noexcept;
    std::string Read(const std::string& a_tableName, uint64_t a_time) const noexcept;
    void Delete(const std::string& a_tableName, uint64_t a_limit) noexcept;

private:
    using callback = int (*)(void*,int,char**,char**);
    static int StoreData(void* a_buffer, int a_colsNum, char** a_rowsData, char** a_colsName) noexcept;
    void ExecuteQuery(const char* a_query, callback a_callbackFunc, void* a_callbackArg) const noexcept;
    uint64_t GetNewMark(const char* a_data) const noexcept;

private:
    sqlite3* m_sqlite;
};


} //sat

#endif