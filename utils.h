#pragma once

#include <sstream>
#include <string>

namespace utils {

class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

std::string TransformToSpecialChars(const std::string& text);
std::string ScreenSpecialChars(const std::string& value);

}