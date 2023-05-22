#include <memory>
#include <iostream>
#include "sqlite_database.hpp"


int main()
{
    using namespace sat;

    std::shared_ptr<SqliteDatabase> db = std::make_shared<SqliteDatabase>("../../sql/housekeeping.sqlt");
    DatabaseBookmark commBookmark;
    
    uint64_t t = 10, size = 25000;
    int address = 127001, port = 5656;
    char query[100];
    sprintf(query, "INSERT INTO comm_table (time, ipAddress, port, size) VALUES (%ld, %d, %d, %ld)", t, address, port, size);
    db->Insert(query);
    
    std::string data = db->Read(details::comm_table, commBookmark);
    std::cout << data << std::endl;

    t = 20;
    address = 127002;
    sprintf(query, "INSERT INTO comm_table (time, ipAddress, port, size) VALUES (%ld, %d, %d, %ld)", t, address, port, size);
    db->Insert(query);

    t = 30;
    address = 127003;
    sprintf(query, "INSERT INTO comm_table (time, ipAddress, port, size) VALUES (%ld, %d, %d, %ld)", t, address, port, size);
    db->Insert(query);

    data = db->Read(details::comm_table); //most recent data
    std::cout << data << std::endl;

    db->Delete(details::comm_table, t);
}
