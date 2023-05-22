#include "database_bookmark.hpp"

namespace sat {

inline DatabaseBookmark::DatabaseBookmark()
: m_bookmark(0)
{}

inline uint64_t DatabaseBookmark::GetBookmark() const noexcept
{
    return m_bookmark;
}

inline void DatabaseBookmark::SetBookmark(uint64_t a_newBookmark) noexcept
{
    m_bookmark = a_newBookmark;
}

} //sat