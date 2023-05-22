#ifndef FAKE_DATA_SOURCE_HPP
#define FAKE_DATA_SOURCE_HPP

#include <fstream>
#include <vector>
#include <string>

namespace sat {

class FakeDataSource {
public:
    explicit FakeDataSource(const std::string& a_filename);
    FakeDataSource(const FakeDataSource& a_other) = delete;
    FakeDataSource& operator=(const FakeDataSource& a_other) = delete;
    ~FakeDataSource();

    std::string ReadData();

private:
    std::ifstream m_fd;
};


} //sat

#include "inl/fake_data_source.hxx"

#endif