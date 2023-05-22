#include <sstream>

#include "fake_data_source.hpp"
#include "sqlite_database.hpp"

namespace sat {


inline FakeDataSource::FakeDataSource(const std::string& a_filename)
: m_fd(a_filename.c_str())
{}


inline FakeDataSource::~FakeDataSource()
{
    m_fd.close();
}


inline std::string FakeDataSource::ReadData()
{
    char line[details::collected_data_length];
    m_fd.getline(line, details::collected_data_length);
    

    return line;
}


} //sat