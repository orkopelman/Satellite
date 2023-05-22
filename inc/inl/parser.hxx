#include <sstream>

#include "parser.hpp"

namespace sat {

inline std::vector<std::string> Parser::ParseLine(char* a_line, size_t a_elements)
{
    std::vector<std::string> vec;
    vec.reserve(a_elements);
    std::stringstream ss(a_line);

    for (size_t i=0; i < a_elements; ++i) {
        std::string elem;
        ss >> elem;
        vec.push_back(elem);
    }

    return vec;
}


} //sat