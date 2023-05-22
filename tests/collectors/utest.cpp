#include "mu_test.h"

#include <thread>
#include <iostream>
#include "collectors.hpp"

using namespace sat;
std::shared_ptr<SqliteDatabase> db = std::make_shared<SqliteDatabase>("../../sql/housekeeping.sqlt");

BEGIN_TEST(comm_collector_with_stop_start)
    CommunicationCollector comColl(db);
    DatabaseBookmark bookmark;

    std::thread t1([&comColl]() { comColl.Operate(); });
    comColl.Stop();
    comColl.Start();
    t1.detach();

    std::string data;
    size_t count = 0;
    while(count < 10) {
        data = db->Read(details::comm_table, bookmark);
        std::cout << data << std::endl;
        ++count;
    }

    db->Delete(details::comm_table, bookmark.GetBookmark());

    ASSERT_PASS();
END_TEST


BEGIN_TEST(all_collectors)
    CommunicationCollector comCol(db);
    AdcsCollector adcsCol(db);
    EpsCollector epsCol(db);
    TempCollector tempCol(db);
    SunCollector sunCol(db);

    std::thread t1([&comCol]() { comCol.Operate(); });
    std::thread t2([&adcsCol]() { adcsCol.Operate(); });
    std::thread t3([&epsCol]() { epsCol.Operate(); });
    std::thread t4([&tempCol]() { tempCol.Operate(); });
    std::thread t5([&sunCol]() { sunCol.Operate(); });

    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();
    t5.detach();

    std::string data;
    size_t count = 0;
    while(count < 5) {
        data = db->Read(details::comm_table);
        std::cout << data << std::endl;

        data = db->Read(details::adcs_table);
        std::cout << data << std::endl;

        data = db->Read(details::eps_table);
        std::cout << data << std::endl;

        data = db->Read(details::temp_table);
        std::cout << data << std::endl;

        data = db->Read(details::sun_table);
        std::cout << data << std::endl;

        ++count;
    }

    ASSERT_PASS();
END_TEST


BEGIN_SUITE(collectors)
    //TEST(comm_collector_with_stop_start)
    TEST(all_collectors)
END_SUITE
