#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

namespace sat {

class Parser {
public:
    Parser() = default;
    Parser(const Parser& a_other) = default;
    Parser& operator=(const Parser& a_other) = default;
    ~Parser() = default;

    static std::vector<std::string> ParseLine(char* a_line, size_t a_elements);
};


} //sat

#include "inl/parser.hxx"

#endif