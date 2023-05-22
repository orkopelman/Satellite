#ifndef DATABASE_BOOKMARK_HPP
#define DATABASE_BOOKMARK_HPP

namespace sat {

class DatabaseBookmark {
public:
    DatabaseBookmark();
    DatabaseBookmark(const DatabaseBookmark& a_other) = default;
    DatabaseBookmark& operator=(const DatabaseBookmark& a_other) = default;
    ~DatabaseBookmark() = default;

    uint64_t GetBookmark() const noexcept;
    void SetBookmark(uint64_t a_newBookmark) noexcept;

private:
    uint64_t m_bookmark;
};

} //sat

#include "inl/database_bookmark.hxx"

#endif