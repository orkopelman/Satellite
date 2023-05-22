#include <cstring> //strcat
#include <iostream> //cerr
#include <sstream>

#include "sqlite_database.hpp"

namespace sat {


SqliteDatabase::SqliteDatabase(const std::string& a_databaseName)
: m_sqlite()
{
    int retval = sqlite3_open(a_databaseName.c_str(), &m_sqlite);
    if (retval) {
        std::cerr << sqlite3_errmsg(m_sqlite) << std::endl;
        exit(1);
    }
}


void SqliteDatabase::Insert(const std::string& a_query) noexcept
{
    ExecuteQuery(a_query.c_str(), NULL, NULL);
}


std::string SqliteDatabase::Read(const std::string& a_tableName) const noexcept
{
    char query[details::db_query_length];
    sprintf(query, "SELECT * FROM %s ORDER BY time DESC LIMIT 1", a_tableName.c_str());

    char data[details::collected_data_length] = "";
    ExecuteQuery(query, StoreData, data);
        
    return std::string(data);
}


std::string SqliteDatabase::Read(const std::string& a_tableName, DatabaseBookmark& a_bookmark) const noexcept
{
    uint64_t mark = a_bookmark.GetBookmark();

    char query[details::db_query_length];
    sprintf(query, "SELECT * FROM %s WHERE time > %ld LIMIT 1", a_tableName.c_str(), mark);

    char data[details::collected_data_length] = "";
    ExecuteQuery(query, StoreData, data);

    if (strcmp(data, "") != 0) {
        mark = GetNewMark(data);
        a_bookmark.SetBookmark(mark);
    }
        
    return std::string(data);
}


std::string SqliteDatabase::Read(const std::string& a_tableName, uint64_t a_time) const noexcept
{
    char query[details::db_query_length];
    sprintf(query, "SELECT * FROM %s WHERE time = %ld", a_tableName.c_str(), a_time);

    char data[details::collected_data_length] = "";
    ExecuteQuery(query, StoreData, data);
        
    return std::string(data);
}


void SqliteDatabase::Delete(const std::string& a_tableName, uint64_t a_limit) noexcept
{
    char query[details::db_query_length];
    sprintf(query, "DELETE FROM %s WHERE time <= %ld", a_tableName.c_str(), a_limit);
    ExecuteQuery(query, NULL, NULL);
}


int SqliteDatabase::StoreData(void* a_buffer, int a_colsNum, char** a_rowsData, char** a_colsName) noexcept
{
    for (int i=0; i < a_colsNum; ++i) {
        if (a_rowsData[i]) {
            strcat(static_cast<char*>(a_buffer), a_rowsData[i]);
            strcat(static_cast<char*>(a_buffer), " ");
        }
    }
    return 0;
}


void SqliteDatabase::ExecuteQuery(const char* a_query, callback a_callbackFunc, void* a_callbackArg) const noexcept
{
    char* errorMsg = 0;
    int retval = sqlite3_exec(m_sqlite, a_query, a_callbackFunc, a_callbackArg, &errorMsg);
    if (retval != SQLITE_OK) {
        std::cerr << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        sqlite3_close(m_sqlite);
        exit(1);
    }
}


uint64_t SqliteDatabase::GetNewMark(const char* a_data) const noexcept
{
    std::stringstream ss(a_data);
    std::string time;

    ss >> time;
    
    uint64_t mark = std::stoi(time.c_str());
    return mark;
}


} //sat